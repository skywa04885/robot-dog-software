/*
    Simple Serial Framed Protocol (SSFP)
*/

#include <stdint.h>
#include <string.h>

#pragma once

typedef struct __attribute__ (( packed )) {
    uint16_t    t_len;      /* Total Packet Length */
    uint8_t     saadr[3];   /* The Packet Source Address */
    uint8_t     daadr[3];   /* The Packet Destination Address */
    uint8_t     cs;         /* Packet Checksum (XOR result) */
    uint8_t     f;          /* The Packet Flags */
    uint16_t    port;       /* Port */
} ssfp_hdr_t;

typedef struct __attribute__ (( packed )) {
    ssfp_hdr_t  hdr;        /* SSFP Header */
    uint8_t     pl[0];      /* Payload */
} ssfp_frame_t;

void ssfp_frame_init(ssfp_frame_t *frame, uint8_t *saadr, uint8_t *daadr, uint16_t port);
void ssfp_frame_sign(ssfp_frame_t *frame);
