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
#ifndef _SCEP_RECOVERY_UTIL_
#define _SCEP_RECOVERY_UTIL_

#include <pspsdk.h>

extern int getMaxlen(const char ** item, int item_num);

extern int ParsePluginsConfig(char *buf, int len, char *path, int *active);

extern void SetPluginsConfig(char *str, int activated);

extern int loadstartModule(char * path);

#endif
