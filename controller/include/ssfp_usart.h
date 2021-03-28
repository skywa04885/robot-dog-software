#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#include "stm32h750xx.h"

#include "../../shared/include/ssfp.h"

#pragma once

#define SSFP_USART_BAUD     (230400)
#define SSFP_USART_BUS_CLK  (120 * 1000000)

typedef struct {
    uint8_t    *buffer;     /* The Memory Buffer */
    int32_t     capacity,   /* The Memory Buffer Size */
                size,       /* The Size of the buffer */
                write,      /* Write Pointer */
                read;       /* Read Pointer */
} ssfp_usart_fifo_t;

typedef enum {
    FREE = 0,               /* In Free State (no frame, nor control bytes) */
    FRAME = 1,              /* In Frame State (receiving frame) */
    FRAME_LEN_EXPECT = 2    /* In Possible Control Bytes Detected (START) */
} ssfp_usart_statename_t;

typedef struct {
    ssfp_usart_statename_t  sn;         /* State Name */
    uint8_t                 *b;         /* The Buffer */
    uint16_t                 btotal,     /* The Buffer Size */
                            bused,      /* The Bytes Used in buffer */
                            fl;         /* Frame Length */
} ssfp_usart_state_t;

void __ssfp_usart_fifo_write_byte (uint8_t byte);
bool __ssfp_usart_fifo_write_bytes (const uint8_t *bytes, uint16_t size);

void __ssfp_usart_write_byte (uint8_t byte);
void __ssfp_usart_write_bytes (const uint8_t *bytes, uint16_t size);

uint8_t __ssfp_usart_fifo_read_byte (void);

bool ssfp_usart_available (void);

void ssfp_usart_init (void);
void ssfp_usart_write (const ssfp_frame_t *frame);

void ssfp_usart_fifo_read (uint8_t *buffer);
