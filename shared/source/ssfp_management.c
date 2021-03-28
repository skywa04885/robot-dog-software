#include "ssfp_management.h"

#ifdef CONTROLLER_COMPILE

const char *FIRMWARE_VENDOR_VAR = FIRMWARE_VENDOR;

void ssfp_mgmt_devi_create (ssfp_frame_t *frame) {
	ssfp_mgmt_t *mgmt_frame = (ssfp_mgmt_t *) frame->pl;
	mgmt_frame->op = SSFP_MGMT_DEV_INFO_RESPONSE;

	ssfp_mgmt_devi_t *mgmt_devi_b = (ssfp_mgmt_devi_t *) mgmt_frame->p;
	mgmt_devi_b->major = FIRMWARE_MAJOR;
	mgmt_devi_b->minor = FIRMWARE_MINOR;
	mgmt_devi_b->patch = FIRMWARE_PATCH;

	uint16_t vendor_string_len = strlen (FIRMWARE_VENDOR_VAR) + 1;
	memcpy ((void *) mgmt_devi_b->vendor, (const void *) FIRMWARE_VENDOR_VAR, vendor_string_len);

	frame->hdr.t_len = sizeof (ssfp_frame_t) + sizeof (ssfp_mgmt_t) + sizeof (ssfp_mgmt_devi_t) + vendor_string_len;
}

#endif