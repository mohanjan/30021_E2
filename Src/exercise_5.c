#include "openlog.h"
#include "30010_io.h"   // Handles USART2 (USB connection to PC / PuTTY)
#include "stm32f30x.h"
#include <stdio.h>

void exercise_5(void) {
    // 1. Initialize USART2 for PuTTY at 9600 baud
    uart_init(9600);

    // 2. Initialize USART1 for OpenLog at 9600 baud (with fixed clock calculation)
    openlog_init(9600);

    printf("\r\n==================================================\r\n");
    printf("--- OpenLog Manual Serial Pass-Through Bridge ---\r\n");
    printf("Instructions:\r\n");
    printf("1. Type 'Ctrl+Z' 3 times in PuTTY to enter Command Mode (> prompt).\r\n");
    printf("2. Type 'append NOTE.TXT' to open/create a file for logging.\r\n");
    printf("3. Type 'read NOTE.TXT' to dump file contents to screen.\r\n");
    printf("==================================================\r\n\r\n");

    // 3. Continuous Pass-Through Loop
    while (1) {
        // Forward typed characters from PC (PuTTY / USART2) to OpenLog (USART1)
        if (uart_get_count() > 0) {
            uint8_t pc_char = uart_get_char();
            openlog_put_char(pc_char);
        }

        // Forward response characters from OpenLog (USART1) to PC (PuTTY / USART2)
        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
            uint8_t openlog_char = openlog_get_char();
            uart_put_char(openlog_char);
        }
    }
}
