#include "lsm9ds1.h"
#include "timer.h"
#include "openlog.h"
#include "stm32f30x.h"



void exercise_5(void) {
	openlog_init(9600);
	/*openlog_put_string("new file1.txt\r");
	openlog_put_string("Testing the functionality of the openlogger device.\n");
	//printf("Testing the functionality of the openlogger device.\n");
	uint8_t a;
	while (1) {
		a = openlog_get_char();
		printf(a);
	}*/

	printf("\r\n=============================================\r\n");
	printf("--- OpenLog Automated Write & Read Test ---\r\n");
	printf("=============================================\r\n");

	// Startup delay for SD card initialization
	delay_ms(1000);

	const char *file_name = "NOTE.TXT";
	const char *text_content = "Hello PuTTY!\r\nThis text was written automatically by the NUCLEO board to the SD card.\r\n";

	// 1. Create file and write text
	printf("\n\r[1/2] Writing short text to '%s'...\n\r", file_name);
	openlog_write_text_file(file_name, text_content);
	printf("Write complete.\n\r");

	delay_ms(500);

	// 2. Read file back and print in PuTTY
	printf("\n\r[2/2] Requesting OpenLog to dump '%s' back...\n\r", file_name);
	openlog_read_file_to_putty(file_name);

}
