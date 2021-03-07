#pragma once

#include "stm32h750xx.h"

#define SERVO_TIM_PSC 2000      /* 120,000,000Hz / 2,000 = 60,000Hz */
#define SERVO_TIM_ARR 1200      /* 60,000Hz / 1,200 = 50Hz */

typedef struct {
  TIM_TypeDef *tim;
  float degrees;
  float min, max;
} servo_group_t;

/**
 * Initializes a group of servos.
 */
void servo_group_init(servo_group_t *group);

/**
 * Moves the servo to the given number of degrees.
 */
void servo_move(servo_group_t *group, uint8_t n, float deg);