/***************************************************************************
    pongian - a free ping pong clone written in c++/sdl
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
* ball.h - structures and so on for ball-related stuff                     *
****************************************************************************/

#ifndef __PONGIAN_BALL_H_
#define __PONGIAN_BALL_H_

#include <SDL/SDL.h>
#include "pongian.hxx"

#define BALL_VER "v0.3"

class ball
{
	public:
	 ball(void);
	~ball(void);

	 void Draw(char state);
	 void DrawShadow(char state);
	 void Move(void);
	 void Do(void);
	 void calcBallCrossing(void);

	 float movex, movey;
	 SDL_Surface *ballSkin;
	 SDL_Rect ballRect, ballShadRect;
	 SDL_Rect willCross;
	 char crossings;

	private:
	 char oldplayer;
};


#endif
