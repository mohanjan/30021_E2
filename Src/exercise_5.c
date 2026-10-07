#include "lsm9ds1.h"
#include "timer.h"
#include "openlog.h"
#include "stm32f30x.h"



void exercise_5(void) {
	openlog_init(9600);

	while (1) {
		openlog_put_string("Hello there \n");
	}


}
