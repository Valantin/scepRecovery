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
#include "menu.h"
#include "ctrl.h"
#include "text.h"
#include "util.h"
#include "registry.h"

#include <pspsdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pspsystimer.h>
#include <psploadexec_kernel.h>
#include <psppower.h>
#include <psprtc.h>
#include <systemctrl.h>
#include <pspusbdevice.h>
#include <pspusbstor.h>

static u32 PSP_CTRL_YES = PSP_CTRL_CROSS, PSP_CTRL_NO = PSP_CTRL_CIRCLE;

int tickHandler() {
    static int div = 59;
    Point p0, p1;
    div++;
    if(div > 59) {
        div = 0;
        pspTime psptime;
        char s[64];
        sceRtcGetCurrentClockLocalTime(&psptime);
        sprintf(s, "%02u:%02u:%02u", psptime.hour, psptime.minutes, psptime.seconds);
        p0.x = 240; p0.y = 240; p1.x = 325; p1.y = 260;
        drawRect_p(15, true, 0, &p0, &p1);
        p0.x = 244; p0.y = 255;
        drawString_p(s, FTHICK | FSHADOW, &p0);
    }
    return -1;
}

static u32 drawMenu(const char ** item, int item_num, int * pos, Point * p0, int height, u32 key, bool init) {
    int lpos = *pos, i, maxlen = getMaxlen(item, item_num);
    u32 press_key;
    if(init) {
        setP(p0->x - 10 , p0->y, p0->x + maxlen * 8 + 10, p0->y + item_num * 20 + 20);
        drawRect(15, true, 0x0A00FFFF);
        for(i = 0; i < item_num; i ++) {
            if(i == *pos) {
                setP(p0->x - 4, p0->y + height * i + 12, p0->x + maxlen * 8 + 4, p0->y + height * i + 28);
                drawRect(0, false, 0);
            }
            setP0(p0->x, p0->y + height * i + 23);
            drawString(item[i], FSHADOW);
        }
    }
    while(1) {
        if(lpos != *pos) {
            setP(p0->x - 4, p0->y + height * lpos + 12, p0->x + maxlen * 8 + 4, p0->y + height * lpos + 28);
            drawRect(0, true, 0);
            setP0(p0->x, p0->y + height * lpos + 23);
            drawString(item[lpos], FSHADOW);
            setP(p0->x - 4, p0->y + height * (*pos) + 12, p0->x + maxlen * 8 + 4, p0->y + height * (*pos) + 28);
            drawRect(0, true, 0);
            drawRect(0, false, 0);
            setP0(p0->x, p0->y + height * (*pos) + 23);
            drawString(item[*pos], FSHADOW);
            lpos = *pos;
        }
        press_key = ctrlWaitMask(key);
        switch(press_key) {
            case PSP_CTRL_LEFT:
                *pos -= 1;
                if(*pos <= -1) {
                    *pos = item_num - 1;
                }
                break;
            case PSP_CTRL_UP:
                *pos -= 1;
                if(*pos <= -1) {
                    *pos = item_num - 1;
                }
                break;
            case PSP_CTRL_RIGHT:
                *pos += 1;
                if(*pos >= item_num) {
                    *pos = 0;
                }
                break;
            case PSP_CTRL_DOWN:
                *pos += 1;
                if(*pos >= item_num) {
                    *pos = 0;
                }
                break;
            case PSP_CTRL_CROSS:
            case PSP_CTRL_CIRCLE:
                if(key == PSP_CTRL_YES) {
                    break;
                } else {
                    return press_key;
                }
                break;
            default :
                return press_key;
                break;
        }
    }
    return 0;
}

