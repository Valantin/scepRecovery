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
#include "registry.h"
#include <pspreg.h>
#include <string.h>

extern int getRegistryValue(const char *dir, const char *name, u32 *val) {
    int ret = 0;
    struct RegParam reg;
    REGHANDLE h;

    memset(&reg, 0, sizeof(reg));
    reg.regtype = 1;
    reg.namelen = strlen("/system");
    reg.unk2 = 1;
    reg.unk3 = 1;
    strcpy(reg.name, "/system");
    if(sceRegOpenRegistry(&reg, 2, &h) == 0) {
        REGHANDLE hd;
        if(!sceRegOpenCategory(h, dir, 2, &hd)) {
            REGHANDLE hk;
            unsigned int type, size;

            if(!sceRegGetKeyInfo(hd, name, &hk, &type, &size)) {
                if(!sceRegGetKeyValue(hd, hk, val, 4)) {
                    ret = 1;
                    sceRegFlushCategory(hd);
                }
            }
            sceRegCloseCategory(hd);
        }
        sceRegFlushRegistry(h);
        sceRegCloseRegistry(h);
    }

    return ret;
}

extern int setRegistryValue(const char *dir, const char *name, u32 val) {
    int ret = 0;
    struct RegParam reg;
    REGHANDLE h;

    memset(&reg, 0, sizeof(reg));
    reg.regtype = 1;
    reg.namelen = strlen("/system");
    reg.unk2 = 1;
    reg.unk3 = 1;
    strcpy(reg.name, "/system");
    if(sceRegOpenRegistry(&reg, 2, &h) == 0) {
        REGHANDLE hd;
        if(!sceRegOpenCategory(h, dir, 2, &hd)) {
            if(!sceRegSetKeyValue(hd, name, &val, 4)) {
                ret = 1;
                sceRegFlushCategory(hd);
            } else {
                sceRegCreateKey(hd, name, REG_TYPE_INT, 4);
                sceRegSetKeyValue(hd, name, &val, 4);
                ret = 1;
                sceRegFlushCategory(hd);
            }
            sceRegCloseCategory(hd);
        }
        sceRegFlushRegistry(h);
        sceRegCloseRegistry(h);
    }

    return ret;
}
