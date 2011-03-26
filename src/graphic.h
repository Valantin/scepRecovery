/*	
	SCEP-Series Graphic engine

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
#ifndef _SCEP_GRAPHIC_
#define _SCEP_GRAPHIC_

#include <pspsdk.h>

typedef enum _bool
{
	true = 1,
	false = 0
} bool;

typedef struct _point
{
	u16 x;
	u16 y;
} Point;

enum ScreenParam
{
	SCREEN_WIDTH = 480,
	SCREEN_HEIGHT = 272
};

enum RectCorner
{
	CORNER_LT = 0x0000F000,
	CORNER_RT = 0x00000F00,
	CORNER_LB = 0x000000F0,
	CORNER_RB = 0x0000000F
};

enum FontStyle
{
	FNORMAL = 0x0000,
	FSHADOW = 0x000F,
	FTHICK = 0x00F0,
	FENDLINE = 0x0F00
};

enum DrawCharError
{
	ENDPAGE = 0x00,
	ENDLINE = 0x0F,
	NEWLINE = 0xF0,
	DRAWED = 0xFF
};

extern void setP0(u16 x, u16 y);

extern void setP1(u16 x, u16 y);

extern void setP(u16 x0, u16 y0, u16 x1, u16 y1);

extern void setGcolor(u32 color, int offset);

extern u32 getGcolor(int offset);

/*	refresh the screen */
//extern void refresh();

/*	init Graphic so that you can draw with it
	@returns true on succuss, or false on error*/
extern bool initGraphic(SceUID id, void * gvram);

/*	restore the screen with a screen buffer 
	stored while Grphic Engine init */
extern void restore();

/*	free Graphic */
extern void freeGraphic();


extern void drawLine();

extern void drawRect_p(int r, bool clear, u32 flag, Point * pt0, Point * pt1);

extern void drawRect(int r, bool clear, u32 flag);

extern void clearRect(u32 color);

extern int drawString_p(const char * txt, u16 flag, Point * pt0);

extern int drawString(const char * txt, u16 flag);

extern void blur();

#endif
