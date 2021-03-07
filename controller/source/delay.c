/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#include "delay.h"

void delay_init(void) {
  RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;

  TIM8->PSC = 120;
}

void delay_us(uint16_t us) {
  TIM8->CNT = 0;
  TIM8->ARR = us;
  TIM8->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;
  while (TIM8->CR1 & TIM_CR1_CEN);
}

void delay_ms(uint16_t ms) {
  for (uint16_t i = 0; i < ms; ++i) {
    delay_us(1000);
  }
}

void delay_s(uint16_t s) {
  for (uint16_t i = 0; i < s; ++i) {
    delay_ms(1000);
  }
}