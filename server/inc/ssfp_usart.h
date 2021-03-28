#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../shared/include/ssfp.h"

#pragma once

typedef struct {
	struct termios tty;
	int32_t fd, speed;

	uint8_t master_address[3];		/* CurrentDevice-Address */
	uint8_t slave_address[3];		/* SlaveDevice-Address */

	const char *path;
} ssfp_usart_t;

void ssfp_usart_init (ssfp_usart_t *usart);
void ssfp_usart_close (ssfp_usart_t *usart);
void ssfp_usart_write (ssfp_usart_t *usart, const ssfp_frame_t *frame);
ssfp_frame_t *ssfp_usart_read (ssfp_usart_t *usart);
void ssfp_usart_print_frame (const ssfp_frame_t *frame);

ssfp_frame_t *ssfp_usart_prepare_frame (ssfp_usart_t *usart, 
	uint16_t payloadSize, uint16_t port);