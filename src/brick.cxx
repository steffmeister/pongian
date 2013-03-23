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
* brick.cpp - brick-related stuff                                          *
*             please note that this is not totally implented!              *
****************************************************************************/

#include "brick.hxx"

brick :: brick(void)
{
	nextBrick = NULL;
	firstBrick = NULL;
	xpos = 0;
	ypos = 0;
	skin = 0;
	powerUp = 0;
	timeToLive = 0;
	Id = 0;
}

brick :: ~brick(void)
{}

brick* brick :: addBrick(void)
{
	brick* retBrick = firstBrick;
	while(retBrick->nextBrick != NULL) retBrick = retBrick->nextBrick;
	retBrick->nextBrick = new brick;
	retBrick->Id = Id++;
	return retBrick->nextBrick;
}

void brick :: delBrick(char delId)
{
	brick* cntBrick = firstBrick;
	brick* tmpBrick;
	if (cntBrick->nextBrick != NULL) 
	{
 		while(cntBrick->nextBrick->Id != delId) cntBrick = cntBrick->nextBrick;
 		tmpBrick = cntBrick->nextBrick->nextBrick;
		delete(cntBrick->nextBrick);
		cntBrick->nextBrick = tmpBrick;
		while(tmpBrick->nextBrick != NULL)
		{
			tmpBrick->Id--;
			tmpBrick = tmpBrick->nextBrick;
		}
	}
}
