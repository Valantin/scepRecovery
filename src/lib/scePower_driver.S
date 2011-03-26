.set noreorder

#include "pspimport.s"

IMPORT_START "scePower_driver", 0x00010011
IMPORT_FUNC "scePower_driver", 0xBC8823E8, scePowerIsBatteryExist
IMPORT_FUNC	"scePower_driver", 0xE5F8596E, scePowerGetBatteryLifePercent
IMPORT_FUNC	"scePower_driver", 0x5202A826, scePowerIsBatteryCharging
IMPORT_FUNC	"scePower_driver", 0x279492B3, scePowerIsLowBattery
IMPORT_FUNC	"scePower_driver", 0x737486F2, scePowerSetClockFrequency
IMPORT_FUNC	"scePower_driver", 0xFDB5BFE9, scePowerGetCpuClockFrequency
IMPORT_FUNC	"scePower_driver", 0x37DCC2DA, scePowerSetCpuClockFrequency
IMPORT_FUNC	"scePower_driver", 0xC5371DAD, scePowerSetBusClockFrequency
IMPORT_FUNC	"scePower_driver", 0xBF5BA7FC, scePowerGetBusClockFrequency
IMPORT_FUNC	"scePower_driver", 0x599A2901,scePowerRequestColdReset
IMPORT_FUNC	"scePower_driver", 0x47F4E1A8,scePowerRequestStandby
IMPORT_FUNC	"scePower_driver", 0xC9885394,scePowerRequestSuspend