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

#include "nv.h"
#include "bands.h"

#define DEFAULT_BANDS_XT897	(0x00000004e84187)
#define DEFAULT_BANDS_XT901	(0x00000000000000)	//?
#define DEFAULT_BANDS_XT905	(0x02000004e80380)
#define DEFAULT_BANDS_XT907	(0x02000004e80387)
#define DEFAULT_BANDS_XT925	(0x02000004e80380)
#define DEFAULT_BANDS_XT926	(0x02000004e80187)
#define DEFAULT_BANDS_MB886	(0x02000086e80587)

#define BANDS_ALL		(0xffffffffffffffff)

#define DEFAULT_LTE_BANDS_XT897	(0x01000000)	//LTE 1900 (25)
#define DEFAULT_LTE_BANDS_XT901	(0x0000081a)	//LTE 700 MHz Class 17 / 1700? / 2100 (2,4,5,12)
#define DEFAULT_LTE_BANDS_XT905	(0x00000044)	//LTE 1800 / 2600 (3,7)
#define DEFAULT_LTE_BANDS_XT907	(0x00001000)	//LTE 700 MHz Class 13 (13)
#define DEFAULT_LTE_BANDS_XT925	(0x00080044)	//LTE 800 / 1800 / 2600 (3,7,20)
#define DEFAULT_LTE_BANDS_XT926	(0x00001000)	//LTE 700 MHz Class 13 (13)
#define DEFAULT_LTE_BANDS_MB886	(0x0001001a)	//LTE 700 MHz Class 17 / 2100 (2,4,5,17)

#define LTE_BANDS_ALL		(0xffffffff)

enum bands {
	BANDS_NO_CHANGE,
	BANDS_XT897,
	BANDS_XT901,
	BANDS_XT905,
	BANDS_XT907,
	BANDS_XT925,
	BANDS_XT926,
	BANDS_MB886,
	BANDS_ALL_PHONES,
	BANDS_ADD_P_GSM,
	BANDS_ADD_WCDMA_900,
	BANDS_ADD_WCDMA_1700,
	BANDS_ADD_WCDMA_1800,
	BANDS_ADD_ONE,
	BANDS_REMOVE_ONE,
	BANDS_LTE_ADD_ONE,
	BANDS_LTE_REMOVE_ONE,
};

