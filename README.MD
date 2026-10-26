# Group 4 - Digital Instrumentation

This repository contains embedded programming exercises for the STM32F302 board. This README summarizes the hardware pin assignments used by the exercises, including the MBED Application Shield, ADC and PWM connections, the LSM9DS1 sensor over SPI, OpenLog over USART1, and the signal input used in Assignment 6.

## MBED Application Shield Pinout (Used in Exercise 1, 2, 3, 6)
| Peripheral | Function | Arduino Pin | STM32 Pin |
|------------|----------|-------------|------------|
| **128x32 Graphics LCD (SPI)** | LCD_MOSI | D11 | PB_15 |
| | LCD_RESET | D12 | PB_14 |
| | LCD_SCK | D13 | PB_13 |
| | LCD_A0 | D7 | PA_8 |
| | LCD_CD_N | D10 | PB_6 |
| **3-Axis ±1.5g Accelerometer (MMA7660, Addr 0x98)** | SCL | SCL | PB_8 |
| | SDA | SDA | PB_9 |
| **Temperature Sensor (LM75B, Addr 0x90)** | SCL | SCL | PB_8 |
| | SDA | SDA | PB_9 |
| **5-Way Joystick (ALPS SKRHADE010)** | Down | A3 | PB_0 |
| | Left | A4 | PC_1 |
| | Centre | D4 | PB_5 |
| | Up | A2 | PA_4 |
| | Right | A5 | PC_0 |
| **Potentiometer 1 (left)** | 10 kΩ | A0 | PA_0 |
| **Potentiometer 2 (right)** | 10 kΩ | A1 | PA_1 |
| **RGB LED (Cree CLV1A-FKB)** | Red | D5 | PB_4 |
| | Green | D9 | PC_7 |
| | Blue | D8 | PA_9 |
| **Speaker (MULTICOMP MCSMT-8030B-3717) (unused)** | PWM Output | D6 | PB_10 |
| **XBee Socket (unused)** | Rx | D1 | PA_2 |
| | Tx | D0 | PA_3 |
| | nReset | D3 | PB_3 |
| | Status | D2 | PA_10 |

## Assignment 3 - ADC and PWM
| Function      | STM32F302 pin |
| :------------:| ------------: |
| ADC1          | PA0           |
| ADC2          | PA1           |
| PWM Out       | PA6           |
| PWM Out       | PB11          |

## Assignment 4
**connections (using SPI2)**
| LSM9DS1             | STM32F302   | STM32F302 pin |
| :------------------ | :---------: | ------------: |
| GND                 | GND         | GND           |
| VDD                 | 3v3         | 3v3           |
| SDA	              | MOSI / D11  | PB15          |
| SCL                 | SCK / D13   | PB13          |
| SDO M + SDO AG      | MISO / D12  | PB14          |
| CS AG               | CS / D10    | PB6           |
| CS M                | D6          | PB10          |

## Assignment 5 - OpenLog USART1
| Function      | STM32F302 pin |
| :------------ | ------------: |
| USART1 TX (to OpenLog RX) | PA9  |
| USART1 RX (from OpenLog TX) | PA10 |
| USART1 RTS (not used; hardware flow control disabled) | PA12 |

## Assignment 6
* PA5 used for reading signal input from either Tektronix AFG2021 function generator or by HC-SR04 ultrasonic distance sensor (Echo pin)
