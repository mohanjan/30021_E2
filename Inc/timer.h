#ifndef TIMER_H_
#define TIMER_H_
#include "stm32f30x.h"
#include <stdio.h>
#include "stm32f30x_tim.h"

// Time Structure
typedef struct {
    volatile uint8_t hours;
    volatile uint8_t minutes;
    volatile uint8_t seconds;
    volatile uint8_t hundredths;
} Time_t;

// Interrupt handler variables
extern volatile uint32_t ICValue1;
extern volatile uint32_t ICValue2;
extern volatile uint8_t ICValid;

Time_t GetTimeAtomic(volatile Time_t *src);
void initTimer2_100Hz(void);
void TIM2_IRQHandler(void);
void TIM2_PWM_init();
void TIM2_init_50Hz();
void TIM16_init_50Hz();
void TIM16_init_10kHz(void);
void TIM16_PWM_init(void);
void GPIO_set_AF1_PA6(void);
void GPIO_set_AF1_PB11(void);
void GPIO_set_AF_PA5(void);
void TIM2_PWMInput_Init(void);

#endif /* TIMER_H_ */
