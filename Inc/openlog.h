#ifndef OPENLOG_H_
#define OPENLOG_H_

#include "stm32f30x_usart.h"

void USART1_setup();
void USART1_SendString(const char *str);
void openlog_init(uint32_t baud);
void openlog_put_char(uint8_t c);
void openlog_put_string(const char *str);
uint8_t openlog_get_char(void);
void delay_ms(uint32_t ms);
void openlog_enter_command_mode(void);
void openlog_write_text_file(const char *filename, const char *text);
void openlog_read_file_to_putty(const char *filename);


#endif
