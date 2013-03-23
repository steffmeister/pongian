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
* paddle.h - paddle related defines and the paddle class                   *
****************************************************************************/

#ifndef __PONGIAN_PADDLE_H_
#define __PONGIAN_PADDLE_H_

#include <SDL/SDL.h>
#include "pongian.hxx"

#define PADDLE_VER "v0.2"
#define CPU_PADDLE_VER "v0.1"

class brett
{
	public:
	 brett(char who);
	~brett(void);

	 void Draw(char state);
	 void DrawShadow(char state);
	 void Move(char moven);
	 void control(void);
	 char move;
	 SDL_Surface *playerSkin;
	 SDL_Rect playerRect, playerShadRect;

	protected:

	private:
		char con;
};

#endif

