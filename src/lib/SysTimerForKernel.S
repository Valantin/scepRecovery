.set noreorder

#include "pspimport.s"

IMPORT_START "SysTimerForKernel", 0x00010011
IMPORT_FUNC "SysTimerForKernel", 0xC01A8A7E, sceSTimerGetCount
IMPORT_FUNC "SysTimerForKernel", 0x9D20D194, sceSTimerStopCount
IMPORT_FUNC "SysTimerForKernel", 0x30D5FF7A, sceSTimerResetCount
IMPORT_FUNC "SysTimerForKernel", 0xCA553E9C, sceSTimerSetHandler
IMPORT_FUNC "SysTimerForKernel", 0xE7BD0E48, sceSTimerStartCount
IMPORT_FUNC "SysTimerForKernel", 0x4ECB4A4C, sceSTimerFree
IMPORT_FUNC "SysTimerForKernel", 0x8758ACCD, sceSTimerAlloc