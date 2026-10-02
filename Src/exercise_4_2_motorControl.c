#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"
#include "flash.h"
#include "addac.h"
#include "joystick.h"
#include "timer.h"
#include "lsm9ds1.h"



void exercise_4_2_motorControl(void) {
	init_spi_lsm9ds1();
	ADC_setup_PA();
	GPIO_set_AF1_PA6();
	GPIO_set_AF1_PB11();
	TIM2_init_50Hz();
	TIM16_init_50Hz();
	TIM16_PWM_init();
	TIM2_PWM_init();

	uint8_t data_out;
	while (1) {
		data_out = lsm9ds1_read(WHO_AM_I);
		printf("Received data = %X02\n", data_out);
	}
}
