/*
    Simple Serial Framed Protocol (SSFP)
*/

#include <stdint.h>
#include <string.h>

#pragma once

#define BSWAP16(A) (((A & 0xFF00) >> 8) | ((A & 0x00FF) << 8))

typedef struct __attribute__ (( packed )) {
    /* 2 2  */ uint16_t    t_len;      /* Total Packet Length */
    /* 3 5  */ uint8_t     saadr[3];   /* The Packet Source Address */
    /* 3 8  */ uint8_t     daadr[3];   /* The Packet Destination Address */
    /* 1 9  */ uint8_t     cs;         /* Packet Checksum (XOR result) */
    /* 1 10 */ uint8_t     f;          /* The Packet Flags */
    /* 2 12 */ uint16_t    port;       /* Port */
} ssfp_hdr_t;

typedef struct __attribute__ (( packed )) {
    ssfp_hdr_t  hdr;        /* SSFP Header */
    uint8_t     pl[0];      /* Payload */
} ssfp_frame_t;

void ssfp_frame_init(ssfp_frame_t *frame, const uint8_t *saadr, const uint8_t *daadr, uint16_t port);
void ssfp_frame_sign(ssfp_frame_t *frame);
