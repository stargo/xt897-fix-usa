/* Functions for decoding bands
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

#include <stdio.h>

void show_bands(uint64_t bands, int noheader)
{
	if (!noheader)
		printf("Bands enabled by 0x%016llx:\n", bands);

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
	if (bands & ((uint64_t)1 << 61)) printf("61: WCDMA: XI  - 1500\n");

	printf("\n");
}

void show_lte_bands(uint32_t bands, int noheader)
{
	int i;

	if (!noheader)
		printf("LTE Bands enabled by 0x%08x:\n", bands);

	for (i = 0; i < 32; i++) {
		if (bands & ((uint32_t)1 << i)) {
			printf("%02d: ", i+1);
			/* http://niviuk.free.fr/lte_band.php */
			switch (i+1) {
				case 1:
					printf("2100");
					break;
				case 2:
					printf("1900 PCS");
					break;
				case 3:
					printf("1800 +");
					break;
				case 4:
					printf("AWS 1");
					break;
				case 5:
					printf("850 ");
					break;
				case 6:
					printf("UMTS only");
					break;
				case 7:
					printf("2600 ");
					break;
				case 8:
					printf("900 ");
					break;
				case 9:
					printf("1800 ");
					break;
				case 10:
					printf("AWS 1+3");
					break;
				case 11:
					printf("1500 Lower");
					break;
				case 12:
					printf("700 ac ");
					break;
				case 13:
					printf("700 c ");
					break;
				case 14:
					printf("700 PS ");
					break;
				case 17:
					printf("700 bc ");
					break;
				case 18:
					printf("800 Lower");
					break;
				case 19:
					printf("800 Upper");
					break;
				case 20:
					printf("800 DD ");
					break;
				case 21:
					printf("1500 Upper");
					break;
				case 22:
					printf("3500 ");
					break;
				case 23:
					printf("2000 S-band");
					break;
				case 24:
					printf("1600 L-band");
					break;
				case 25:
					printf("1900 + ");
					break;
				case 26:
					printf("850 + ");
					break;
				case 27:
					printf("800 SMR");
					break;
				case 28:
					printf("700 APT");
					break;
				case 29:
					printf("700 de ");
					break;
				case 30:
					printf("2300 WCS");
					break;
				case 31:
					printf("450 ");
					break;
				case 32:
					printf("1500 L-band");
					break;
				default:
					printf("?");
					break;
			}
			printf("\n");
		}
	}

	printf("\n");
}
