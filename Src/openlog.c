#include "openlog.h"
#include "stm32f30x_usart.h"

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 4000; i++) {
        __NOP();
    }
}

void USART1_setup(){
	/* pins used:
	 * TX1: PA9
	 * RX1: PA10
	 * RTS1: PA12
	 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// RX Pin (PA10)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE); // Enable USART1

}

void USART1_SendString(const char *str)
	{
	    while (*str != '\0')
	    {
	        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	        USART_SendData(USART1, (uint16_t)*str);
	        str++;
	    }
	    // Wait for transmission to fully complete (optional)
	    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void openlog_init(uint32_t baud) {
    // Enable Clocks for GPIOA and USART1
    RCC->AHBENR  |= RCC_AHBPeriph_GPIOA;
    RCC->APB2ENR |= RCC_APB2Periph_USART1;

    // Connect PA9 and PA10 to USART1 (Alternate Function 7)
    GPIOA->AFR[1] &= ~((0x0F << ((9 - 8) * 4)) | (0x0F << ((10 - 8) * 4)));
    GPIOA->AFR[1] |=  ((0x07 << ((9 - 8) * 4)) | (0x07 << ((10 - 8) * 4)));

    // Configure PA9 and PA10 for 10 MHz Alternate Function
    GPIOA->OSPEEDR &= ~((0x03 << (9 * 2)) | (0x03 << (10 * 2)));
    GPIOA->OSPEEDR |=  ((0x01 << (9 * 2)) | (0x01 << (10 * 2)));
    GPIOA->OTYPER  &= ~((0x01 << 9) | (0x01 << 10));
    GPIOA->MODER   &= ~((0x03 << (9 * 2)) | (0x03 << (10 * 2)));
    GPIOA->MODER   |=  ((0x02 << (9 * 2)) | (0x02 << (10 * 2)));
    GPIOA->PUPDR   &= ~((0x03 << (9 * 2)) | (0x03 << (10 * 2)));
    GPIOA->PUPDR   |=  ((0x01 << (9 * 2)) | (0x01 << (10 * 2)));

    // Connect PA12 to USART1_RTS (Alternate Function 7)
    GPIOA->AFR[1] &= ~(0x0F << ((12 - 8) * 4));
    GPIOA->AFR[1] |=  (0x07 << ((12 - 8) * 4));

    // Configure PA12 for alternate function push-pull, pull-up
    GPIOA->OSPEEDR &= ~(0x03 << (12 * 2));
    GPIOA->OSPEEDR |=  (0x01 << (12 * 2));
    GPIOA->OTYPER  &= ~(0x01 << 12);
    GPIOA->MODER   &= ~(0x03 << (12 * 2));
    GPIOA->MODER   |=  (0x02 << (12 * 2));
    GPIOA->PUPDR   &= ~(0x03 << (12 * 2));
    GPIOA->PUPDR   |=  (0x01 << (12 * 2));

    // Configure USART1 registers
    USART1->CR1 &= ~0x00000001; // Disable USART1
    USART1->CR2 &= ~0x00003000; // 1 stop bit
    USART1->CR1 &= ~(0x00001000 | 0x00000400 | 0x00000200); // 8 bits, no parity
    USART1->CR1 |=  (0x00000004 | 0x00000008); // Enable RX and TX
    USART1->CR3 &= ~0x00000200;   // Disable CTS (bit 9)
    USART1->CR3 |=  0x00000100;   // Enable RTS  (bit 8)

    // Set Baud Rate (9600 Baud @ 8 MHz clock = 833)
    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    uint32_t apbclock = RCC_ClocksStatus.USART1CLK_Frequency;
    if (apbclock == 0) apbclock = 8000000;

    USART1->BRR = (uint16_t)(apbclock / baud);
    USART1->CR1 |= 0x00000001; // Enable USART1
}

// Transmit a single character to OpenLog over USART1
void openlog_put_char(uint8_t c) {
    USART_SendData(USART1, (uint8_t)c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
}

// Transmit a string to OpenLog over USART1
void openlog_put_string(const char *str) {
    while (*str) {
        openlog_put_char((uint8_t)*str++);
    }
}

// Receive a single character from OpenLog over USART1 (blocking)
uint8_t openlog_get_char(void) {

    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {}
    return (uint8_t)(USART_ReceiveData(USART1) & 0xFF);
}

// Send 3x Ctrl+Z (ASCII 26) to force OpenLog into Command Mode ( prompt '>')
void openlog_enter_command_mode(void) {
    delay_ms(100);
    openlog_put_char(26); // Ctrl+Z
    openlog_put_char(26); // Ctrl+Z
    openlog_put_char(26); // Ctrl+Z
    delay_ms(200);        // Wait for OpenLog prompt change
}

// Step 1: Automate creating/opening a file and writing short text to it
void openlog_write_text_file(const char *filename, const char *text) {
    // 1. Drop into Command Mode
    openlog_enter_command_mode();

    // 2. Issue 'append <filename>\r' command to create/open the target file
    openlog_put_string("append ");
    openlog_put_string(filename);
    openlog_put_string("\r"); // Commands must terminate with carriage return '\r'
    delay_ms(200);

    // 3. Send text contents to be saved into the SD file
    openlog_put_string(text);
    delay_ms(100);

    // 4. Send Ctrl+Z x3 to exit append mode and save file to SD card
    openlog_enter_command_mode();
}

// Step 2: Automate reading the text file from SD card and printing response to PuTTY
void openlog_read_file_to_putty(const char *filename) {
    // 1. Ensure OpenLog is in Command Mode
    openlog_enter_command_mode();

    printf("\n\r--- Reading '%s' from SD card ---\n\r", filename);

    // 2. Send 'read <filename>\r' command to OpenLog
    openlog_put_string("read ");
    openlog_put_string(filename);
    openlog_put_string("\r");

    // 3. Receive character stream from OpenLog (USART1) and forward to PuTTY (USART2)
    for (uint32_t timeout = 0; timeout < 2000; timeout++) {
        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
            uint8_t ch = openlog_get_char();
            uart_put_char(ch); // Direct pass-through to PuTTY via 30010_io library
            timeout = 0;       // Reset inactivity timer when data arrives
        }
        delay_ms(1);
    }

    printf("\n\r--- End of File Stream ---\n\r");
}
