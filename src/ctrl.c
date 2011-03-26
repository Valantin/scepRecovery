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
#include "ctrl.h"
/*
#define CTRL_REPEAT_TIME 0x40000
#define CTRL_REPEAT_INTERVAL 0x12000

static u32 last_btn = 0;
static u32 last_tick = 0;
static u32 repeat_flag = 0;

static u32 ctrlRead() {
    SceCtrlData ctl;
    sceCtrlPeekBufferPositive(&ctl, 1);

    if (ctl.Buttons == last_btn) {
        if (ctl.TimeStamp - last_tick < (repeat_flag ? CTRL_REPEAT_INTERVAL : CTRL_REPEAT_TIME)) return 0;
        repeat_flag = 1;
        last_tick = ctl.TimeStamp;
        return last_btn;
    }
    repeat_flag = 0;
    last_tick = ctl.TimeStamp;
    last_btn  = ctl.Buttons;
    return last_btn;
}*/

extern u32 ctrlWaitMask(u32 keymask) {
    u32 key;
    while((key = (g_buttons_on & keymask)) == 0) {
        sceKernelDelayThread(20000);
    }
    return key;
}
