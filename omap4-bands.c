/* Change bands of Motorola Omap4 phones
 *
 * Copyright (c) 2016 Michael Gernoth <michael@gernoth.net>
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
#include <termios.h>
#include <unistd.h>

#include "nv.h"
#include "bands.h"

#define DEFAULT_BANDS_XT894	(0x02000004e80387)
#define DEFAULT_BANDS_XT910	(0x02000004e80180)

#define BANDS_ALL		(0xffffffffffffffff)

enum bands {
	BANDS_NO_CHANGE,
	BANDS_XT897,
	BANDS_XT910,
	BANDS_ALL_PHONES,
	BANDS_ADD_ONE,
	BANDS_REMOVE_ONE,
};

int main(int argc, char **argv)
{
	unsigned char nv_get[] = { 0x26, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char nv_set[] = { 0x27, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char data[512];
	int fd;
	int len;
	uint64_t bands;
	int change_bands = BANDS_NO_CHANGE;
	uint64_t user_band = 0;
	uint64_t new_bands;
	char *endptr;
	struct termios stbuf;
	int i;

	if (argc > 1) {
		do {
			if (argc == 2) {
				if (!strcmp(argv[1], "all_bands")) {
					change_bands = BANDS_ALL_PHONES;
					break;
				} else if (!strcmp(argv[1], "xt894_bands")) {
					change_bands = BANDS_XT897;
					break;
				} else if (!strcmp(argv[1], "xt910_bands")) {
					change_bands = BANDS_XT910;
					break;
				} else if (!strcmp(argv[1], "band?")) {
					show_bands(BANDS_ALL, 1);
					exit(EXIT_SUCCESS);
				} else if (!strncmp(argv[1], "band",4)) {
					if (strlen(argv[1]) >= 6) {
						i = strtoul(argv[1]+5, &endptr, 10);
						if (*endptr == '\0' && i >= 0) {
							user_band = 1 << i;
							switch (*(argv[1]+4)) {
								case '+':
									change_bands = BANDS_ADD_ONE;
									break;
								case '-':
									change_bands = BANDS_REMOVE_ONE;
									break;
							}
						}
						break;
					}
				}
			}

			fprintf(stderr, "Syntax: %s [lock|bands]\n\n", argv[0]);
			fprintf(stderr, "Possible value for bands:\n");
			fprintf(stderr, "\txt894_bands\tdefault bands/lte-bands of XT894\n");
			fprintf(stderr, "\tall_bands\tbands of all phones\n");
			fprintf(stderr, "\tband?\tShow possible bands\n");
			fprintf(stderr, "\tband+X\tAdd band X to supported bands\n");
			fprintf(stderr, "\tband-X\tRemove band X from supported bands\n");
			exit(EXIT_FAILURE);
		} while(0);
	}

	fd = open("/dev/ttyUSB0", O_RDWR);
	if (fd == -1) {
		perror("open");
		goto err;
	}

	memset (&stbuf, 0, sizeof (stbuf));
	if (tcgetattr(fd, &stbuf) != 0) {
		perror("tcgetattr");
		goto err;
	}

	stbuf.c_cflag &= ~(CBAUD | CSIZE | CSTOPB | CLOCAL | PARENB);
	stbuf.c_iflag &= ~(HUPCL | IUTF8 | IUCLC | ISTRIP | IXON | IXOFF | IXANY | ICRNL);
	stbuf.c_oflag &= ~(OPOST | OCRNL | ONLCR | OLCUC | ONLRET);
	stbuf.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO | ECHOE | ECHOK | ECHONL);
	stbuf.c_lflag &= ~(NOFLSH | XCASE | TOSTOP | ECHOPRT | ECHOCTL | ECHOKE);
	stbuf.c_cc[VMIN] = 1;
	stbuf.c_cc[VTIME] = 0;
	stbuf.c_cc[VEOF] = 1;
	stbuf.c_cflag |= (B115200 | CS8 | CREAD | 0 | 0);  /* No parity, 1 stop bit */

	if (tcsetattr(fd, TCSANOW, &stbuf) < 0) {
		perror("tcsetattr");
		goto err;
	}

	printf("Opened diag...\n");

	/* Supported bands */
	printf("\n");
	printf("Reading value of NV-item 1877...\n");

	nv_get[1] = 0x55;
	nv_get[2] = 0x07;

	len = sizeof(data);
	if (!diag_rw(fd, nv_get, sizeof(nv_get), data, &len)) {
		goto err;
	}

	if (len != sizeof(nv_get)) {
		printf("Wrong amount of data read %d != %d!\n", len, sizeof(nv_get));
		goto err;
	}

	bands = 0;
	for (i = 7; i >= 0; i--) {
		bands <<= 8;
		bands |= data[3+i];
	}

	show_bands(bands, 0);

	if (change_bands != BANDS_NO_CHANGE) {
		new_bands = 0;

		switch (change_bands) {
			case BANDS_ALL_PHONES:
				printf("Enabling bands of XT894:\n");
				new_bands |= DEFAULT_BANDS_XT894;
				break;
			case BANDS_XT897:
				printf("Resetting to default Droid 4 bands:\n");
				new_bands = DEFAULT_BANDS_XT894;
				break;
			case BANDS_XT910:
				printf("Resetting to default Razr bands:\n");
				new_bands = DEFAULT_BANDS_XT910;
				break;
			case BANDS_ADD_ONE:
				new_bands = bands;
				new_bands |= user_band;
				break;
			case BANDS_REMOVE_ONE:
				new_bands = bands;
				new_bands &= ~user_band;
				break;
			default:
				new_bands = bands;
				break;
		}

		if (bands == new_bands) {
			printf("No need to change anything!\n");
		} else {
			printf("Setting value of NV-item 1877 to 0x%016llx (%llu)...\n", new_bands, new_bands);
			show_bands(new_bands, 0);

			memcpy(data, nv_set, sizeof(nv_set));

			data[1] = 0x55;
			data[2] = 0x07;

			for (i = 7; i >= 0; i--) {
				data[3+i] = (new_bands >> (i * 8)) & 0xff;
			}

			len = sizeof(data);
			if (!diag_rw(fd, data, sizeof(nv_set), data, &len)) {
				goto err;
			}
		}
		printf("\n");
	}

	close(fd);

	exit(EXIT_SUCCESS);

err:
	exit(EXIT_FAILURE);
}