static int baseconfigMenu() {
    TNConfig config;
    sctrlSEGetConfig(&config);
    u8 cfw[9];
    cfw[0] = config.skipgameboot;
    cfw[1] = config.showmac;
    cfw[2] = config.notnupdate;
    cfw[3] = config.hidepic;
    cfw[4] = config.nospoofversion;
    cfw[5] = config.slimcolor;
    cfw[6] = config.fastscroll;
    cfw[7] = config.protectflash;
    cfw[8] = config.fakeregion;
    Point p0;
    p0.x = 30; p0.y = 35;
    int pos = 0;
    setP(10 , 35, 370, 225);
    drawRect(15, true, 0x0A00FFFF);
    int i;
    setP(p0.x - 4, p0.y + 12, p0.x + strlen(menu_baseconfig[0]) * 10 + 4, p0.y + 28);
    drawRect(0, false, 0);
    for(i = 0; i < menu_baseconfig_s; i ++) {
        setP0(p0.x, p0.y + 20 * i + 23);
        drawString(menu_baseconfig[i], FSHADOW);
        setP0(274, p0.y + 20 * i + 23);
        if(i == 8) {
            drawString(fake_region[cfw[i]], FSHADOW);
        } else {
            drawString(enable_disable[cfw[i]], FSHADOW);
        }
    }
    while(1) {
        u32 key = drawMenu(menu_baseconfig, menu_baseconfig_s, &pos, &p0, 20, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, false);
        if(key == PSP_CTRL_YES) {
            char * item[2];
            if(pos == 8) {
                item[0] = (char *)fake_region[cfw[pos]];
                cfw[pos] ++;
                if(cfw[pos] >= fake_region_s) {
                    cfw[pos] = 0;
                }
                item[1] = (char *)fake_region[cfw[pos]];
            } else {
                item[0] = (char *)enable_disable[cfw[pos]];
                cfw[pos] = 1 - cfw[pos];
                item[1] = (char *)enable_disable[cfw[pos]];
            }
            setP(270, p0.y + 20 * pos + 12, 280 + strlen(item[0]) * 8, p0.y + 20 * pos + 28);
            drawRect(0, true, 0);
            setP0(274, p0.y + 20 * pos + 23);
            drawString(item[1], FSHADOW);
       } else {
            config.skipgameboot = cfw[0];
            config.showmac = cfw[1];
            config.notnupdate = cfw[2];
            config.hidepic = cfw[3];
            config.nospoofversion = cfw[4];
            config.slimcolor = cfw[5];
            config.fastscroll = cfw[6];
            config.protectflash = cfw[7];
            config.fakeregion = cfw[8];
            sctrlSESetConfig(&config);
            setP(10 , 35, 380, 235);
            clearRect(0);
            return 0;
        }
    }
    return 0;
}

static int registryhackMenu(Point * p0) {
    int pos = 0;
    int maxlen = getMaxlen(menu_registryhack, menu_registryhack_s);
    u32 value;
    bool init = true;
    while(1) {
        u32 key = drawMenu(menu_registryhack, menu_registryhack_s, &pos, p0, 20, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, init);
        if(key == PSP_CTRL_YES) {
            init = false;
            switch(pos) {
                case 0:
                    getRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", &value);
                    value = 1 - value;
                    setRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", value);
                    setP(p0->x, 45, p0->x + strlen(isEnter[value]) * 8 + 20, 65);
                    drawRect(10, true, 0x0800FFFF);
                    setP0(p0->x + 10, 60);
                    drawString(isEnter[value], FSHADOW);
                    sceKernelDelayThread(1000000);
                    setP(p0->x, 45, p0->x + strlen(isEnter[value]) * 8 + 30, 75);
                    clearRect(0);
                    break;
                case 1:
                    getRegistryValue("/CONFIG/MUSIC", "wma_play", &value);
                    setP(p0->x, 45, p0->x + strlen(wma[value]) * 8 + 20, 65);
                    drawRect(10, true, 0x0800FFFF);
                    setP0(p0->x + 10, 60);
                    drawString(wma[value], FSHADOW);
                    if (value == 0) {
                        setRegistryValue("/CONFIG/MUSIC", "wma_play", 1);
                    }
                    sceKernelDelayThread(1000000);
                    setP(p0->x, 45, p0->x + strlen(wma[value]) * 8 + 30, 75);
                    clearRect(0);
                    break;
                case 2:
                    getRegistryValue("/CONFIG/BROWSER", "flash_activated", &value);
                    setP(p0->x, 45, p0->x + strlen(flashplayer[value]) * 8 + 20, 65);
                    drawRect(10, true, 0x0800FFFF);
                    setP0(p0->x + 10, 60);
                    drawString(flashplayer[value], FSHADOW);
                    if (value == 0) {
                        setRegistryValue("/CONFIG/BROWSER", "flash_activated", 1);
                        setRegistryValue("/CONFIG/BROWSER", "flash_play", 1);
                    }
                    sceKernelDelayThread(1000000);
                    setP(p0->x, 45, p0->x + strlen(flashplayer[value]) * 8 + 30, 75);
                    clearRect(0);
                    break;
            }
        } else {
            setP(p0->x - 10, p0->y, p0->x + maxlen * 8 + 20, p0->y + menu_registryhack_s * 20 + 30);
            clearRect(0);
            return 0;
        }
    }
    return 0;
}

