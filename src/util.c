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
#include "util.h"

#include <string.h>

extern int getMaxlen(const char ** item, int item_num) {
    int i, maxlen = 0;
    for(i = 0; i < item_num; i ++) {
        if(strlen(item[i]) > maxlen) {
            maxlen = strlen(item[i]);
        }
    }
    return maxlen;
}

void trim(char *buf) {
    char *s = buf + strlen(buf) - 1;

    while (s >= buf && (*s == ' ' || *s == '\t')) {
        *s = '\0';
        s--;
    }
}

extern int ParsePluginsConfig(char *buf, int len, char *path, int *active) {
    int i, j;
    char c, *p, *s;

    p = path;
    for (i = 0, j = 0; i < len; i++) {
        c = buf[i];
        if (c >= ' ' || c == '\t') {
            *p = c;
            p++;
            j++;
        } else {
            if (j != 0)
                break;
        }
    }
    trim(path);
    *active = 0;

    if (i > 0) {
        if ((p = strpbrk(path, " \t"))) {
            s = p;
            p++;
            while (*p < 0)
                p++;
            *active = (strcmp(p, "1") == 0);
            *s = 0;
        }
    }

    return i;
}

extern void SetPluginsConfig(char *str, int activated) {
    char *s = str;
    if(activated == 0)
        strcat(s," 0\n");
    else
        strcat(s," 1\n");
}

extern int loadstartModule(char * path) {
    SceUID mod = sceKernelLoadModule(path, 0, NULL);
    if(mod >= 0)
        mod = sceKernelStartModule(mod, strlen(path)+1, path, NULL, NULL);
    return mod;
}