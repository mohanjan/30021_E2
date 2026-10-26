#include "timer.h"
#include "stm32f30x.h"

#define TIM_TICK_US  1.0f    // (PSC + 1) / 64 MHz = 1 micro second

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
	        // Fall back in case distance measured is greater than 400cm
	        if (distance > 400.0f) {
	        	printf("Distance < 400 cm, Width of pulse < 23200 us\r");
	        }
	        else {
	        	printf("Distance = %.2f cm, Width of pulse = %.2f us\r",distance,pulse_width);
	        }


	    }
	}
}
