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
#include "menu.h"
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspsysmem_kernel.h>
#include <systemctrl.h>
#include <string.h>

PSP_MODULE_INFO("scepVshMenu", 0, 1, 0);
static SceUID thid;

static int vsh_menu_ctrl(SceCtrlData *pad_data, int count) {
    static SceCtrlData g_pad_data = {0};

    int i;

    /*scePaf_*/memcpy(&g_pad_data, pad_data, sizeof(SceCtrlData));
    g_buttons_on = (~g_cur_buttons) & g_pad_data.Buttons;
    g_cur_buttons = g_pad_data.Buttons;

    for (i = 0; i < count; i++)
        pad_data[i].Buttons &= ~ALL_KEYS;

    return 0;
}

int main_thread(SceSize args, void *argp) {
    sceKernelChangeThreadPriority(0, 0x8);
    vctrlVSHRegisterVshMenu(vsh_menu_ctrl);
    void * vram;
    int bufferwidth = 512, pixelformat = PSP_DISPLAY_PIXEL_FORMAT_8888, unk = 1;
    //vram = (void *) (0x40000000 | 0x04000000);
    sceDisplayGetFrameBuf(&vram, &bufferwidth, &pixelformat, unk);
    vram = (void *) ((u32)vram | 0x40000000);
    sceDisplaySetMode(0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceDisplaySetFrameBuf((void *)vram, bufferwidth, pixelformat, unk);
    sceKernelSetDdrMemoryProtection((void *)0x88400000, 0x00400000, 0xF);
    SceUID heapid = sceKernelCreateHeap(5, 1024 * 1024, 1, "graphic_heap");
    if(heapid >= 0 && initGraphic(heapid, vram)) {
        mainMenu();
        freeGraphic();
        sceKernelDeleteHeap(heapid);
    }
    //vctrlVSHExitVSHMenu(&config);
    return sceKernelExitDeleteThread(0);
}

int module_start(SceSize args, void *argp) {
    thid = sceKernelCreateThread("VshMenu_Thread", main_thread, 0x10, 0x1000, 0, NULL);
	if(thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }
    return 0;
}

int module_stop(SceSize args, void *argp) {
    SceUInt timeout = 100000;
    if (sceKernelWaitThreadEnd(thid, &timeout) < 0)
        sceKernelTerminateDeleteThread(thid);
    return 0;
}