static int drawScroll(int min1, int min, int max, int step, int *pos, Point * p0) {
    setP(p0->x, p0->y, p0->x + 50, p0->y + 120);
    drawRect(15, true, 0x0A00FFFF);
    setP0(p0->x + 20, p0->y + 20);
    char s[64];
    sprintf(s, "%u", min);
    drawString(s, FSHADOW);
    setP0(p0->x + 20, p0->y + 110);
    sprintf(s, "%u", max);
    drawString(s, FSHADOW);
    int tpos = *pos;
    while(1) {
        setP(p0->x + 10, p0->y + 10, p0->x + 15, p0->y + 115);
        drawRect(2, true, 0);
        drawRect(2, false, 0xFFFF);
        setP(p0->x + 10, p0->y + tpos * 100 / max + 10, p0->x + 15, p0->y + tpos * 100 / max + 15);
        clearRect(0);
        setP(p0->x + 20, p0->y + 50, p0->x + 50, p0->y + 70);
        drawRect(0, true, 0);
        setP0(p0->x + 20, p0->y + 65);
        sprintf(s, "%u", tpos);
        if(tpos == 0) {
            strcpy(s, "OFF");
        }
        drawString(s, FSHADOW);
        u32 key = ctrlWaitMask(PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_LEFT | PSP_CTRL_RIGHT | PSP_CTRL_YES | PSP_CTRL_NO);
        switch(key) {
            case PSP_CTRL_LEFT:
                tpos = min1;
                break;
            case PSP_CTRL_RIGHT:
                tpos = max;
                break;
            case PSP_CTRL_DOWN:
                tpos += step;
                if(tpos > max) {
                    tpos = max;
                } else if(tpos < min) {
                    tpos = min;
                }
                break;
            case PSP_CTRL_UP:
                tpos -= step;
                if(tpos < min) {
                    tpos = min1;
                }
                break;
            case PSP_CTRL_CIRCLE:
            case PSP_CTRL_CROSS:
                setP(p0->x, p0->y, p0->x + 90, p0->y + 130);
                clearRect(0);
                if(key == PSP_CTRL_YES) {
                    *pos = tpos;
                    return 1;
                } else {
                    return 0;
                }
                break;
        }
    }
    return 0;
}

static int cpuspeed(Point * p0) {
    TNConfig config;
    sctrlSEGetConfig(&config);
    int cfw[2];
    cfw[0] = config.vshcpuspeed;
    cfw[1] = config.umdisocpuspeed;
    if(cfw[0] == 0) {
        cfw[0] = 222;
    }
    if(cfw[1] == 0) {
        cfw[1] = 222;
    }
    int pos = 0;
    int maxlen = getMaxlen(menu_cpuspeed, menu_cpuspeed_s);
    bool init = true;
    Point p;
    while(1) {
        u32 key = drawMenu(menu_cpuspeed, menu_cpuspeed_s, &pos, p0, 20, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, init);
        if(key == PSP_CTRL_YES) {
            init = false;
            p.x = p0->x + maxlen * 8 + 30; p.y = p0->y;
            drawScroll(0, 33, 333, 10, &cfw[pos], &p);
            config.vshcpuspeed = cfw[0];
            config.vshbusspeed = cfw[0]/2;
            config.umdisocpuspeed = cfw[1];
            config.umdisobusspeed = cfw[1]/2;
            sctrlSESetConfig(&config);
        } else {
            setP(p0->x - 10, p0->y, p0->x + maxlen * 8 + 20, p0->y + menu_cpuspeed_s * 20 + 30);
            clearRect(0);
            return 0;
        }
    }
}

