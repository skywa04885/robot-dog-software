#include "ssfp.h"

/* External Usage */ const uint8_t SSFP_START[3] = { 0x98, 0x98, 0x98 };
/* External Usage */ const uint8_t SSFP_BROADCAST[3] = { 0x88, 0x88, 0x88 };

const uint16_t SSFP_START_SIZE = sizeof (SSFP_START) / sizeof (uint8_t);

void ssfp_frame_init(ssfp_frame_t *frame, uint8_t *saadr, uint8_t *daadr, uint16_t port) {
    // Copies the SAADR and DAARD values into the frame, and sets the port
    //  to the one specified in the arguments.
    memcpy((void *) &frame->hdr.saadr, (const void *) saadr, 3);
    memcpy((void *) &frame->hdr.daadr, (const void *) daadr, 3);
    frame->hdr.port = port;
}

void ssfp_frame_sign(ssfp_frame_t *frame) {
    // Clears the existing checksum to 0x00, to prevent undefined behaviour.
    frame->hdr.cs = 0x00;

    // Loops over all the bytes in the frame, these will include the header and body
    //  we will XOR all the bytes to the result.
    for (uint16_t i = 0; i < frame->hdr.t_len; ++i) {
        frame->hdr.cs ^= ((uint8_t *) frame)[i];
    }
}
