/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#include "main.h"
#include "sysclk.h"
#include "servo.h"
#include "delay.h"
#include "buzzer.h"

#include "../../shared/include/ssfp.h"
#include "../../shared/include/ssfp_servo.h"
#include "../../shared/include/ssfp_management.h"
#include "../../shared/include/ssfp_ports.h"
#include "ssfp_usart.h"

uint8_t write_buffer[1024];
uint8_t read_buffer[1024];

const uint8_t SLAVE_ADDRESS[3] = { 0x2, 0x3, 0x4 };

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

buzzer_t buzzer = {
    .gpio = GPIOA,
    .pin = 8
};

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

void setup (void) {
    // Initializes the CPU clock, to 480 Mhz, and configures one of the timers
    //  to be used for delay.
    sysclk_init ();
    delay_init ();

    // Enables the required peripheral clocks.
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN | RCC_AHB4ENR_GPIOBEN | RCC_AHB4ENR_GPIOEEN;

    // Initializes the USART hardware for the SSFP Protocol.
    ssfp_usart_init ();

    // Performs some clock-required peripheral initialization.
    servos_init ();

    // Initializes the buzzer.
    buzzer_init(&buzzer);
}

void handle_mgmt_frame(ssfp_frame_t *frame) {
    ssfp_mgmt_t *mgmt = (ssfp_mgmt_t *) frame->pl;

    switch (mgmt->op) {
        case SSFP_MGMT_GREET:
            buzzer_buzz(&buzzer, 200, 700);
            buzzer_buzz(&buzzer, 100, 520);
            break;
        case SSFP_MGMT_DEV_INFO_REQUEST: {
            ssfp_frame_t *response = (ssfp_frame_t *) write_buffer;

            // Initializes the frame, and assigns the management device info
            //  to the packet.
            ssfp_frame_init(response, SLAVE_ADDRESS, frame->hdr.saadr, SSFP_MANAGEMENT_PORT);
            ssfp_mgmt_devi_create(response);

            // Signs the response, and writes it over UART.
            ssfp_frame_sign(response);
            ssfp_usart_write(response);
            break;
        }
    }
}

int32_t main (void) {
    // Performs the startup code.
    setup ();

    // Performs the startup complete, chip ready buzz.
    buzzer_buzz(&buzzer, 300, 500);

    for (;;) {
        if (!ssfp_usart_available ()) continue;

        ssfp_usart_fifo_read (read_buffer);
        ssfp_frame_t *frame = (ssfp_frame_t *) read_buffer;

        switch (frame->hdr.port) {
            case SSFP_MANAGEMENT_PORT:
                handle_mgmt_frame (frame);
                break;
            case SSFP_SERVO_CTRL_PORT:
                break;
        }

        // servo_move(&servo_group_mg996r, 0, 0.f);
        // servo_move(&servo_group_hs805mg, 0, 180.f);
        // servo_move(&servo_group_hs805mg, 1, 60.f);
        // delay_s(2);
        // servo_move(&servo_group_mg996r, 0, 180.f);
        // servo_move(&servo_group_hs805mg, 0, 0.f);
        // servo_move(&servo_group_hs805mg, 1, 180.f);
        // delay_s(2);
    }
}