static int settingsMenu() {
    Point p0;
    p0.x = 20; p0.y = 35;
    int pos = 0;
    int maxlen = getMaxlen(menu_settings, menu_settings_s);
    bool init = true;
    while(1) {
        u32 key = drawMenu(menu_settings, menu_settings_s, &pos, &p0, 20, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, init);
        if(pos != 2 && pos != 1) {
            setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_settings_s * 20 + 30);
            clearRect(0);
        }
        if(key == PSP_CTRL_YES) {
            init = true;
            switch(pos) {
                case 0:
                    baseconfigMenu();
                    break;
                case 1:{
                    init = false;
                    Point p;
                    p.x = p0.x + maxlen * 8 + 30; p.y = p0.y + 35;
                    cpuspeed(&p);
                    break;
                }
                case 2:{
                    init = false;
                    Point p;
                    p.x = p0.x + maxlen * 8 + 30; p.y = p0.y + 50;
                    registryhackMenu(&p);
                    break;
                }
            }
        } else {
            setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_settings_s * 20 + 30);
            clearRect(0);
            break;
        }
    }
    return 0;
}

static int showPlugins(int tpos) {
    int activated = 0;
    char *buf, *tmpbuf;
    char plugins[10][64];
    char plugin[64];
    char plugin_name[10][64];
    memset(plugins, 0, 640);
    memset(plugin_name, 0, 640);
    char * items[10];
    char * p;
    u8 status[10];
    int count = 0, i = 0, ret;
    bool change = false;
    SceUID fd = sceIoOpen(plugin_path[tpos], PSP_O_RDONLY, 0777);
    SceUID fpl = sceKernelCreateFpl("", 1, 0, 0x400, 1, NULL);
    sceKernelAllocateFpl(fpl, (void **) &buf, NULL);
    int len = sceIoRead(fd, buf, 0x400);
    tmpbuf = buf;
    if (fd >= 0) {
        for (i = 0; i < 10; i++) {
            memset(plugin, 0, 64);
            activated = 0;
            ret = ParsePluginsConfig(tmpbuf, len, plugin, &activated);
            if (ret > 3) {
                len -= ret;
                tmpbuf += ret;
                status[i] = activated;
                strcpy(plugins[i], plugin);
                p = strrchr(plugins[i], '/');
                if (p) {
                    strcpy(plugin_name[i], p + 1);
                }
                count ++;
            } else
                break;
        }
        sceIoClose(fd);
    }
    if (buf) {
        sceKernelFreeFpl(fpl, buf);
        sceKernelDeleteFpl(fpl);
    }
    if(fd < 0 || count < 1) {
        setP(120, 100, 360, 170);
        drawRect(15, true, 0x0A00FFFF);
        setP0(150, 130);
        drawString("  No plugin found!", FSHADOW);
        sceKernelDelayThread(1000000);
        setP(120, 100, 370, 180);
        clearRect(0);
        return 0;
    }
    for(i = 0; i < count; i++) {
        items[i] = plugin_name[i];
    }
    Point p0;
    p0.x = 30; p0.y = 35;
    int pos = 0;
    setP(10 , 35, 370, 225);
    drawRect(15, true, 0x0A00FFFF);
    setP(p0.x - 4, p0.y + 12, p0.x + strlen(items[0]) * 8 + 4, p0.y + 28);
    drawRect(0, false, 0);
    for(i = 0; i < count; i ++) {
        setP0(p0.x, p0.y + 18 * i + 23);
        drawString(items[i], FSHADOW);
        setP0(274, p0.y + 18 * i + 23);
        drawString(enable_disable[status[i]], FSHADOW);
    }
    while(1) {
        u32 key = drawMenu((const char **)items, count, &pos, &p0, 18, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, false);
        if(key == PSP_CTRL_YES) {
            setP(270, p0.y + 18 * pos + 12, 280 + strlen(enable_disable[status[pos]]) * 8, p0.y + 20 * pos + 28);
            status[pos] = 1 - status[pos];
            drawRect(0, true, 0);
            setP0(274, p0.y + 18 * pos + 23);
            drawString(enable_disable[status[pos]], FSHADOW);
            change = true;
        } else {
            if(change) {
                sceIoRemove(plugin_path[tpos]);
                fd = sceIoOpen(plugin_path[tpos], PSP_O_WRONLY | PSP_O_CREAT, 0777);
                if(fd >= 0) {
                    for(i = 0; i < count; i++) {
                        SetPluginsConfig(plugins[i], status[i]);
                        sceIoWrite(fd, plugins[i], strlen(plugins[i]));
                    }
                    sceIoClose(fd);
                }
            }
            setP(10 , 35, 380, 235);
            clearRect(0);
            return 0;
        }
    }
    return 0;
}

