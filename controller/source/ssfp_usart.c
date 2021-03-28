#include "ssfp_usart.h"

extern const uint8_t SSFP_START[3];
extern const uint8_t SSFP_BROADCAST[3];

uint8_t ssfp_usart_fifo_buffer[2 * 1024];
ssfp_usart_fifo_t fifo = {
    .buffer = ssfp_usart_fifo_buffer,
    .size = 0,
    .capacity = sizeof (ssfp_usart_fifo_buffer) / sizeof (uint8_t),
    .write = -1,
    .read = -1,
};

void __ssfp_usart_fifo_write_byte (uint8_t byte) {
    if (fifo.write >= fifo.capacity) {
        return;
    }

    ++fifo.size;
    fifo.write = (fifo.write + 1) % fifo.capacity;
    fifo.buffer[fifo.write] = byte;
}

uint8_t __ssfp_usart_fifo_read_byte (void) {
    if (fifo.size <= 0) {
        return 0;
    }

    --fifo.size;
    fifo.read = (fifo.read + 1) % fifo.capacity;
    return fifo.buffer[fifo.read];
}

bool __ssfp_usart_fifo_write_bytes (const uint8_t *bytes, uint16_t size) {
    if (fifo.size + size > fifo.capacity) {
        return false;
    }

    for (uint16_t i = 0; i < size; ++i) {
        __ssfp_usart_fifo_write_byte (bytes[i]);
    }

    return true;
}

uint8_t __ssfp_usart_buffer[512];
ssfp_usart_state_t state = {
    .sn = FREE,
    .b = __ssfp_usart_buffer,
    .btotal = sizeof (__ssfp_usart_buffer) / sizeof (uint8_t),
    .bused = 0
};

void USART1_IRQHandler (void) {
    // Checks if the interru[t has been triggered by an
    //  RX FIFO not empty event.
    if (USART1->ISR & USART_ISR_RXNE_RXFNE) {
        GPIOE->ODR &= ~GPIO_ODR_OD0;

        // Reads the byte from the read data register.
        state.b[state.bused++] = *((uint8_t *) &USART1->RDR);

        // Checks the current state of transmission, and handles the data.
        switch (state.sn) {
            // >>
            //     Awaiting Start Sequence
            // >>
            case FREE: {
                // If current byte in buffer does not equal the specified byte
                //  from the start sequence, set used to 0, to listen for more.
                if (state.b[state.bused - 1] != SSFP_START[state.bused - 1]) {
                    state.bused = 0;
                }

                // If the number of bytes used in free mode equals 3, than we know
                //  that we received the start sequence, and we will wait for the
                //  frame length.
                if (state.bused == 3) {
                    state.sn = FRAME_LEN_EXPECT;
                    state.bused = 0;
                    break;
                }

                break;
            }
            // >>
            //     Awaiting Frame Length
            // >>
            case FRAME_LEN_EXPECT: {
                // Checks if we've received all 2 bytes of the frame length,
                //  if so set the state to frame, and start receiving ...
                if (state.bused == 2) {
                    state.fl = (((uint16_t) state.b[0]) <<  8) | ((uint16_t) state.b[1]);
                    
                    state.bused = 0;
                    state.sn = FRAME;
                }

                break;
            }
            // >>
            //     Fetching Frame
            // >>
            case FRAME: {
                // Checks if we've received the full number of bytes specified
                //  by the frame length, sent before the frame itself. If so
                //  put the received frame in the ring-buffer.
                if (state.bused >= state.fl) {
                    __ssfp_usart_fifo_write_bytes(state.b, state.bused);

                    state.bused = state.fl = 0;
                    state.sn = FREE;
                }

                break;
            }
        }

        GPIOE->ODR |= GPIO_ODR_OD0;
    }
}

void __ssfp_usart_write_byte (uint8_t byte) {
    GPIOE->ODR &= ~GPIO_ODR_OD1;
    
    *((uint8_t *) &USART1->TDR) = byte;
    while (!(USART1->ISR & USART_ISR_TC));

    GPIOE->ODR |= GPIO_ODR_OD1;
}

void __ssfp_usart_write_bytes (const uint8_t *bytes, uint16_t size) {
    for (uint16_t i = 0; i < size; ++i) {
        __ssfp_usart_write_byte (bytes[i]);
    }
}

void ssfp_usart_init (void) {
    //
    // Configures the GPIO pins for STATUS LEDs.
    //

    // Makes PA8 output.
    GPIOE->MODER &= ~((0x3 << GPIO_MODER_MODE0_Pos)
        | (0x3 << GPIO_MODER_MODE1_Pos));
    GPIOE->MODER |= ((0x1 << GPIO_MODER_MODE0_Pos)
        | (0x1 << GPIO_MODER_MODE1_Pos));

    // Default LOW
    GPIOE->ODR |= (GPIO_ODR_OD0
        | GPIO_ODR_OD1);

    //
    // Configures the GPIO pins for USART.
    //

    // Reset the GPIOA 9, 10 modes.
    GPIOA->MODER &= ~((0x3 << GPIO_MODER_MODE10_Pos)
        | (0x3 << GPIO_MODER_MODE9_Pos));

    // Makes GPIOA 9, 10 use the alternative function mode.
    GPIOA->MODER |= ((0x2 << GPIO_MODER_MODE10_Pos)
        | (0x2 << GPIO_MODER_MODE9_Pos));

    // Selects the alternative function 7 for both GPIOA pin 9 and 10.
    //  this will make them available for the USART hardware.
    GPIOA->AFR[1] |= ((7 << GPIO_AFRH_AFSEL10_Pos)
        | (7 << GPIO_AFRH_AFSEL9_Pos));

    //
    // Configures the USART hardware.
    //

    // Enables the USART1 in the Reset Clock Control (RCC).
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Calculates and sets the BAUD rate.
    USART1->BRR = (((2 * SSFP_USART_BUS_CLK) + SSFP_USART_BAUD) / (2 * SSFP_USART_BAUD));

    // Enables the USART1 in the NVIC.
    NVIC_EnableIRQ (USART1_IRQn);

    // Enables the USART1 Peripheral.
    USART1->CR1 = USART_CR1_TE 
        | USART_CR1_RE
        | USART_CR1_UE
        | USART_CR1_RXNEIE_RXFNEIE;
}

void ssfp_usart_write (const ssfp_frame_t *frame) {
    // Writes the start sequence for a new frame.
    __ssfp_usart_write_bytes (SSFP_START, sizeof (SSFP_START) / sizeof (uint8_t));

    // Writes the length of the new frame.
    __ssfp_usart_write_byte((uint8_t) ((frame->hdr.t_len & 0xFF00) >> 8));
    __ssfp_usart_write_byte((uint8_t) ((frame->hdr.t_len & 0x00FF) >> 0));

    // Writes the SSFP frame itself, since the start sequence has been sent
    //  we're ready to send the payload.
    __ssfp_usart_write_bytes ((const uint8_t *) frame, frame->hdr.t_len);
}

void ssfp_usart_fifo_read (uint8_t *buffer) {
    ssfp_frame_t *frame = (ssfp_frame_t *) &fifo.buffer[fifo.read];
    for (uint16_t i = 0; i < frame->hdr.t_len; ++i) {
        buffer[i] = __ssfp_usart_fifo_read_byte();
    }
}

bool ssfp_usart_available (void) {
    return fifo.size != 0;
}