int main(int argc, char **argv)
{
	unsigned char nv_get[] = { 0x26, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char nv_set[] = { 0x27, 0x82, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char data[512];
	int fd;
	int len;
	int ret;
	uint64_t bands;
	int change_bands = BANDS_NO_CHANGE;
	uint64_t user_band = 0;
	uint64_t new_bands;
	uint32_t lte_bands;
	uint32_t new_lte_bands;
	char *endptr;
	int i;

	if (argc > 1) {
		do {
			if (argc == 2) {
				if (!strcmp(argv[1], "lock")) {
					printf("Locking USA GSM!\n");
					nv_set[3] = 0x01;
					break;
				} else if (!strcmp(argv[1], "all_bands")) {
					change_bands = BANDS_ALL_PHONES;
					break;
				} else if (!strcmp(argv[1], "xt897_bands")) {
					change_bands = BANDS_XT897;
					break;
				} else if (!strcmp(argv[1], "xt901_bands")) {
					change_bands = BANDS_XT901;
					break;
				} else if (!strcmp(argv[1], "xt905_bands")) {
					change_bands = BANDS_XT905;
					break;
				} else if (!strcmp(argv[1], "xt907_bands")) {
					change_bands = BANDS_XT907;
					break;
				} else if (!strcmp(argv[1], "xt925_bands")) {
					change_bands = BANDS_XT925;
					break;
				} else if (!strcmp(argv[1], "xt926_bands")) {
					change_bands = BANDS_XT926;
					break;
				} else if (!strcmp(argv[1], "mb886_bands")) {
					change_bands = BANDS_MB886;
					break;
				} else if (!strcmp(argv[1], "add_p_gsm")) {
					change_bands = BANDS_ADD_P_GSM;
					break;
				} else if (!strcmp(argv[1], "add_wcdma_900")) {
					change_bands = BANDS_ADD_WCDMA_900;
					break;
				} else if (!strcmp(argv[1], "add_wcdma_1700")) {
					change_bands = BANDS_ADD_WCDMA_1700;
					break;
				} else if (!strcmp(argv[1], "add_wcdma_1800")) {
					change_bands = BANDS_ADD_WCDMA_1800;
					break;
				} else if (!strcmp(argv[1], "band?")) {
					show_bands(BANDS_ALL, 1);
					exit(EXIT_SUCCESS);
				} else if (!strncmp(argv[1], "band",4)) {
					if (strlen(argv[1]) >= 6) {
						i = strtoul(argv[1]+5, &endptr, 10);
						if (*endptr == '\0' && i >= 0) {
							user_band = (uint64_t)1 << i;
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
				} else if (!strcmp(argv[1], "lte?")) {
					show_lte_bands(LTE_BANDS_ALL, 1);
					exit(EXIT_SUCCESS);
				} else if (!strncmp(argv[1], "lte",3)) {
					if (strlen(argv[1]) >= 5) {
						i = strtoul(argv[1]+4, &endptr, 10);
						if (*endptr == '\0' && i > 0) {
							user_band = (uint64_t)1 << (i-1);
							switch (*(argv[1]+3)) {
								case '+':
									change_bands = BANDS_LTE_ADD_ONE;
									break;
								case '-':
									change_bands = BANDS_LTE_REMOVE_ONE;
									break;
							}
						}
						break;
					}
				}
			}

			fprintf(stderr, "Syntax: %s [lock|bands]\n\n", argv[0]);
			fprintf(stderr, "\tlock\tlock operation on US GSM providers\n");
			fprintf(stderr, "Possible value for bands:\n");
			fprintf(stderr, "\txt897_bands\tdefault bands/lte-bands of XT897\n");
			fprintf(stderr, "\txt901_bands\tdefault bands/lte-bands of XT901\n");
			fprintf(stderr, "\txt905_bands\tdefault bands/lte-bands of XT905\n");
			fprintf(stderr, "\txt907_bands\tdefault bands/lte-bands of XT907\n");
			fprintf(stderr, "\txt925_bands\tdefault bands/lte-bands of XT925\n");
			fprintf(stderr, "\txt926_bands\tdefault bands/lte-bands of XT926\n");
			fprintf(stderr, "\tmb886_bands\tdefault bands/lte-bands of MB886\n");
			fprintf(stderr, "\tall_bands\tbands of all phones\n");
			fprintf(stderr, "\tadd_p_gsm\tAdd P_GSM to currently supported bands\n");
			fprintf(stderr, "\tadd_wcdma_900\tAdd WCDMA900 to currently supported bands\n");
			fprintf(stderr, "\tadd_wcdma_1700\tAdd WCDMA1700 to currently supported bands\n");
			fprintf(stderr, "\tadd_wcdma_1800\tAdd WCDMA1800 to currently supported bands\n");
			fprintf(stderr, "\tband?\tShow possible bands\n");
			fprintf(stderr, "\tband+X\tAdd band X to supported bands\n");
			fprintf(stderr, "\tband-X\tRemove band X from supported bands\n");
			fprintf(stderr, "\tlte?\tShow possible LTE bands\n");
			fprintf(stderr, "\tlte+X\tAdd LTE band X to supported LTE bands\n");
			fprintf(stderr, "\tlte-X\tRemove LTE band X from supported LTE bands\n");
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
	printf("Reading value of NV-item 8322...\n");

	len = sizeof(data);
	if (!diag_rw(fd, nv_get, sizeof(nv_get), data, &len)) {
		goto err;
	}

	if (len != sizeof(nv_get)) {
		printf("Wrong amount of data read %d != %d!\n", len, sizeof(nv_get));
		goto err;
	}

	printf("GSM locked in the US: %d\n", data[3]);
	if (data[3] == nv_set[3]) {
		printf("No need to change anything!\n");
	} else {
		printf("Setting value of NV-item 8322 to %d...\n", nv_set[3]);

		len = sizeof(data);
		if (!diag_rw(fd, nv_set, sizeof(nv_set), data, &len)) {
			goto err;
		}

		printf("Reading value of NV-item 8322...\n");

		len = sizeof(data);
		if (!diag_rw(fd, nv_get, sizeof(nv_get), data, &len)) {
			goto err;
		}

		if (len != sizeof(nv_get)) {
			printf("Wrong amount of data read %d != %d!\n", len, sizeof(nv_get));
			goto err;
		}

		printf("GSM locked in the US: %d\n", data[3]);
		if (data[3] != nv_set[3]) {
			printf("Couldn't change GSM-us-lock-bit!\n");
			goto err;
		}
	}

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
				printf("Enabling bands of XT897, XT905, XT907, XT925 and XT926:\n");
				new_bands |= DEFAULT_BANDS_XT897;
				new_bands |= DEFAULT_BANDS_XT901;
				new_bands |= DEFAULT_BANDS_XT905;
				new_bands |= DEFAULT_BANDS_XT907;
				new_bands |= DEFAULT_BANDS_XT925;
				new_bands |= DEFAULT_BANDS_XT926;
				new_bands |= DEFAULT_BANDS_MB886;
				break;
			case BANDS_XT897:
				printf("Resetting to default Photon Q bands:\n");
				new_bands = DEFAULT_BANDS_XT897; /* default on XT897 */
				break;
			case BANDS_XT901:
				printf("Resetting to default Electrify M bands:\n");
				new_bands = DEFAULT_BANDS_XT901; /* default on XT901 */
				break;
			case BANDS_XT905:
				printf("Resetting to default Razr M bands:\n");
				new_bands = DEFAULT_BANDS_XT905; /* default on XT905 */
				break;
			case BANDS_XT907:
				printf("Resetting to default Droid Razr M bands:\n");
				new_bands = DEFAULT_BANDS_XT907; /* default on XT907 */
				break;
			case BANDS_XT925:
				printf("Resetting to default Razr HD bands:\n");
				new_bands = DEFAULT_BANDS_XT925; /* default on XT925 */
				break;
			case BANDS_XT926:
				printf("Resetting to default Droid Razr HD bands:\n");
				new_bands = DEFAULT_BANDS_XT926; /* default on XT926 */
				break;
			case BANDS_MB886:
				printf("Resetting to default Atrix HD bands:\n");
				new_bands = DEFAULT_BANDS_MB886; /* default on MB886 */
				break;
			case BANDS_ADD_P_GSM:
				printf("Adding primary GSM:\n");
				new_bands = bands | (1 << 9);
				break;
			case BANDS_ADD_WCDMA_900:
				printf("Adding WCDMA 900:\n");
				new_bands = bands | ((uint64_t)1 << 49); /* WCDMA 900 */
				break;
			case BANDS_ADD_WCDMA_1700:
				printf("Adding WCDMA 1700:\n");
				new_bands = bands | ((uint64_t)1 << 25); /* WCDMA 1700 */
				break;
			case BANDS_ADD_WCDMA_1800:
				printf("Adding WCDMA 1800:\n");
				new_bands = bands | ((uint64_t)1 << 24); /* WCDMA 1800 */
				//new_bands |= ((uint64_t)1 << 48); /* Europe 2600 */
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

	/* LTE */
	printf("\n");
	printf("Reading value of NV-item 6828...\n");

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
	for (i = 3; i >= 0; i--) {
		lte_bands <<= 8;
		lte_bands |= data[3+i];
	}

	show_lte_bands(lte_bands, 0);

	printf("\n");

	/* This also needs a modified/removed
	 * /nv/item_files/modem/mmode/lte_bandpref in EFS on xt897.
	 * After this the modem will crash as soon as it tries to
	 * switch to a newly enabled frequency
	 */
	if (change_bands != BANDS_NO_CHANGE) {
		new_lte_bands = 0;
		switch (change_bands) {
			case BANDS_XT897:
				printf("Resetting to default Photon Q LTE bands:\n");
				new_lte_bands = DEFAULT_LTE_BANDS_XT897; /* default on XT897 */
				break;
			case BANDS_XT901:
				printf("Resetting to default Electrify M LTE bands:\n");
				new_lte_bands = DEFAULT_LTE_BANDS_XT901; /* default on XT901 */
				break;
			case BANDS_XT905:
				printf("Resetting to default Razr M LTE bands:\n");
				new_lte_bands = DEFAULT_LTE_BANDS_XT905; /* default on XT905 */
				break;
			case BANDS_XT907:
				printf("Resetting to default Droid Razr M LTE bands:\n");
				new_lte_bands = DEFAULT_LTE_BANDS_XT907; /* default on XT907 */
				break;
			case BANDS_XT925:
				printf("Resetting to default Razr HD LTE bands:\n");
				new_lte_bands = DEFAULT_LTE_BANDS_XT925; /* default on XT925 */
				break;
			case BANDS_XT926:
				printf("Resetting to default Droid Razr HD LTE bands:\n");
				new_lte_bands = DEFAULT_LTE_BANDS_XT926; /* default on XT926 */
				break;
			case BANDS_MB886:
				printf("Resetting to default Atrix HD LTE bands:\n");
				new_lte_bands = DEFAULT_LTE_BANDS_MB886; /* default on MB886 */
				break;
			case BANDS_LTE_ADD_ONE:
				new_lte_bands = lte_bands;
				new_lte_bands |= user_band;
				break;
			case BANDS_LTE_REMOVE_ONE:
				new_lte_bands = lte_bands;
				new_lte_bands &= ~user_band;
				break;
			default:
				new_lte_bands = lte_bands;
				break;
		}

		if (lte_bands == new_lte_bands) {
			printf("No need to change anything!\n");
		} else {
			printf("Setting value of NV-item 6828 to 0x%06x (%u)...\n", new_lte_bands, new_lte_bands);
			show_lte_bands(new_lte_bands, 0);

			memcpy(data, nv_set, sizeof(nv_set));

			data[1] = 0xAC;
			data[2] = 0x1A;

			for (i = 3; i >= 0; i--) {
				data[3+i] = (new_lte_bands >> (i * 8)) & 0xff;
			}

			len = sizeof(data);
			if (!diag_rw(fd, data, sizeof(nv_set), data, &len)) {
				goto err;
			}
		}
		printf("\n");
	}

	close(fd);

	if (!logging_mode(0)) {
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);

err:
	logging_mode(0);
	exit(EXIT_FAILURE);
}