static int usbMenu() {
    Point p0, p;
    p0.x = 168; p0.y = 35;
    int pos = 0;
    int maxlen = getMaxlen(menu_toggleusb, menu_toggleusb_s);
    bool init = true;
    while(1) {
        u32 key = drawMenu(menu_toggleusb, menu_toggleusb_s, &pos, &p0, 20, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, init);
        p.x = p0.x + maxlen * 8 + 30; p.y = p0.y + pos * 20 + 10;
        if(key == PSP_CTRL_YES) {
            switch(pos) {
                case 0:
                    pspUsbDeviceFinishDevice();
                    sceUsbstorBootSetCapacity(0x800000);
                    setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_toggleusb_s * 20 + 30);
                    clearRect(0);
                    return 0;
                case 1:
                    pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH0, 0, 0);
                    sceUsbstorBootSetCapacity(0x800000);
                    setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_toggleusb_s * 20 + 30);
                    clearRect(0);
                    return 0;
                case 2:
                    pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH1, 0, 0);
                    sceUsbstorBootSetCapacity(0x800000);
                    setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_toggleusb_s * 20 + 30);
                    clearRect(0);
                    return 0;
                case 3:
                    pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH2, 0, 0);
                    sceUsbstorBootSetCapacity(0x800000);
                    setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_toggleusb_s * 20 + 30);
                    clearRect(0);
                    return 0;
                case 4:
                    pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH3, 0, 0);
                    sceUsbstorBootSetCapacity(0x800000);
                    setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_toggleusb_s * 20 + 30);
                    clearRect(0);
                    return 0;
                case 5:
                    pspUsbDeviceSetDevice(PSP_USBDEVICE_UMD9660, 0, 0);
                    sceUsbstorBootSetCapacity(0x800000);
                    setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_toggleusb_s * 20 + 30);
                    clearRect(0);
                    return 0;
            }
        } else {
            setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_toggleusb_s * 20 + 30);
            clearRect(0);
            return 0;
        }
    }
    return 0;
}

static int pluginMenu() {
    Point p0, p;
    p0.x = 104; p0.y = 35;
    int pos = 0;
    int maxlen = getMaxlen(menu_plugin, menu_plugin_s);
    bool init = true;
    while(1) {
        u32 key = drawMenu(menu_plugin, menu_plugin_s, &pos, &p0, 20, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, init);
        p.x = p0.x + maxlen * 8 + 30; p.y = p0.y + pos * 20 + 10;
        if(key == PSP_CTRL_YES) {
            init = false;
            if(pos >= 0) {
                init = true;
                setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_plugin_s * 20 + 30);
                clearRect(0);
                showPlugins(pos);
            }
        } else {
            setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_plugin_s * 20 + 30);
            clearRect(0);
            return 0;
        }
    }
    return 0;
}

