#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"
#include "flash.h"
#include "addac.h"
#include "gpio.h"
#include "timer.h"

void exercise_3_2(void)
{
	init_spi_lcd();
	ADC_setup_PA();
	GPIO_set_AF1_PA6();
	TIM16_init();
	TIM16_PWM_init();
	uint8_t pw = 128;
	uint16_t V_CALC = ADC_reference();
	printf("V_ref: %u mV\n", V_CALC);

	uint16_t adc1, adc2;
	float adc1vol, adc2vol;

	uint8_t fbuffer[512];
	char buffer[32];


    while (1)
    {

    	adc1 = ADC_measure_PA(1);
    	adc1vol = ((float)V_CALC/1000)/(4085)*adc1;
        sprintf(buffer, "ADC1: %4g V",
    			adc1vol);
    	lcd_write_string(buffer, fbuffer, 0, 0);

    	lcd_push_buffer(fbuffer);

    	if(adc1vol> 1.0 && pw>0){
    		pw--;
    		TIM_SetCompare1(TIM16,pw);
    	}
    	if(adc1vol< 1.0 && pw <255){
    	    pw++;
    	    TIM_SetCompare1(TIM16,pw);
    	}

    }
}
