#include <stdio.h>
#include <time.h>
#include <gtk/gtk.h>

#include "inc/control.h"
#include "inc/ssfp_usart.h"
#include "../../shared/include/ssfp_management.h"
#include "../../shared/include/ssfp_ports.h"

ssfp_usart_t usart = {
	.speed = 230400,
	.master_address = { 0x4, 0x3, 0x2 },
	.slave_address = { 0x2, 0x3, 0x4 },
	.path = "/dev/ttyUSB0"
};

void activate (GtkApplication *app, gpointer udata) {

}

void send_greeting_frame () {
	// Allocates memory for the frame.
	ssfp_frame_t *frame = ssfp_usart_prepare_frame (&usart, sizeof (ssfp_mgmt_t), SSFP_MANAGEMENT_PORT);

	printf("Frame Size: %d\n", frame->hdr.t_len);

	// Sets the greeting opcode for the frame.
	ssfp_mgmt_t *mgmt = (ssfp_mgmt_t *) frame->pl;
	mgmt->op = SSFP_MGMT_GREET;

	// Calculates the frame checksum, and writes it to the
	//  slave device.
	ssfp_frame_sign (frame);
	ssfp_usart_write (&usart, frame);

	// Frees the frame allocated memory.
	free (frame);
}

void send_devid_frame () {
	// Allocates memory for the frame.
	ssfp_frame_t *frame = ssfp_usart_prepare_frame (&usart, sizeof (ssfp_mgmt_t) + sizeof (ssfp_mgmt_devi_t), SSFP_MANAGEMENT_PORT);

	// Sets the device id opcode for the frame.
	ssfp_mgmt_t *mgmt = (ssfp_mgmt_t *) frame->pl;
	mgmt->op = SSFP_MGMT_DEV_INFO_REQUEST;

	// Calculates the frame checksum, and writes it to the
	//  slave device.
	ssfp_frame_sign (frame);
	ssfp_usart_write (&usart, frame);

	// Frees the frame allocated memory.
	free (frame);
}

int packets = 0;

void read_devid_frame () {
	ssfp_frame_t *frame = NULL;

	if ((frame = ssfp_usart_read(&usart)) == NULL) {
		printf("Failure!\n");
		return;
	}

	ssfp_mgmt_t *mgmt_frame = (ssfp_mgmt_t *) frame->pl;
	ssfp_mgmt_devi_t *devid_frame = (ssfp_mgmt_devi_t *) mgmt_frame->p;

	fprintf(stdout, "Device Firmware Version -> %u:%u:%u\n", devid_frame->major, devid_frame->minor, devid_frame->patch);
	fprintf(stdout, "Vendor String: %s\n", devid_frame->vendor);
	printf("packet: %d\n", ++packets);
}

int main(int argc, char **argv) {
	ssfp_usart_init(&usart);
	send_greeting_frame();

	for (;;) {
		send_devid_frame();
		read_devid_frame();
	}


    GtkApplication *app = gtk_application_new ("nl.fannst.robot_dog.gui", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

    g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    ssfp_usart_close(&usart);

    return 0;
}
