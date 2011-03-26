.set noreorder

#include "pspimport.s"

IMPORT_START	"pspUsbDevice_driver",0x00010000
IMPORT_FUNC		"pspUsbDevice_driver",0xD4D90520, pspUsbDeviceSetDevice
IMPORT_FUNC		"pspUsbDevice_driver",0xB3CF0329, pspUsbDeviceFinishDevice