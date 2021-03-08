/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#include "main.h"
#include "sysclk.h"
#include "servo.h"
#include "delay.h"

servo_group_t servo_group_mg996r = {
  /* Hardware Configuration */
  .tim = TIM4,
  /* Motor specifications */
  .degrees = 180.f,
  /* 0 - 180 Degrees */
  .min = MG996R_MIN, .max = MG996R_MAX
};

servo_group_t servo_group_hs805mg = {
  /* Hardware Configuration */
  .tim = TIM5,
  /* Motor specifications */
  .degrees = 180.f,
  /* 0 - 180 Degrees */
  .min = HS805MG_MIN, .max = HS805MG_MAX
};

void servos_init() {
  //
  // Initializes MG996R servo's
  //
  
  // Enables the timer
  RCC->APB1LENR |= RCC_APB1LENR_TIM4EN;

  // Makes the pins AF.
  GPIOB->MODER &= ~(0x3 << (6 * 2) | 0x3 << (7 * 2));
  GPIOB->MODER |= (0x2 << (6 * 2) | 0x2 << (7 * 2));

  // Selects AF2 in order to create a pathway to TIM4_CH1 and TIM4_CH2.
  GPIOB->AFR[0] |= ((2 << (6 * 4)) | (2 << (7 * 4)));

  // Performs the servo initialization.
  servo_group_init(&servo_group_mg996r);

  //
  // Initializes HS805MG servo's
  //

  // Enables the timer
  RCC->APB1LENR |= RCC_APB1LENR_TIM5EN;

  // Makes the pins AF
  GPIOA->MODER &= ~(0x3 << (0 * 2) | 0x3 << (1 * 2));
  GPIOA->MODER |= (0x2 << (0 * 2) | 0x2 << (1 * 2));

  // Selects AF2 in order to create a pathway to TIM5_CH1.
  GPIOA->AFR[0] |= ((2 << (0 * 4)) | (2 << (1 * 4)));

  servo_group_init(&servo_group_hs805mg);
}

int32_t main(void) {
  sysclk_init();
  delay_init();

  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN | RCC_AHB4ENR_GPIOBEN;

  servos_init();

  for (;;) {
    servo_move(&servo_group_mg996r, 0, 0.f);
    servo_move(&servo_group_hs805mg, 0, 180.f);
    servo_move(&servo_group_hs805mg, 1, 60.f);
    delay_s(2);
    servo_move(&servo_group_mg996r, 0, 180.f);
    servo_move(&servo_group_hs805mg, 0, 0.f);
    servo_move(&servo_group_hs805mg, 1, 180.f);
    delay_s(2);
  }
}