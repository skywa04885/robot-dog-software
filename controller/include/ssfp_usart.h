#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#include "stm32h750xx.h"

#include "ssfp.h"

#pragma once

#define SSFP_USART_BAUD     (115200)
#define SSFP_USART_BUS_CLK  (120 * 1000000)

typedef struct __attribute__ (( packed )) {
    ssfp_frame_t    frame;  /* The Received Frame */
} ssfp_usart_fifo_frame_t;

typedef struct {
    uint8_t    *buffer;     /* The Memory Buffer */
    uint16_t    size,       /* The Memory Buffer Size */
                write,      /* Write Pointer */
                read;       /* Read Pointer */
} ssfp_usart_fifo_t;

typedef enum {
    FREE = 0,               /* In Free State (no frame, nor control bytes) */
    FRAME = 1,              /* In Frame State (receiving frame) */
    POSSIBLE_CONTROL = 2    /* In Possible Control Bytes Detected (START) */
} ssfp_usart_statename_t;

typedef struct {
    ssfp_usart_statename_t  sn;         /* State Name */
    uint8_t                 *b;         /* The Buffer */
    uint16_t                btotal,     /* The Buffer Size */
                            bused;      /* The Bytes Used in buffer */
} ssfp_usart_state_t;

void __ssfp_usart_fifo_write_byte(uint8_t byte);
bool __ssfp_usart_fifo_write_bytes(const uint8_t *bytes, uint16_t size);

void __ssfp_usart_write_byte(uint8_t byte);
void __ssfp_usart_write_bytes(const uint8_t *bytes, uint16_t size);

void ssfp_usart_init(void);
void ssfp_usart_write(const ssfp_frame_t *frame);

/**
 * Reads an SSFP Usart frame from the FIFO
 */
ssfp_usart_fifo_frame_t *ssfp_usart_fifo_read(void);

/**
 * Increments the read pointer to the end of current frame.
 */
void ssfp_usart_fifo_next(void);
