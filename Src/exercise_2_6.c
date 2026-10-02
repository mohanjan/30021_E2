#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"
#include "flash.h"
#include "addac.h"
#include "gpio.h"

void exercise_2_6(void)
{
	init_spi_lcd();
	ADC_setup_PA();
    initJoystick();
    initEXTI();


	uint16_t V_CALC = ADC_reference();
	uint16_t adc1, adc2;
	float adc1vol, adc2vol;

	uint8_t fbuffer[512];
	char buffer[32];

	uint8_t old_joystick_state = 0xFF;

	printf("V_ref: %u mV\n", V_CALC);

    sprintf(buffer, "ADC1: 0", adc1);
	lcd_write_string(buffer, fbuffer, 0, 0);

    sprintf(buffer, "ADC2: 0", adc2);
	lcd_write_string(buffer, fbuffer, 0, 1);

    sprintf(buffer, " ");
	lcd_write_string(buffer, fbuffer, 0, 2);
	lcd_write_string(buffer, fbuffer, 0, 3);
	lcd_push_buffer(fbuffer);

    while (1)
    {
    	//check if interrupt has been set
    	if (joystick_flag){
    		joystick_flag = 0;

    		sprintf(buffer, "Calibrate ADC?");
    		lcd_write_string(buffer, fbuffer, 0, 0);

    		sprintf(buffer, "UP: CHANNEL 0");
    		lcd_write_string(buffer, fbuffer, 0, 1);

    		sprintf(buffer, "DOWN: CHANNEL 1");
    		lcd_write_string(buffer, fbuffer, 0, 2);

    		sprintf(buffer, "LEFT: EXIT");
    		lcd_write_string(buffer, fbuffer, 0, 3);
    		lcd_push_buffer(fbuffer);

    		if (current_joystick_state != old_joystick_state ){
    			__WFI();

    			if(current_joystick_state == JOYSTICK_UP){
    	    		sprintf(buffer, "Calibrating ADC1ch0");
    	    		lcd_write_string(buffer, fbuffer, 0, 0);

    	    		sprintf(buffer, "Please connect Multimeter");
    	    		lcd_write_string(buffer, fbuffer, 0, 1);

    	    		sprintf(buffer, "Probes to pin");
    	    		lcd_write_string(buffer, fbuffer, 0, 2);

    	    		sprintf(buffer, "PA0");
    	    		lcd_write_string(buffer, fbuffer, 0, 3);
    	    		lcd_push_buffer(fbuffer);

    				__WFI();

    				//do the function
    			}

    			if(current_joystick_state == JOYSTICK_DOWN){
    	    		sprintf(buffer, "Calibrating ADC1ch1");
    	    		lcd_write_string(buffer, fbuffer, 0, 0);

    	    		sprintf(buffer, "Please connect Multimeter");
    	    		lcd_write_string(buffer, fbuffer, 0, 1);

    	    		sprintf(buffer, "Probes to pin");
    	    		lcd_write_string(buffer, fbuffer, 0, 2);

    	    		sprintf(buffer, "PA1");
    	    		lcd_write_string(buffer, fbuffer, 0, 3);
    	    		lcd_push_buffer(fbuffer);
    			}

    			if(current_joystick_state == JOYSTICK_LEFT){}

    		}

    	}




    	//VREFINT_CAL;
    	adc1 = ADC_measure_PA(1);
    	adc1vol = 3.3/(4085)*adc1;
        sprintf(buffer, "ADC1: %4g V",
    			adc1vol);
    	lcd_write_string(buffer, fbuffer, 0, 0);

    	adc2 = ADC_measure_PA(2);
    	adc2vol = 3.3/(4085)*adc2;
        sprintf(buffer, "ADC2: %4g V",
    			adc2vol);
    	lcd_write_string(buffer, fbuffer, 0, 1);

        sprintf(buffer, "V_CALC: %4u mV", V_CALC);
    	lcd_write_string(buffer, fbuffer, 0, 3);
    	lcd_push_buffer(fbuffer);





    }
}
