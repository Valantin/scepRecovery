.set noreorder

#include "pspimport.s"

IMPORT_START	"sceRtc_driver",0x00010000
IMPORT_FUNC		"sceRtc_driver",0xA0B3BF0F,sceRtcGetCurrentClockLocalTime
