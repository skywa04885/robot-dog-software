/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#include "buzzer.h"

void buzzer_init(const buzzer_t *buzzer) {
	buzzer->gpio->MODER &= ~(0x3 << (2 * buzzer->pin));
	buzzer->gpio->MODER |= (0x1 << (2 * buzzer->pin));

	buzzer->gpio->ODR &= ~(0x1 << buzzer->pin);
}

void buzzer_buzz(const buzzer_t *buzzer, uint16_t pulses, 
    uint16_t delay) {
	for (uint16_t i = 0; i < pulses; ++i) {
		buzzer->gpio->ODR |= (0x1 << buzzer->pin);
		delay_us(delay);
		buzzer->gpio->ODR &= ~(0x1 << buzzer->pin);
		delay_us(delay);
	}
}