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
#include "lsm9ds1.h"

void calibrate_accelerometer(void){
	pos1 = 1700;
	pos2 = 4694;
	pos3 = 8172;



	//TIM_SetCompare4(TIM2,pw1);
	TIM_SetCompare1(TIM16,pos1);
	// measure accelerometer




}

void exercise_4_2_motorControl(void) {
	//init_spi_lsm9ds1();
	ADC_setup_PA();
	GPIO_set_AF1_PA6();
	GPIO_set_AF1_PB11();
	TIM2_init_50Hz();
	TIM16_init_50Hz();
	TIM16_PWM_init();
	TIM2_PWM_init();

	uint16_t pw1 = 0;
	uint16_t pw2 = 0;

	uint16_t pot1;
	uint16_t pot2;


	//uint8_t data_out;
	while (1) {
		//data_out = lsm9ds1_read(WHO_AM_I);
		//printf("Received data = %X02\n", data_out);

    	pot1 = ADC_measure_PA(1);
    	pot2 = ADC_measure_PA(2);

    	pw1 = pot1 << 1;
    	pw2 = pot2 << 1;



		TIM_SetCompare4(TIM2,pw1);
		TIM_SetCompare1(TIM16,pw2);



	}

	/* 1. control motor via PWM
	 * 2. change timers to have higher resolution on the ~20% of the PWM
	 * 3. routine to move servo first to normal position, then to inverted position, then to upright position.
	 * read data from gyroscope for each and calibrate as such.
	 * maximal position for pwm signal (rotate servo all the way to the front: 8172
	 * upright position: 4694
	 * down position: 1700
	 *
	 */


}
