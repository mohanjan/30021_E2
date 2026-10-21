#include "timer.h"
#include "stm32f30x.h"

#define TIM_TICK_US  1.0f    // (PSC + 1) / 64 MHz = 1 micro second

volatile uint32_t ICValue1, ICValue2; // Initialize variables to store last readings of the CCR1 and CCR2
volatile uint8_t  ICValid = 0; // Flag to see if new reading is available

void TIM2_IRQHandler(void) {
	// ISR = Interrupt Service Routine.
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		ICValue1 = TIM_GetCapture1(TIM2); // Period from CCR1
		ICValue2 = TIM_GetCapture2(TIM2); // Width from CCR2
		ICValid = 1;
    }
}

// Code for calculating running average
#define AVG_N 10
static uint32_t pbuf[AVG_N], hbuf[AVG_N];
static uint64_t psum = 0, hsum = 0;
static uint8_t  avg_idx = 0, avg_count = 0;

// Add one measurement (period and high time, in ticks)
void avg_add(uint32_t p, uint32_t h)
{
    psum -= pbuf[avg_idx];          // remove the oldest sample
    hsum -= hbuf[avg_idx];
    pbuf[avg_idx] = p;              // store the new one
    hbuf[avg_idx] = h;
    psum += p;
    hsum += h;
    avg_idx = (avg_idx + 1) % AVG_N;
    if (avg_count < AVG_N) avg_count++;
}

void avg_reset(void)
// Resets all variables to zero
{
    for (uint8_t i = 0; i < AVG_N; i++) {
        pbuf[i] = 0;
        hbuf[i] = 0;
    }
    psum = 0;
    hsum = 0;
    avg_idx = 0;
    avg_count = 0;
}

// Averages in ticks; returns 0 if there are no samples yet
float avg_period_ticks(void) { return avg_count ? (float)psum / avg_count : 0.0f; }
float avg_high_ticks(void)   { return avg_count ? (float)hsum / avg_count : 0.0f; }



void exercise_6_2(void) {
	TIM2_PWMInput_Init();
	__enable_irq();

	float pulse_width, distance;
	printf("\r\n--- running ---\r\n");

	while (1) {
		/*To activate the sensor a HIGH pulse of at least 10μs must be put on Trig.
		Once the trigger pulse goes back to LOW 8 sonic pulses at 40 kHz are transmitted from the sensor and the Echo pin goes to HIGH
		and stays like this until the reflection of the 8 sonic pulses is detected. To determine the distance the time that the
		Echo pin is HIGH must be measured.

		Trigger of wifth minimi  10 mus, trigger max rep rate 50 mus (GPIO)

		Echo detect pulse width in muS --> conert to range (cm) by muS / 58 (GPIO) */


	    if (ICValid) {
	        NVIC_DisableIRQ(TIM2_IRQn);
	        pulse_width = ICValue2 + 1;
	        ICValid = 0;
	        NVIC_EnableIRQ(TIM2_IRQn);

	        distance = pulse_width/58;
	        // distance[cm] = (Speed of sounds * pulse_width[µs] * 0.001) / 2
	        distance = (343 * pulse_width * 0.001) / 2;

	        printf("Distance = %.2f cm, Width of pulse = %.2f µs\r",distance,pulse_width);
	}
}