static int powerMenu() {
    Point p0;
    p0.x = 268; p0.y = 35;
    int pos = 0;
    int maxlen = getMaxlen(menu_power, menu_power_s);
    while(1) {
        u32 key = drawMenu(menu_power, menu_power_s, &pos, &p0, 20, PSP_CTRL_UP | PSP_CTRL_DOWN | PSP_CTRL_YES | PSP_CTRL_NO, true);
        setP(p0.x - 10, p0.y, p0.x + maxlen * 8 + 20, p0.y + menu_power_s * 20 + 30);
        clearRect(0);
        if(key == PSP_CTRL_YES) {
            switch(pos) {
                case 0:
                    scePowerRequestSuspend();
                    return 1;
                case 1:
                    scePowerRequestStandby();
                    return 1;
                case 2:
                    scePowerRequestColdReset(0);
                    return 1;
                case 3:
                    sctrlKernelExitVSH(0);
                    return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}

static void uiInfo() {
	unsigned short pw;
	if(scePowerIsBatteryExist())
		pw = scePowerGetBatteryLifePercent();
	else pw = 0;
    setP(380, 7, 405, 17);
    drawRect(2, false, 0x0000FFFF);
    u32 tmp = getGcolor(0);
    setGcolor(0xA06049EF, 0);
    setP1(pw * 20 / 100 + 380, 17);
    drawRect(2, false, 0x0000FFFF);
    setGcolor(tmp, 0);
	char s[16];
	sprintf(s, "%u%%", pw);
    setP0(410, 15);
    drawString(s, FTHICK | FSHADOW);
    setP0(440, 15);
	if(!scePowerIsBatteryExist()) {
		drawString("X", FTHICK | FSHADOW);
	} else {
		if(scePowerIsBatteryCharging())
			drawString("C", FTHICK | FSHADOW);
		if(scePowerIsLowBattery())
			drawString("L", FTHICK | FSHADOW);
	}
	unsigned short cpu = scePowerGetCpuClockFrequency();
	unsigned short bus = scePowerGetBusClockFrequency();
	sprintf(s, "%u/%u", cpu, bus);
    setP0(160, 255);
    drawString(s, FTHICK | FSHADOW);
}

extern int mainMenu() {
    u32 value = 1;
    getRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", &value);
    if(value == 0) {
        PSP_CTRL_YES = PSP_CTRL_CIRCLE;
        PSP_CTRL_NO = PSP_CTRL_CROSS;
    }

    SceSysTimerId timer;
    timer = sceSTimerAlloc();
    sceSTimerStartCount(timer);
    sceSTimerSetHandler(timer, 799999, tickHandler, 0);

    setP(145, 240, 479, 260);
    drawRect(10, true, 0x0800F0F0);
    setP0(335, 255);
    static char g_verinfo[] = "6.20 TN- (HEN)";
    g_verinfo[8] = (sctrlHENGetVersion() & 0xF) + 0x41;
    drawString(g_verinfo, FTHICK | FSHADOW);

    setP(5, 0, 474, 25);
    drawRect(15, true, 0x0A0000FF);

    uiInfo();
    int i;
    int lp[] = {24, 104, 175, 268};
    char ** items = (char **)menu_main3xx;
    u8 items_s = menu_main3xx_s;
    int lpos = 0, pos = 0;
    for(i = 0; i < items_s; i ++) {
        if(i == pos) {
            setP(lp[i] - 4, 0, lp[i] + strlen(items[i]) * 9 + 4, 25);
            drawRect(0, false, 0);
            setP0(lp[i], 15);
            drawString(items[i], FTHICK | FSHADOW);
        } else {
            setP0(lp[i], 15);
            drawString(items[i], FTHICK | FSHADOW);
        }
    }
    while(1) {
        lpos = pos;
        u32 key = ctrlWaitMask(PSP_CTRL_LEFT | PSP_CTRL_RIGHT | PSP_CTRL_YES | PSP_CTRL_NO);
        switch(key) {
            case PSP_CTRL_LEFT:
                pos --;
                if(pos < 0) {
                    pos = items_s - 1;
                }
                break;
            case PSP_CTRL_RIGHT:
                pos ++;
                if(pos >= items_s) {
                    pos = 0;
                }
                break;
            case PSP_CTRL_CROSS:
            case PSP_CTRL_CIRCLE:
                if(key == PSP_CTRL_YES) {
                    switch(pos) {
                        case 0:
                            settingsMenu();
                            break;
                        case 1:
                            pluginMenu();
                            break;
                        case 2:
                            usbMenu();
                            break;
                        case 3:
                            if(powerMenu()) {
                                sceSTimerStopCount(timer);
                                sceSTimerFree(timer);
                                return 0;
                            }
                            break;
                    }
                } else {
                    sceSTimerStopCount(timer);
                    sceSTimerFree(timer);
                    return 0;
                }
                break;
        }
        if(lpos != pos) {
            setP(lp[lpos] - 4, 0, lp[lpos] + strlen(items[lpos]) * 9 + 4, 25);
            drawRect(0, true, 0);
            setP0(lp[lpos], 15);
            drawString(items[lpos], FTHICK | FSHADOW);
            setP(lp[pos] - 4, 0, lp[pos] + strlen(items[pos]) * 9 + 4, 25);
            drawRect(0, true, 0);
            drawRect(0, false, 0);
            setP0(lp[pos], 15);
            drawString(items[pos], FTHICK | FSHADOW);
        }
    }
    sceSTimerStopCount(timer);
    sceSTimerFree(timer);
    return 0;
}
