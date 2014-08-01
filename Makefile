CMROOT=/build/Android/cm-11.0
CC=$(CMROOT)/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.7/bin/arm-linux-androideabi-gcc
CFLAGS=--sysroot $(CMROOT)/prebuilts/ndk/9/platforms/android-10/arch-arm/ -Wall

all: xt897-fix-usa

xt897-fix-usa: xt897-fix-usa.c

run: xt897-fix-usa
	adb push xt897-fix-usa /data/local/tmp/
	adb shell /data/local/tmp/xt897-fix-usa

clean:
	rm -f xt897-fix-usa xt897-fix-usa.o

.PHONY: clean run
