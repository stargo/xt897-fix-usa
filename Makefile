CMROOT=/build/Android/cm-12.1
CC=$(CMROOT)/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-gcc
CFLAGS=--sysroot $(CMROOT)/prebuilts/ndk/9/platforms/android-10/arch-arm/ -Wall
LDFLAGS=-fPIE -fPIC -pie

all: xt897-fix-usa omap4-bands

xt897-fix-usa: xt897-fix-usa.c nv.c bands.c

omap4-bands: omap4-bands.c nv.c bands.c

run: xt897-fix-usa
	adb push xt897-fix-usa /data/local/tmp/
	adb shell /data/local/tmp/xt897-fix-usa

clean:
	rm -f xt897-fix-usa xt897-fix-usa.o

.PHONY: clean run
