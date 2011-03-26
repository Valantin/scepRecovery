TARGET = recovery
OBJS = main.o ctrl.o graphic.o menu.o registry.o util.o
OBJS += ./lib/SysTimerForKernel.o ./lib/sceRtc.o \
		./lib/scePower_driver.o ./lib/SystemCtrlForKernel.o \
		./lib/VshCtrlLib.o ./lib/pspUsbDevice_driver.o

BUILD_PRX = 1

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

PRX_EXPORTS = exports.exp

PSP_FW_VERSION = 620

INCDIR = ./include
CFLAGS = -Os -G0 -Wall -fno-strict-aliasing -fno-builtin-printf
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ./lib
LIBS=  -lpspreg_driver -lpspusb_driver -lpspusbstor #-lpspusbdevice_driver
LDFLAGS = -mno-crt0 -nostartfiles

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak