/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#include "stm32h750xx.h"

#include "delay.h"

#pragma once

typedef struct {
    GPIO_TypeDef   *gpio;   /* The GPIO port */
    uint8_t         pin;    /* The Pin */
} buzzer_t;

void buzzer_init(const buzzer_t *buzzer);
void buzzer_buzz(const buzzer_t *buzzer, uint16_t pulses, 
    uint16_t delay);
