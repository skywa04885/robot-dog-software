#include <stdint.h>
#include <string.h>

#include "ssfp.h"

#pragma once

//
// Firmware version
//

#ifndef FIRMWARE_MAJOR
#define FIRMWARE_MAJOR 0
#endif

#ifndef FIRMWARE_MINOR
#define FIRMWARE_MINOR 0
#endif

#ifndef FIRMWARE_PATCH
#define FIRMWARE_PATCH 1
#endif

#ifndef FIRMWARE_VENDOR
#define FIRMWARE_VENDOR "Official Firmware, by Luke Rieff"
#endif

//
// Data Structures
//

typedef enum {
    SSFP_MGMT_GREET = 0,
    SSFP_MGMT_DEV_INFO_REQUEST = 1,
    SSFP_MGMT_DEV_INFO_RESPONSE = 2
} ssfp_mgmt_op_t;

typedef struct __attribute__ (( packed )) {
    uint8_t     op;         /* Management Frame Opcode */
    uint8_t     p[0];       /* Management Frame Payload */
} ssfp_mgmt_t;

typedef struct __attribute__ (( packed )) {
    uint8_t     flags;      /* Device Flags */
    uint8_t     major;      /* Major Version */
    uint8_t     minor;      /* Minor Version */
    uint8_t     patch;      /* Patch Version */
    char        vendor[0];  /* The Vendor String */
} ssfp_mgmt_devi_t;

#ifdef CONTROLLER_COMPILE

void ssfp_mgmt_devi_create (ssfp_frame_t *frame);

#endif