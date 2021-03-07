/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#include "main.h"
#include "sysclk.h"
#include "servo.h"
#include "delay.h"

servo_group_t servo_group_1 = {
  /* Hardware Configuration */
  .tim = TIM4,
  /* Motor specifications */
  .degrees = 180.f,
  /* 0 - 180 Degrees */
  .min = .553f, .max = 2.400f
};

void servos_init() {
  //
  // Initializes Servo's for LEG-1
  //
  
  // >> Initializes the top servo.

  // Enables the timer
  RCC->APB1LENR |= RCC_APB1LENR_TIM4EN;

  // Makes the pins AF.
  GPIOB->MODER &= ~(0x3 << (6 * 2) | 0x3 << (7 * 2));
  GPIOB->MODER |= (0x2 << (6 * 2) | 0x2 << (7 * 2));

  // Selects AF2 in order to create a pathway to TIM4_CH1.
  GPIOB->AFR[0] |= ((2 << (6 * 4)) | (2 << (7 * 4)));

  // Performs the servo initialization.
  servo_group_init(&servo_group_1);
}

int main(void) {
  sysclk_init();
  delay_init();

  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN | RCC_AHB4ENR_GPIOBEN;

  servos_init();

  for (;;) {
    servo_move(&servo_group_1, 0, 0.f);
    servo_move(&servo_group_1, 1, 180.f);
    delay_s(2);
    servo_move(&servo_group_1, 0, 180.f);
    servo_move(&servo_group_1, 1, 0.f);
    delay_s(2);
  }
}