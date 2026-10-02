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

void exercise_3_3(void)
{
	ADC_setup_PA();
	GPIO_set_AF1_PA6();
	GPIO_set_AF1_PB11();
	TIM2_init_50Hz();
	TIM16_init_50Hz();
	TIM16_PWM_init();
	TIM2_PWM_init();

	//no need to connect the PDIP output to the board, it just needs to be connected directly to the servo


	uint8_t pw1 = 128;
	uint8_t pw2 = 128;

	uint16_t pot1;
	uint16_t pot2;


    while (1)
    {
    	pot1 = ADC_measure_PA(1);
    	pot2 = ADC_measure_PA(2);

    	pw1 = pot1 >> 7;
    	pw2 = pot2 >> 7;

		TIM_SetCompare4(TIM2,pw1);
		TIM_SetCompare1(TIM16,pw2);

    }
}
