/* NV functions using the diag interface on Qualcomm modems
 *
 * Copyright (c) 2014-16 Michael Gernoth <michael@gernoth.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int logging_mode(int internal)
{
	char *mode;
	int fd;
	int len;

	if (internal)
		mode = "internal";
	else
		mode = "usb";

	printf("Setting logging_mode to %s...\n", mode);
	fd = open("/sys/devices/virtual/diag/diag/logging_mode", O_WRONLY);
	if (fd == -1) {
		perror("open");
		return 0;
	}

	len = write(fd, mode, strlen(mode));
	if (len != (int)strlen(mode)) {
		perror("write");
		return 0;
	}

	close(fd);

	return 1;
}

static uint16_t crc16(unsigned char* data_p, unsigned char length){
	unsigned short crc = 0xFFFF;
	int i;

	while (length--) {
		crc ^= *data_p++;
		for (i = 0; i < 8; i++) {
			if (crc & 1) {
				crc = (crc >> 1) ^ 0x8408;
			} else {
				crc = (crc >> 1);
			}
		}
	}

	return ~crc;
}

int diag_rw(int fd, unsigned char *req, int req_len, unsigned char *resp, int *resp_len)
{
	int ret;
	int len;
	unsigned char buf[1024];
	uint16_t crc;
	int i;

	memset(buf, 0, sizeof(buf));
	memcpy(buf, req, req_len);

	crc = crc16(req, req_len);
	buf[req_len++] = crc & 0xff;
	buf[req_len++] = (crc >> 8) & 0xff;

	for (i = 0; i < req_len; i++) {
		if ((buf[i] != 0x7d) && ((buf[i] != 0x7e)))
			continue;

		memmove(buf+i+1, buf+i, req_len - i);
		req_len++;
		buf[i] = 0x7d;
		i++;
		buf[i] ^= 0x20;
	}

	buf[i] = 0x7e;
	req_len++;

	do {
		errno = 0;
		ret = write(fd, buf, req_len);
	} while ((ret != req_len) && (errno == EAGAIN));

	if (ret != req_len) {
		perror("write");
		return 0;
	}

	len = read(fd, resp, *resp_len);
	if (len == -1) {
		perror("read");
		return 0;
	}

	for (i = 0; i < len; i++) {
		if (resp[i] == 0x7e) {
			/* Should actually check CRC here... */
			len = i - 2;
			break;
		}

		if (resp[i] != 0x7d)
			continue;

		memmove(resp + i, resp + i + 1, len - (i + 1));
		resp[i] ^= 0x20;
		len--;
	}

	*resp_len = len;

	return 1;
}
