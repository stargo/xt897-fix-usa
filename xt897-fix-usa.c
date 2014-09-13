/* Allow usage of US GSM-providers on Motorola Photon Q by setting RF NV 8322 to 0
 *
 * Copyright (c) 2014 Michael Gernoth <michael@gernoth.net>
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
	if (len != strlen(mode)) {
		perror("write");
		return 0;
	}

	close(fd);

	return 1;
}

void show_bands(uint64_t bands)
{
	printf("Bands enabled by 0x%016llx (%llu):\n", bands, bands);

	if (bands & ((uint64_t)1 <<  0)) printf("00: CDMA-1: BC 0, A-System\n");
	if (bands & ((uint64_t)1 <<  1)) printf("01: CDMA-1: BC 0, B-System\n");
	if (bands & ((uint64_t)1 <<  2)) printf("02: CDMA-1: BC 1, all blocks\n");
	if (bands & ((uint64_t)1 <<  3)) printf("03: CDMA-1: BC 2, place holder\n");
	if (bands & ((uint64_t)1 <<  4)) printf("04: CDMA-1: BC 3, A-System\n");
	if (bands & ((uint64_t)1 <<  5)) printf("05: CDMA-1: BC 4, all blocks\n");
	if (bands & ((uint64_t)1 <<  6)) printf("06: CDMA-1: BC 5, all blocks\n");
	if (bands & ((uint64_t)1 <<  7)) printf("07: GSM: DCS 1800\n");
	if (bands & ((uint64_t)1 <<  8)) printf("08: GSM: Extended GSM (E-GSM)\n");
	if (bands & ((uint64_t)1 <<  9)) printf("09: GSM: Primary GSM (P-GSM)\n");
	if (bands & ((uint64_t)1 << 10)) printf("10: CDMA-1: BC 6\n");
	if (bands & ((uint64_t)1 << 11)) printf("11: CDMA-1: BC 7\n");
	if (bands & ((uint64_t)1 << 12)) printf("12: CDMA-1: BC 8\n");
	if (bands & ((uint64_t)1 << 13)) printf("13: CDMA-1: BC 9\n");
	if (bands & ((uint64_t)1 << 14)) printf("14: CDMA-2: BC 10\n");
	if (bands & ((uint64_t)1 << 15)) printf("15: CDMA-2: BC 11\n");
	if (bands & ((uint64_t)1 << 16)) printf("16: GSM: GSM 450\n");
	if (bands & ((uint64_t)1 << 17)) printf("17: GSM: GSM 480\n");
	if (bands & ((uint64_t)1 << 18)) printf("18: GSM: GSM 750\n");
	if (bands & ((uint64_t)1 << 19)) printf("19: GSM: GSM 850\n");
	if (bands & ((uint64_t)1 << 20)) printf("20: GSM: Railway GSM 900\n");
	if (bands & ((uint64_t)1 << 21)) printf("21: GSM: PCS 1900\n");
	if (bands & ((uint64_t)1 << 22)) printf("22: WCDMA: I   - Europe Japan China IMT 2100\n");
	if (bands & ((uint64_t)1 << 23)) printf("23: WCDMA: II  - US PCS 1900\n");
	if (bands & ((uint64_t)1 << 24)) printf("24: WCDMA: III - Europe China DCS 1800\n");
	if (bands & ((uint64_t)1 << 25)) printf("25: WCDMA: IV  - US 1700\n");
	if (bands & ((uint64_t)1 << 26)) printf("26: WCDMA: V   - US 850\n");
	if (bands & ((uint64_t)1 << 27)) printf("27: WCDMA: VI  - Japan 800\n");
	if (bands & ((uint64_t)1 << 28)) printf("28: CDMA-2: BC 12\n");
	if (bands & ((uint64_t)1 << 29)) printf("29: CDMA-2: BC 14\n");
	if (bands & ((uint64_t)1 << 31)) printf("31: CDMA-2: BC 15\n");
	if (bands & ((uint64_t)1 << 48)) printf("48: WCDMA: VII - Europe 2600\n");
	if (bands & ((uint64_t)1 << 49)) printf("49: WCDMA: VIII- Europe Japan 900\n");
	if (bands & ((uint64_t)1 << 50)) printf("50: WCDMA: IX  - Japan 1700\n");
	if (bands & ((uint64_t)1 << 56)) printf("56: CDMA-2: BC 16\n");
	if (bands & ((uint64_t)1 << 57)) printf("57: CDMA-2: BC 17\n");
	if (bands & ((uint64_t)1 << 58)) printf("58: CDMA-2: BC 18\n");
	if (bands & ((uint64_t)1 << 59)) printf("59: CDMA-2: BC 19\n");
	if (bands & ((uint64_t)1 << 61)) printf("50: WCDMA: XI  - 1500\n");

	printf("\n");
}

void show_lte_bands(uint32_t bands)
{
	printf("LTE Bands enabled by 0x%08x (%u):\n", bands, bands);

	/* Guessed from defaults of different devices */
	if (bands & ((uint32_t)1 << 0) <<  0) printf("1900 MHz\n");
	if (bands & ((uint32_t)1 << 3) <<  8) printf("800 MHz\n");
	if (bands & ((uint32_t)1 << 4) << 16) printf("700 MHz\n");
	if (bands & ((uint32_t)1 << 2) << 24) printf("1800 or 2600 MHz\n");
	if (bands & ((uint32_t)1 << 6) << 24) printf("1800 or 2600 MHz\n");

	printf("\n");
}

