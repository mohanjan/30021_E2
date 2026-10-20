#include "lsm9ds1.h"
#include "timer.h"
#include "openlog.h"
#include "stm32f30x.h"
#include "30010_io.h"



void exercise_5(void) {
	openlog_init(9600);
	//openlog_put_char(0x1A);
	//openlog_put_char(0x1A);
	//openlog_put_char(0x1A);

	openlog_put_string("new file1.txt\r");
	openlog_put_string("Testing the functionality of the openlogger device.\n");

	uint8_t * buf;
	buf = openlog_get_response();
	printf(buf);

	uint8_t n = 0;
	while (1) {
		openlog_put_char(n);
		openlog_put_string("hello \n");
		n++;
		uart2_put_string(openlog_get_response());
		uart2_put_string("hello \n \r");
	}


}
