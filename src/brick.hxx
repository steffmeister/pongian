/***************************************************************************
    pongian - a free ping pong clone written in sdl
    Copyright (C) 2004 Stefan Zidar

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to Free Software 
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    
    Stefan Zidar
    AUSTRIA
    steffed@gmx.net
****************************************************************************

****************************************************************************
* brick.h - brick stuff <- not implented yet!                              *
****************************************************************************/

#ifndef __PONGIAN_BRICK_H_
#define __PONGIAN_BRICK_H_

#include "pongian.hxx"

class brick
{
	public:
		brick(void);
		~brick(void);
		brick* addBrick(void);
		void delBrick(char delId);
		brick* nextBrick;
		brick* firstBrick;
	private:
		char xpos;
		char ypos;
		char skin;
		char powerUp;
		char timeToLive;
		char Id;
};

#endif
