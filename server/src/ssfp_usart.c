#include "ssfp_usart.h"

extern const uint8_t SSFP_START[3];
extern const uint8_t SSFP_BROADCAST[3];

void ssfp_usart_init (ssfp_usart_t *usart) {
	//
	// Opens the FD
	//

	// Opens the FD.
	usart->fd = open (usart->path, (O_RDWR
		| O_NOCTTY
		| O_SYNC));

	// Checks if the FD is opened with success, if not throw
	//  runtime error with error string.
	if (usart->fd < 0) {
		fprintf (stderr, "Failed to open fd %d:%s\n", errno, strerror (errno));
		return;
	}

	//
	// Sets the interface attributes
	//

	// Gets the attributes default attributes from device.
	if (tcgetattr (usart->fd, &usart->tty) != 0) {
		fprintf (stderr, "Failed to get attributes %d:%s\n", errno, strerror (errno));
		return;
	}

	// Sets the IN/OUT BAUD Rate.
	cfsetspeed (&usart->tty, usart->speed);

	// Sets some flags.
	usart->tty.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB | CRTSCTS);
	usart->tty.c_cflag |= (CLOCAL | CREAD | CS8);

	usart->tty.c_iflag &= ~(IGNBRK | IXON | IXOFF | IXANY);
	
	usart->tty.c_lflag = 0;
	usart->tty.c_oflag = 0;

	usart->tty.c_cc[VMIN] = 0;
	usart->tty.c_cc[VTIME] = 1;

	// Attempts to set the new attributes.
	if (tcsetattr (usart->fd, TCSANOW, &usart->tty) != 0) {
		fprintf (stderr, "Failed to set attributes %d:%s\n", errno, strerror (errno));
		return;
	}
}

void ssfp_usart_close (ssfp_usart_t *usart) {
	close (usart->fd);
}

ssfp_frame_t *ssfp_usart_prepare_frame (ssfp_usart_t *usart, 
	uint16_t payloadSize, uint16_t port) {
	uint8_t *allocatedMemory = (uint8_t *) malloc (sizeof (ssfp_frame_t) + payloadSize);
	if (allocatedMemory == NULL) {
		fprintf (stderr, "Failed to allocate memory %d:%s\n", errno, strerror (errno));
		return NULL;
	}

	ssfp_frame_t *frame = (ssfp_frame_t *) allocatedMemory;

	// Configures the frame itself.
	frame->hdr.f = 0;
	frame->hdr.t_len = sizeof (ssfp_frame_t) + payloadSize;
	frame->hdr.port = port;

	// Sets the sender and destination address in the packet.
	memcpy ((void *) &frame->hdr.saadr, (const void *) usart->master_address, 3);
	memcpy ((void *) &frame->hdr.daadr, (const void *) usart->slave_address, 3);

	// Returns the prepared frame.
	return frame;
}

void ssfp_usart_write (ssfp_usart_t *usart, const ssfp_frame_t *frame) {
	// Writes the start sequence.
	write (usart->fd, (const void *) SSFP_START, 3);

	// Writes the frame size.
	uint16_t temp = (uint8_t) ((frame->hdr.t_len & 0xFF00) >> 8);
	write (usart->fd, &temp, 1);
	temp = (uint8_t) ((frame->hdr.t_len & 0x00FF) >> 0);
	write (usart->fd, &temp, 1);

	// Writes the frame contents.
	write (usart->fd, (const void *) frame, frame->hdr.t_len);
}

ssfp_frame_t *ssfp_usart_read (ssfp_usart_t *usart) {
	//
	// Waits for the start sequence
	//

	for (uint8_t i = 0, matches = 0; ; ++i) {
		uint8_t byte, received;
		if ((received = read(usart->fd, &byte, 1)) < 0) {
			return NULL;
		} else if (received == 1) {
			if (byte == 0x98) {
				++matches;

				if (matches == 3) {
					break;
				}
			} else {
				matches = 0;
			}
		}

		if (i > 5) {
			return NULL;
		}
	}

	//
	// Reads the frame length
	//

	uint16_t frame_len = 0;
	for (uint8_t i = 0, j = 0; ; ++i) {
		uint8_t byte, received;
		if ((received = read(usart->fd, &byte, 1)) < 0) {
			return NULL;
		} else if (received == 1) {
			if (j == 0) {
				frame_len |= (((uint16_t) byte) << 8);
				j = 1;
			} else if (j == 1) {
				frame_len |= (((uint16_t) byte) >> 0);
				break;
			}
		}

		if (i > 5) {
			return NULL;
		}
	}

	//
	// Reads the packet
	//

	// // Allocates the number of bytes specified in the frame length.
	uint8_t *buffer = (uint8_t *) malloc (frame_len);
	uint16_t buffer_i = 0;

	for (uint8_t i = 0; ; ++i) {
		uint8_t byte, received;
		if ((received = read(usart->fd, &byte, 1)) < 0) {
			return NULL;
		} else if (received == 1) {
			buffer[buffer_i++] = byte;

			if (buffer_i == frame_len) {
				break;
			}

			continue;
		}

		if (i > 5) {
			return NULL;
		}
	}

	return (ssfp_frame_t *) buffer;
}

void ssfp_usart_print_frame (const ssfp_frame_t *frame) {
	printf("{\n");
	printf("\tHdr: {\n");
	printf("\t\tlen: %d\n", frame->hdr.t_len);
	printf("\t\tsaadr: %02x:%02x:%02x\n", frame->hdr.saadr[0],
		frame->hdr.saadr[1], frame->hdr.saadr[2]);
	printf("\t\tdaadr: %02x:%02x:%02x\n", frame->hdr.daadr[0],
		frame->hdr.daadr[1], frame->hdr.daadr[2]);
	printf("\t\tcs: %02x\n", frame->hdr.cs);
	printf("\t\tf: %02x\n", frame->hdr.f);
	printf("\t\tport: %d\n", frame->hdr.port);
	printf("\t}\n");
	printf("}\n");
}