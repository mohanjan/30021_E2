#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>
#include "lcd.h"
#include "string.h"
#include "flash.h"
//#include "addac.h"
#include "gpio.h"
#include "timer.h"

void exercise_3_1(void)
{
	GPIO_set_AF1_PA6();
	TIM16_init();
	TIM16_PWM_init();



    while (1)
    {

    }
}
