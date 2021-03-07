/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#pragma once

#include "stm32h750xx.h"

void delay_init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void delay_s(uint16_t s);