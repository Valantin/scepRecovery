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
#ifndef _SCEP_RECOVERY_CTRL_
#define _SCEP_RECOVERY_CTRL_
#include <pspsdk.h>
#include <pspctrl.h>

/**
 * wait a key mask
 *
 * @param keymask - The keymask to be waited.
 *
 * @returns a key mask pressed
*/
extern u32 ctrlWaitMask(u32 keymask);

#endif