int diag_rw(int fd, unsigned char *req, int req_len, unsigned char *resp, int *resp_len)
{
	int ret;
	int len;

	do {
		errno = 0;
		ret = write(fd, req, req_len);
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

	*resp_len = len;

	return 1;
}

int main(int argc, char **argv)
{
	unsigned char nv_get[] = { 0x26, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4c, 0x64, 0x7e };
	unsigned char nv_set[] = { 0x27, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x54, 0x7e };
	unsigned char data[512];
	int fd;
	int len;
	int ret;
	uint64_t bands;
	uint64_t new_bands;
	uint32_t lte_bands;
	uint32_t new_lte_bands;
	int i;

	if (argc > 1) {
		do {
			if (argc == 2) {
				if (!strcmp(argv[1], "lock")) {
					printf("Locking USA GSM!\n");
					nv_set[3] = 0x01;
					break;
				}
			}

			fprintf(stderr, "Syntax: %s [lock]\n", argv[0]);
			exit(EXIT_FAILURE);
		} while(0);
	}

	if (!logging_mode(1)) {
		exit(EXIT_FAILURE);
	}

	fd = open("/dev/diag_tty", O_RDWR);
	if ((fd == -1) && (errno == ENOENT)) {
		dev_t dev;

		printf("Creating /dev/diag_tty...\n");

		dev = makedev(185, 0);
		ret = mknod("/dev/diag_tty", S_IFCHR | 0600, dev);
		if (ret == -1) {
			perror("mknod");
			goto err;
		}

		fd = open("/dev/diag_tty", O_RDWR);
	}

	if (fd == -1) {
		perror("open");
		goto err;
	}

	printf("Opened diag...\n");

	/* GSM inside US */
	printf("\n");
	printf("Getting value of NV-item 8322...\n");

	len = sizeof(data);
	if (!diag_rw(fd, nv_get, sizeof(nv_get), data, &len)) {
		goto err;
	}

	if (len != sizeof(nv_get)) {
		printf("Wrong amount of data read %d != %d!\n", len, sizeof(nv_get));
		goto err;
	}

	printf("GSM outside USA only: %d\n", data[3]);
	if (data[3] == nv_set[3]) {
		printf("No need to change anything!\n");
	} else {
		printf("Setting value of NV-item 8322 to %d...\n", nv_set[3]);

		len = sizeof(data);
		if (!diag_rw(fd, nv_set, sizeof(nv_set), data, &len)) {
			goto err;
		}

		printf("Getting value of NV-item 8322...\n");

		len = sizeof(data);
		if (!diag_rw(fd, nv_get, sizeof(nv_get), data, &len)) {
			goto err;
		}

		if (len != sizeof(nv_get)) {
			printf("Wrong amount of data read %d != %d!\n", len, sizeof(nv_get));
			goto err;
		}

		printf("GSM outside USA only: %d\n", data[3]);
		if (data[3] != nv_set[3]) {
			printf("Couldn't change GSM-outside-USA-only-bit!\n");
			goto err;
		}
	}

	/* Supported bands */
	printf("\n");
	printf("Getting value of NV-item 1877...\n");

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

	show_bands(bands);

#if 0
	new_bands = bands;
	new_bands |= 82329991; /* default on XT897 */
	new_bands |= 562950035735424; /* default on XT925 */
	//new_bands |= ((uint64_t)1 << 9); /* P-GSM */
	//new_bands |= ((uint64_t)1 << 49); /* Europe Japan 900 */
	new_bands |= ((uint64_t)1 << 24); /* WCDMA 1800 */
	new_bands |= ((uint64_t)1 << 25); /* WCDMA 1700 */
	//new_bands |= ((uint64_t)1 << 48); /* Europe 2600 */

	if (bands == new_bands) {
		printf("No need to change anything!\n");
	} else {
		printf("Setting value of NV-item 1877 to 0x%016llx (%llu)...\n", new_bands, new_bands);
		show_bands(new_bands);

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
#endif

	/* LTE */
	printf("\n");
	printf("Getting value of NV-item 6828...\n");

	nv_get[1] = 0xAC;
	nv_get[2] = 0x1A;

	len = sizeof(data);
	if (!diag_rw(fd, nv_get, sizeof(nv_get), data, &len)) {
		goto err;
	}

	if (len != sizeof(nv_get)) {
		printf("Wrong amount of data read %d != %d!\n", len, sizeof(nv_get));
		goto err;
	}

	lte_bands = 0;
	for (i = 0; i < 4; i++) {
		lte_bands <<= 8;
		lte_bands |= data[3+i];
	}

	show_lte_bands(lte_bands);

	printf("\n");

	/* This also needs a modified/removed
	 * /nv/item_files/modem/mmode/lte_bandpref in EFS.
	 * After this the modem will crash as soon as it tries to
	 * switch to a newly enabled frequency
	 */
#if 0
	new_lte_bands = lte_bands;
	new_lte_bands |= 0x01; /* default on XT897, 1900MHz */
	//new_lte_bands |= 0x100000; /* default on XT907, XT926, 700MHz? */
	//new_lte_bands |= 0x44000800; /* default on XT925 */
	//new_lte_bands |= 0x44000000; /* default on XT905 */

	if (lte_bands == new_lte_bands) {
		printf("No need to change anything!\n");
	} else {
		printf("Setting value of NV-item 6828 to 0x%06x (%u)...\n", new_lte_bands, new_lte_bands);
		show_lte_bands(new_lte_bands);

		memcpy(data, nv_set, sizeof(nv_set));

		data[1] = 0xAC;
		data[2] = 0x1A;

		for (i = 0; i < 4; i++) {
			data[3+i] = (new_lte_bands >> ((3 - i) * 8)) & 0xff;
		}

		len = sizeof(data);
		if (!diag_rw(fd, data, sizeof(nv_set), data, &len)) {
			goto err;
		}
	}
	printf("\n");
#endif

	close(fd);

	if (!logging_mode(0)) {
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);

err:
	logging_mode(0);
	exit(EXIT_FAILURE);
}
