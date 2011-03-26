/*	
	SCEP-Recovery 0.1

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    
*/
#ifndef _SCEP_RECOVERY_TEXT_
#define _SCEP_RECOVERY_TEXT_

#include <pspsdk.h>

static const char * plugin_path[] =
{
	"ms0:/seplugins/vsh.txt",
	"ms0:/seplugins/game.txt",
	"ms0:/seplugins/pops.txt"
};

static const char * isEnter[] =
{
	"O is Enter now!",
	"X is Enter now!"
};

static const char * wma[] =
{
	"Activating WMA...",
	"WMA was already activated."
};

static const char * flashplayer[] =
{
	"Activating Flash Player...",
	"Flash Player was already activated."
};

static const char * enable_disable[] =
{
	"[disable]",
	"[enable]"
};

static const char * fake_region[] =
{
	"[disable]",
	"[Japan]",
	"[America]",
	"[Europe]",
	"[Korea]",
	"[United Kingdom]",
	"[Mexico]",
	"[Australia/New Zealand]",
	"[East]",
	"[Taiwan]",
	"[Russia]",
	"[China]",
	"[Debug Type I]",
	"[Debug Type II]"
};

static const u8 fake_region_s = 14;

static const char * menu_main3xx[] =
{
	"Settings",
	"Plugins",
    "USBDevice",
	"Power"
};

static const u8 menu_main3xx_s = 4;

static const char * menu_power[] =
{
	"Sleep",
	"PowerOff",
	"Reboot",
    "ResetVSH",
};

static const u8 menu_power_s = 4;

static const char * menu_settings[] =
{
	"Base config",
	"CPU Speed",
	"Registry hack"
};

static const u8 menu_settings_s = 3;

static const char * menu_baseconfig[] =
{
	"Skip GameBoot",
	"Show MAC Address",
	"Original Update",
	"Hide Pic",
	"Show OFW Version",
	"USB Charge/slimColor",
	"Fast Scroll",
	"Protect Flash",
	"Fake Region"
};

static const u8 menu_baseconfig_s = 9;

static const char * menu_advanced[] =
{
	"Capture",
	"File Manager"
};

static const u8 menu_advanced_s = 2;

static const char * menu_registryhack[] =
{
	"Swap Button O/X",
	"Activate WMA",
	"Activate Flash Player"
};

static const u8 menu_registryhack_s = 3;

static const char * menu_cpuspeed[] =
{
	"CPU VSH",
	"CPU GAME"
};

static u8 menu_cpuspeed_s = 2;

static const char * menu_plugin[] =
{
	"Plugin VSH",
	"Plugin GAME",
	"Plugin POPS"
};

static const u8 menu_plugin_s = 3;

static const char * menu_toggleusb[] =
{
	"[Toggle ms0:]",
	"[Toggle flash0:]",
	"[Toggle flash1:]",
    "[Toggle flash2:]",
    "[Toggle flash3:]",
    "[Toggle umd0:]"
};

static const u8 menu_toggleusb_s = 6;

#endif
