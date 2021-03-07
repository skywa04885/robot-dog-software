#include "servo.h"

/**
 * Initializes a group of servos.
 */
void servo_group_init(servo_group_t *group) {
  TIM_TypeDef *TIM = group->tim;

  // Configures the prescalar, auto-reload register and the capture compare register.
  TIM->CNT = 0;               // Start counting at zero.
  TIM->PSC = SERVO_TIM_PSC;
  TIM->ARR = SERVO_TIM_ARR;
  TIM->CCR1 = TIM->CCR2 = TIM->CCR3 = TIM->CCR4 = (uint16_t) ((((float) SERVO_TIM_ARR) / 20.f) * group->min);

  // OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register. It
  // can be programmed as active high or active low. OCx output is enabled by the CCxE bit in
  // the TIMx_CCER register. Refer to the TIMx_CCERx register description for more details.

  // Enables the Capture/Compare thingy, with active-high.
  TIM->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;

  // The PWM mode can be selected independently on each channel (one PWM per OCx
  // output) by writing 110 (PWM mode 1) or ‘111 (PWM mode 2) in the OCxM bits in the
  // TIMx_CCMRx register. The corresponding preload register must be enabled by setting the
  // OCxPE bit in the TIMx_CCMRx register, and eventually the auto-reload preload register (in
  // upcounting or center-aligned modes) by setting the ARPE bit in the TIMx_CR1 register.

  // Sets the PWM mode to 1 (0b110), and enables the auto-reload preload register.
  TIM->CCMR1 |= ((0b110 << TIM_CCMR1_OC1M_Pos)
    | (0b110 << TIM_CCMR1_OC2M_Pos)
    | TIM_CCMR1_OC1PE
    | TIM_CCMR1_OC2PE);

  TIM->CCMR2 |= ((0b110 << TIM_CCMR2_OC3M_Pos)
    | (0b110 << TIM_CCMR2_OC4M_Pos)
    | TIM_CCMR2_OC3PE
    | TIM_CCMR2_OC4PE);

  // Sets the ARPE bit in the control register.
  TIM->CR1 |= TIM_CR1_ARPE;

  // As the preload registers are transferred to the shadow registers only when an update event
  // occurs, before starting the counter, all registers must be initialized by setting the UG bit in
  // the TIMx_EGR register.

  // Generate update event.
  TIM->EGR |= TIM_EGR_UG;

  // Enables the timer
  TIM->CR1 |= TIM_CR1_CEN;
}

/**
 * Moves the servo to the given number of degrees.
 */
void servo_move(servo_group_t *group, uint8_t n, float deg) {
  float v = group->min + ((group->max - group->min) * (deg / group->degrees));
  (&group->tim->CCR1)[n] = (uint32_t) ((((float) SERVO_TIM_ARR) / 20.f) * v);
}
