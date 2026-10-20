#include "lsm9ds1.h"
#include "timer.h"
#include "openlog.h"
#include "stm32f30x.h"



void exercise_5(void) {
	openlog_init(9600);
	openlog_put_string("new file1.txt\r");
	openlog_put_string("Testing the functionality of the openlogger device.\n");
	//printf("Testing the functionality of the openlogger device.\n");
	uint8_t a;
	while (1) {
		a = openlog_get_char();
		printf(a);
	}


}
