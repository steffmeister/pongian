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
* ball.cpp - abrs (advanced ball reflection system) and other              *
*            ball-related stuff.                                           *
****************************************************************************/

#include "ball.hxx"

ball :: ball(void)
{
	ballRect.x = 0;
	ballRect.y = 0;
	ballRect.w = pongian.diameter;
	ballRect.h = pongian.diameter;
	ballShadRect = ballRect;
}

ball :: ~ball(void)
{ }

void ball :: DrawShadow(char state)
{
	if (pongian.ballShadow == TRUE)
	{
		if (state == TRUE)
		{
			ballShadRect.x = ballRect.x + SHADOW_X;
			ballShadRect.y = ballRect.y + SHADOW_Y;
			SDL_BlitSurface(xtheme.daballShad, NULL, screen, &ballShadRect);

		}
		if (state == FALSE)
		{
			ballShadRect.x = ballRect.x + SHADOW_X;
			ballShadRect.y = ballRect.y + SHADOW_Y;
			SDL_BlitSurface(xtheme.game_background, &ballShadRect, screen, &ballShadRect);
		}
	}
}

void ball :: Draw(char state)
{
	if (state == TRUE) 
	{
		SDL_BlitSurface(xtheme.daballSkin, NULL, screen, &ballRect);
	}
	if (state == FALSE) 
	{
		DrawShadow(FALSE);
		SDL_BlitSurface(xtheme.game_background, &ballRect, screen, &ballRect);
	}
}

void ball :: Do(void)
{
	if (oldplayer != pongian.actualPlayer)
	{
		if ((oldplayer == 1) && (pongian.actualPlayer == 0))
		{
			movey = -DEFAULT_BALL_SPEED;
			movex = DEFAULT_BALL_SPEED;
//			if (pongian.settings.soundEnabled) PlaySound(&xtheme.shotSND);
		}

		if ((oldplayer == 2) && (pongian.actualPlayer == 0))
		{
			movey = DEFAULT_BALL_SPEED;
			movex = -DEFAULT_BALL_SPEED;
//			if (pongian.settings.soundEnabled) PlaySound(&xtheme.shotSND);
		}
		oldplayer = pongian.actualPlayer;

	}

	if (pongian.actualPlayer == 1)
	{
		ballRect.x = player1.playerRect.x + (pongian.length/2) - pongian.diameter/2;
		ballRect.y = player1.playerRect.y - pongian.diameter;
	}

	if (pongian.actualPlayer == 2)
	{
		ballRect.x = player2.playerRect.x + (pongian.length/2) - pongian.diameter/2;
		ballRect.y = player2.playerRect.y + pongian.height;
	}

	if (pongian.actualPlayer == 0) Move();
}

void ball :: Move(void)
{
	ballRect.x += (char)movex;
	ballRect.y += (char)movey;
	if ((ballRect.x + pongian.diameter) >= pongian.settings.x)
	{
		movex = -movex;
//		if (pongian.settings.soundEnabled) PlaySound(&xtheme.wallbangSND);
		ballRect.x = pongian.settings.x - pongian.diameter - 1;
	}
		
	if (ballRect.x <= 0)
	{
		movex = -movex;
//		if (pongian.settings.soundEnabled) PlaySound(&xtheme.wallbangSND);
		ballRect.x = 1;
	}
	
	if (((ballRect.x >= (player1.playerRect.x - pongian.diameter/2)) &&
	   (ballRect.x <= (player1.playerRect.x + pongian.length + pongian.diameter/2)) &&
	   (ballRect.y >= (player1.playerRect.y - pongian.diameter))) && (movey > 0))
	{
//		if (pongian.settings.soundEnabled) PlaySound(&xtheme.paddlebangSND);
		if (ballRect.x > (player1.playerRect.x + pongian.length/2))
		{
			movex += 1 + (ballRect.x - (player1.playerRect.x + pongian.length/2)) / 10;
		} else
		{
			movex -= -1 - (ballRect.x - (player1.playerRect.x + pongian.length/2)) / 10;
		}
		if (player1.move == RIGHT) movex += 0.47;
		if (player1.move == LEFT) movex -= 0.47;
		movey = -movey; /* this works! */
		ballRect.y = player1.playerRect.y - pongian.diameter;
	}
	if (((ballRect.x >= (player2.playerRect.x - pongian.diameter/2)) &&
	   (ballRect.x <= (player2.playerRect.x + pongian.length + pongian.diameter/2)) &&
	   (ballRect.y <= (player2.playerRect.y + pongian.height))) && (movey < 0))
	{
//		if (pongian.settings.soundEnabled) PlaySound(&xtheme.paddlebangSND);
		if (ballRect.x > (player2.playerRect.x + pongian.length /2))
		{
			movex += 1 + (ballRect.x - (player2.playerRect.x + pongian.length/2)) / 10;
		} else
		{
			movex -= -1 - (ballRect.x - (player2.playerRect.x + pongian.length/2)) / 10;
		}
		if (player2.move == RIGHT) movex += 0.47;
		if (player2.move == LEFT) movex -= 0.47;
		movey = -movey; /* this works! */
		ballRect.y = player2.playerRect.y + pongian.height;
	}
}

void ball :: calcBallCrossing(void)
{
	float xmovex = movex, xmovey = movey;
	crossings = 0;
	willCross = ball::ballRect;
	while (willCross.y >= pongian.height)
	{
		willCross.x += (char)xmovex;
		willCross.y += (char)xmovey;
		if ((willCross.x > (pongian.settings.x - pongian.diameter)) || (willCross.x <= 1))
		{
			xmovex = -xmovex;
			crossings++;
		}
	}
	if (movey > 0) crossings = -1;
}
