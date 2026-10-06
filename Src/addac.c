#include "stm32f30x.h"
#include <stdio.h>
#include "addac.h"


extern float calfact0 = 1.0f;
extern float calfact1 = 1.0f;

void ADC_setup_PA(){
	//PA0 & PA1 as input pins for potentiometers
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div8);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	ADC_InitTypeDef ADC_InitStruct;

	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ExternalTrigConvEvent = 0;
	ADC_InitStruct.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_OverrunMode = DISABLE;
	ADC_InitStruct.ADC_AutoInjMode = DISABLE;
	ADC_InitStruct.ADC_NbrOfRegChannel = 1;

	//sequencer length

	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_Cmd(ADC1,ENABLE);



	//calibration
	ADC_VoltageRegulatorCmd(ADC1,ENABLE);
	for(uint32_t i = 0; i<100;i++);

	ADC_Cmd(ADC1,DISABLE);
	while(ADC_GetDisableCmdStatus(ADC1)){} // wait for disable of ADC
	ADC_SelectCalibrationMode(ADC1,ADC_CalibrationMode_Single);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)){}
	for(uint32_t i = 0; i<100;i++);

	//enable ADC
	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_RDY)){}




}

uint16_t ADC_reference(){
	ADC_Cmd(ADC1,DISABLE);
	ADC_VrefintCmd(ADC1,ENABLE); // setup ref voltage to channel 18
	for(uint32_t i = 0; i<10000;i++); // I think this is needed...
	ADC_Cmd(ADC1,ENABLE);
	// turn on ADC

	ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_601Cycles5);
	for(uint32_t i = 0; i<10000;i++); // I think this is needed...
	ADC_StartConversion(ADC1); // Start ADC read

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); // Wait for ADC read
	uint16_t sampled_value = ADC_GetConversionValue(ADC1);
	uint16_t vref_calc = 0;
	vref_calc = (uint16_t) (3300 * ((float)VREFINT_CAL/(float)sampled_value));
	ADC_VrefintCmd(ADC1,DISABLE); // setup ref voltage to channel 18
	return vref_calc;
}

uint16_t ADC_measure_PA(uint8_t ch){

	if(ch == 1){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);

	}

	if(ch == 2){
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
	}

	ADC_StartConversion(ADC1); // Start ADC read
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0); // Wait for ADC read


	//insert correction factor here?
	return ADC_GetConversionValue(ADC1);
}

float average_voltage(uint8_t channel)
{
    float sum = 0.0f;

    for (int i = 0; i < 16; i++)
    {
        sum += ADC_measure_PA(channel);
    }

    return sum / 16.0f;
}


void write_calfacts_flash(float cf0, float cf1)
{
    FLASH_Unlock();
    FLASH_ErasePage(CALFACT_PAGE_ADDR);          /* erase ONCE */
    /* program both words at their offsets, no erase between them */
    FLASH_ProgramWord(CALFACT_PAGE_ADDR + CALFACT0_OFFSET * 4, *(uint32_t*)&cf0);
    FLASH_ProgramWord(CALFACT_PAGE_ADDR + CALFACT1_OFFSET * 4, *(uint32_t*)&cf1);
    FLASH_Lock();
}

void load_calibration(void)
{
    float stored0;
    float stored1;

    stored0 = read_float_flash(CALFACT_PAGE_ADDR, CALFACT0_OFFSET);
    stored1 = read_float_flash(CALFACT_PAGE_ADDR, CALFACT1_OFFSET);

    /* PA0 calibration factor */
    if ((stored0 > 0.5f) && (stored0 < 1.5f))
    {
        calfact0 = stored0;
    }
    else
    {
        calfact0 = 1.0f;
    }

    /* PA1 calibration factor */
    if ((stored1 > 0.5f) && (stored1 < 1.5f))
    {
        calfact1 = stored1;
    }
    else
    {
        calfact1 = 1.0f;
    }

    printf("Loaded calibration:\r\n");
    //printf("CALFACT0: %.6f\r\n", calfact0);
    //printf("CALFACT1: %.6f\r\n", calfact1);
}


void calibrate_adc()
{
    float pa0_avg;
    float pa1_avg;

    printf("\r\n");float calibrated_voltage_PA(uint8_t channel)
    {
        float voltage;

        voltage = ADC_get_voltage_PA(channel);

        if (channel == 1)
        {
            voltage *= calfact0;
        }
        else if (channel == 2)
        {
            voltage *= calfact1;
        }

        return voltage;
    }
    printf("========================================\r\n");
    printf("ADC CALIBRATION\r\n");
    printf("========================================\r\n");
    printf("Expected voltage: 3.200 V\r\n");
    printf("Taking 16 measurements per channel...\r\n");

    /* Average 16 measurements */
    pa0_avg = average_voltage(1);
    pa1_avg = average_voltage(2);

    //printf("PA0_Vavg: %.6f V\r\n", pa0_avg);
    //printf("PA1_Vavg: %.6f V\r\n", pa1_avg);

    /* Calculate correction factors */
    if (pa0_avg > 0.0f)
    {
        calfact0 = 3.200f / pa0_avg;
    }
    else
    {
        calfact0 = 1.0f;
    }

    if (pa1_avg > 0.0f)
    {
        calfact1 = 3.200f / pa1_avg;
    }
    else
    {
        calfact1 = 1.0f;
    }

    //printf("CALFACT0: %.6f\r\n", calfact0);
    //printf("CALFACT1: %.6f\r\n", calfact1);

    /* Store calibration factors in flash */
    write_calfacts_flash(calfact0, calfact1);

    printf("Calibration factors saved to flash.\r\n");
    printf("========================================\r\n");
}

float calibrated_voltage_PA(uint8_t channel)
{
    float voltage;

    voltage = ADC_get_voltage_PA(channel);

    if (channel == 1)
    {
        voltage *= calfact0;
    }
    else if (channel == 2)
    {
        voltage *= calfact1;
    }

    return voltage;
}

