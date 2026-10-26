#include "gpio.h"
#include "timer.h"

void uart_send_string(const char *str)
{
    while (*str)
    {
        // Wait until Transmit Data Register is empty (TXE)
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

        // Send current character
        USART_SendData(USART2, (uint8_t)*str++);
    }
}

// Global Time Registers
volatile Time_t clock_time = {0, 0, 0, 0};
volatile uint8_t clock_running = 0;
volatile uint8_t second_changed_flag = 0;

// Split Time Storage
Time_t split1 = {0, 0, 0, 0};
Time_t split2 = {0, 0, 0, 0};

void exercise_1_4(void)
{
    initJoystick();
    initLed();
    initTimer2_100Hz();

    __enable_irq();

    printf("\r\n--- Event-Driven Stopwatch Ready ---\r\n");
    printf("Controls: Center = Start/Stop | Left = Split 1 | Right = Split 2 | Down = Reset\r\n");

    while (1)
    {
        __WFI(); // Sleep until 100Hz Timer interrupt wakes the CPU

        // 1. Output time every time the second variable changes
        if (second_changed_flag)
        {
            second_changed_flag = 0;
            Time_t now = GetTimeAtomic(&clock_time);
            // printf("TIME: %02u:%02u:%02u\r\n", now.hours, now.minutes, now.seconds);
            char buffer[32];

            sprintf(buffer, "Time: %02u:%02u:%02u.%02u\r\n",
            		now.hours,
					now.minutes,
                    now.seconds,
                    now.hundredths);
            uart_send_string(buffer);
        }

        // 2. Process input event when joystick state changes
        if (joystick_flag)
        {
            joystick_flag = 0; // Clear flag
            uint8_t joystickState = current_joystick_state;

            // CENTER: Start / Stop
            if (joystickState & JOYSTICK_CENTER)
            {
                clock_running = !clock_running;
                Time_t now = GetTimeAtomic(&clock_time);
                if (clock_running)
                    printf("[STARTED] %02u:%02u:%02u.%02u\r\n", now.hours, now.minutes, now.seconds, now.hundredths);
                else
                    printf("[STOPPED] %02u:%02u:%02u.%02u\r\n", now.hours, now.minutes, now.seconds, now.hundredths);
            }
            // LEFT: Split 1
            else if (joystickState & JOYSTICK_LEFT)
            {
                split1 = GetTimeAtomic(&clock_time);
                printf("[SPLIT 1] %02u:%02u:%02u.%02u\r\n", split1.hours, split1.minutes, split1.seconds, split1.hundredths);
            }
            // RIGHT: Split 2
            else if (joystickState & JOYSTICK_RIGHT)
            {
                split2 = GetTimeAtomic(&clock_time);
                printf("[SPLIT 2] %02u:%02u:%02u.%02u\r\n", split2.hours, split2.minutes, split2.seconds, split2.hundredths);
            }
            // DOWN: Stop and Reset to 00:00:00.00
            else if (joystickState & JOYSTICK_DOWN)
            {
                clock_running = 0;

                __disable_irq();
                clock_time.hours = 0;
                clock_time.minutes = 0;
                clock_time.seconds = 0;
                clock_time.hundredths = 0;
                __enable_irq();

                printf("[RESET] 00:00:00.00\r\n");
            }
        }
    }
}

