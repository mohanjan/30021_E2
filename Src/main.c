#include "stm32f30x_conf.h"
#include "30010_io.h"
#include <stdio.h>
#include <inttypes.h>

#include "gpio.h"
#include "exercises.h"

int main(void) {
	uart_init(9600);

	exercise_4_2_motorControl();
	while(1)
	{

	}
}
