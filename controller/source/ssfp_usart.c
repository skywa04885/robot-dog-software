#include "ssfp_usart.h"

extern const uint8_t SSFP_START[3];
extern const uint8_t SSFP_BROADCAST[3];

volatile uint8_t ssfp_usart_fifo_buffer[8 * 1024];
volatile ssfp_usart_fifo_t fifo = {
    .buffer = ssfp_usart_fifo_buffer,
    .size = sizeof (ssfp_usart_fifo_buffer) / sizeof (uint8_t),
    .write = 0,
    .read = 0
};

void __ssfp_usart_fifo_write_byte(uint8_t byte) {
    fifo.buffer[fifo.write] = byte;
    fifo.write = (fifo.write + 1) % fifo.size;
}

bool __ssfp_usart_fifo_write_bytes(const uint8_t *bytes, uint16_t size) {
    uint16_t fake_write = fifo.write;
    for (uint16_t i = 0; i < size; ++i) {
        fake_write = (fake_write + 1) % fifo.size;
        if (fake_write == fifo.read) {
            return false;
        }
    }

    for (uint16_t i = 0; i < size; ++i) {
        __ssfp_usart_fifo_write_byte(bytes[i]);
    }

    return true;
}

void USART1_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_RXNE_RXFNE) {
        const uint8_t byte = *((uint8_t *) &USART1->RDR);
    }
}

void __ssfp_usart_write_byte(uint8_t byte) {
    *((uint8_t *) &USART1->TDR) = byte;
    while (!(USART1->ISR & USART_ISR_TC));
}

void __ssfp_usart_write_bytes(const uint8_t *bytes, uint16_t size) {
    for (uint16_t i = 0; i < size; ++i) {
        __ssfp_usart_write_byte(bytes[i]);
    }
}

void ssfp_usart_init(void) {
    // Enables the USART1 in the Reset Clock Control (RCC).
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Calculates and sets the BAUD rate.
    USART1->BRR = (((2 * SSFP_USART_BUS_CLK) + SSFP_USART_BAUD) / (2 * SSFP_USART_BAUD));

    // Enables the USART1 Peripheral.
    USART1->CR1 = USART_CR1_TE 
        | USART_CR1_RE
        | USART_CR1_UE
        | USART_CR1_RXNEIE_RXFNEIE;
    
    // Enables the USART1 in the NVIC.
    NVIC_EnableIRQ(USART1_IRQn);
}

void ssfp_usart_write(const ssfp_frame_t *frame) {
    // Writes the start sequence for a new frame.
    __ssfp_usart_write_bytes(SSFP_START, sizeof (SSFP_START) / sizeof (uint8_t));

    // Writes the SSFP frame itself, since the start sequence has been sent
    //  we're ready to send the payload.
    __ssfp_usart_write_bytes((const uint8_t *) frame, frame->hdr.t_len);
}
