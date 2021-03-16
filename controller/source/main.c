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

#define REG_STATUS_RDY (1 << 0);

typedef struct __attribute__ (( "packed" )) {
  uint8_t _reg_status;
  uint8_t _reg_motorctl;
  float _reg_servo_a1_tpos;
  float _reg_servo_a2_tpos;
  float _reg_servo_a3_tpos;
  float _reg_servo_b1_tpos;
  float _reg_servo_b2_tpos;
  float _reg_servo_b3_tpos;
  float _reg_servo_c1_tpos;
  float _reg_servo_c2_tpos;
  float _reg_servo_c3_tpos;
} registers_t;

volatile registers_t __attribute__ (( section("bss") )) registers;

void servos_init (void) {
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

#define SPI_CMD_RSET 0x00 /* Software Reset */
#define SPI_CMD_RDCR 0x01 /* Read CR */
#define SPI_CMD_WRCR 0x02 /* Write CR */
#define SPI_CMD_BSCR 0x03 /* Is bit set CR */
#define SPI_CMD_BCCR 0x04 /* Is bit clear CR */
#define SPI_CMD_SBCR 0x05 /* Set bit CR */
#define SPI_CMD_CBCR 0x06 /* Clear bit CR */

void SPI1_IRQHandler (void) {
  while (SPI1->SR & SPI1_SR_RXP) {
    uint8_t data = *((volatile uint8_t *) &SPI1->RXDR);
  }
}

/**
 * Initializes the SPI hardware in slave mode.
 */
void spi_slave_init (void) {
  SPI1->CR1 = SPI_CR1_SPE;
  SPI1->IER = SPI_IER_RXPIE;
}

int32_t main (void) {
  // Initializes the CPU clock, to 480 Mhz, and configures one of the timers
  //  to be used for delay.
  sysclk_init();
  delay_init();

  // Enables the required peripheral clocks.
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN | RCC_AHB4ENR_GPIOBEN;

  // Performs some clock-required peripheral initialization.
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
