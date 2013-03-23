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
* paddle.cpp - contains paddle (human + computer controlled players)       *
*              related stuff.                                              *
****************************************************************************/

#include "paddle.hxx"

brett :: brett(char who)
{
	playerRect.x = 0;
	playerRect.y = 0;
	playerRect.w = pongian.length;
	playerRect.h = pongian.height;
	con = who;
	playerSkin = NULL;
	playerShadRect = playerRect;
}

brett :: ~brett(void)
{}

void brett :: Move(char moven)
{
	move = moven;

	switch(moven)
	{
		case LEFT:
			if ((playerRect.x - DEFAULT_SPEED) >= 0) 
			{
				playerRect.x -= (int)DEFAULT_SPEED;
			} else
			{
				playerRect.x = 0;
				move = STOP;
			}
		break;

		case RIGHT:
			if ((playerRect.x + DEFAULT_SPEED) <= (pongian.settings.x - pongian.length))
			{
				playerRect.x += (int)DEFAULT_SPEED;
			} else
			{
				playerRect.x = pongian.settings.x - pongian.length;
				move = STOP;
			}
		break;
	}
}

void brett :: control(void)
{
	SDL_PollEvent(&event);
	if (con == 1)
	{
		if ((pongian.settings.p1ctrl == KBD1) || (pongian.settings.p1ctrl == KBD2))
		{
			if ((event.key.keysym.sym == pongian.p1Up) && (pongian.actualPlayer == 1))
			{
				pongian.actualPlayer = 0;
			} else if (event.key.keysym.sym == pongian.p1Left)
			{
				move = LEFT;
			} else if (event.key.keysym.sym == pongian.p1Right)
			{
				move = RIGHT;
			} else if (event.key.keysym.sym == pongian.p1Down)
			{
				move = STOP;
			}
		} else if (pongian.settings.p1ctrl == MOUSE)
		{
			if (event.button.type == SDL_MOUSEBUTTONDOWN)
			{
				if ((event.button.button == 1) && (event.button.state == SDL_PRESSED) && (pongian.actualPlayer == 1))
				{
					pongian.actualPlayer = 0;
				}
				if ((event.button.button == 2) && (event.button.state == SDL_PRESSED))
				{
					move = STOP;
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				if (event.motion.xrel < 0)
				{
					move = LEFT;
				}
				if (event.motion.xrel > 0)	
				{
					move = RIGHT;
				}
			}
		} else if (pongian.settings.p1ctrl == JOYSTICK)
		{
			if (event.type == SDL_JOYAXISMOTION)
			{
				if ((event.jaxis.axis == 0) && (event.jaxis.value < -3200)) move = LEFT;
				if ((event.jaxis.axis == 0) && (event.jaxis.value > 3200)) move = RIGHT;
			}
			if (event.type == SDL_JOYBUTTONDOWN)
			{
				if (event.jbutton.button == 0) pongian.actualPlayer = 0;
				if (event.jbutton.button == 1) move = STOP;
			}
		}
	}

	if (con == 2)
	{
		if (pongian.cpuPaddleEnabled == 0)
		{
			if ((pongian.settings.p2ctrl == KBD1) || (pongian.settings.p2ctrl == KBD2))
			{
				if ((event.key.keysym.sym == pongian.p2Up) && (pongian.actualPlayer == 2))
				{
					pongian.actualPlayer = 0;
				} else if (event.key.keysym.sym == pongian.p2Left)
				{
					move = LEFT;
				} else if (event.key.keysym.sym == pongian.p2Right)
				{
					move = RIGHT;
				} else if (event.key.keysym.sym == pongian.p2Down)
				{
					move = STOP;
				}
			} else if (pongian.settings.p2ctrl == MOUSE)
			{
				if (event.button.type == SDL_MOUSEBUTTONDOWN)
				{
					if ((event.button.button == 1) && (event.button.state == SDL_PRESSED) && (pongian.actualPlayer == 2))
					{
						pongian.actualPlayer = 0;
					}
					if ((event.button.button == 2) && (event.button.state == SDL_PRESSED))
					{
						move = STOP;
					}
				}
				if (event.type == SDL_MOUSEMOTION)
				{
					if (event.motion.xrel < 0)
					{
						move = LEFT;
					}
					if (event.motion.xrel > 0)	
					{
						move = RIGHT;
					}
				}
			}
		} else if (pongian.settings.p1ctrl == JOYSTICK)
		{
			if (event.type == SDL_JOYAXISMOTION)
			{
				if ((event.jaxis.axis == 0) && (event.jaxis.value < -3200)) move = LEFT;
				if ((event.jaxis.axis == 0) && (event.jaxis.value > 3200)) move = RIGHT;
			}
			if (event.type == SDL_JOYBUTTONDOWN)
			{
				if (event.jbutton.button == 0) pongian.actualPlayer = 0;
				if (event.jbutton.button == 1) move = STOP;
			}
		} else if (pongian.cpuPaddleEnabled == 1)
		{
			if (pongian.actualPlayer == con)
			{
				pongian.actualPlayer = 0;
			} else if (kugel.ballRect.x > (playerRect.x + pongian.length))
			{
				move = RIGHT;
			} else if (kugel.ballRect.x < playerRect.x)
			{
				move = LEFT;
			} else if ((kugel.ballRect.x > playerRect.x) && (kugel.ballRect.x < (playerRect.x + pongian.length)))
			{
				move = STOP;
			}
		} else if (pongian.cpuPaddleEnabled == 2)
		{
			if (pongian.actualPlayer == con)
			{
				pongian.actualPlayer = 0;
			} else if (kugel.movey < 0)
			{
				kugel.calcBallCrossing();
				if ((kugel.crossings == 0) && (kugel.ballRect.y < (pongian.settings.y / 3)))
				{
					if (kugel.willCross.x < playerRect.x)
					{
						move = LEFT;
					} else if (kugel.willCross.x > (playerRect.x + pongian.length))
					{
						move = RIGHT;
					} else if ((kugel.ballRect.x > playerRect.x) && (kugel.ballRect.x < (playerRect.x + pongian.length)))
					{
						move = STOP;
					}
				} else
				{
					if (kugel.ballRect.x > (playerRect.x + pongian.length))
					{
						move = RIGHT;
					} else if (kugel.ballRect.x < playerRect.x)
					{
						move = LEFT;
					} else if ((kugel.ballRect.x > playerRect.x) && (kugel.ballRect.x < (playerRect.x + pongian.length)))
					{
						move = STOP;
					}
				}
			}
		} else if (pongian.cpuPaddleEnabled == 3)
		{
			if (pongian.actualPlayer == con)
			{
				pongian.actualPlayer = 0;
			} else if (kugel.movey < 0)
			{
				kugel.calcBallCrossing();
				if (kugel.crossings == 0)
				{
					if (kugel.willCross.x < playerRect.x)
					{
						move = LEFT;
					} else if (kugel.willCross.x > (playerRect.x + pongian.length))
					{
						move = RIGHT;
					} else if ((kugel.ballRect.x > playerRect.x) && (kugel.ballRect.x < (playerRect.x + pongian.length)))
					{
						move = STOP;
					}
				} else
				{
					if (kugel.ballRect.x > (playerRect.x + pongian.length))
					{
						move = RIGHT;
					} else if (kugel.ballRect.x < playerRect.x)
					{
						move = LEFT;
					} else if ((kugel.ballRect.x > playerRect.x) && (kugel.ballRect.x < (playerRect.x + pongian.length)))
					{
						move = STOP;
					}
				}
			}
		} else if (pongian.cpuPaddleEnabled == 4)
		{
			if (pongian.actualPlayer == con)
			{
				pongian.actualPlayer = 0;
			} else if (kugel.movey < 0)
			{
				kugel.calcBallCrossing();
				if (kugel.willCross.x < playerRect.x)
				{
					move = LEFT;
				} else if (kugel.willCross.x > (playerRect.x + pongian.length))
				{
					move = RIGHT;
				} else if ((kugel.ballRect.x > playerRect.x) && (kugel.ballRect.x < (playerRect.x + pongian.length)))
				{
					move = STOP;
				}
			}
		}
	}
	Move(move);
}

void brett :: DrawShadow(char state)
{
	if (pongian.paddleShadow == TRUE)
	{
		if (state == TRUE)
		{
			if (pongian.paddleShadow == TRUE)
			{
				playerShadRect.x = playerRect.x + SHADOW_X;
				playerShadRect.y = playerRect.y + SHADOW_Y;
				SDL_BlitSurface(xtheme.daplayerShad, NULL, screen, &playerShadRect);
			}
		}
		if (state == FALSE)
		{
			if (pongian.paddleShadow == TRUE)
			{
				playerShadRect.x = playerRect.x + SHADOW_X;
				playerShadRect.y = playerRect.y + SHADOW_Y;
				SDL_BlitSurface(xtheme.game_background, &playerShadRect, screen, &playerShadRect);
			}
		}
	}
}

void brett :: Draw(char state)
{
	if (playerSkin == NULL)
	{
		if (con == 1) playerSkin = xtheme.daplayer1Skin;
		if (con == 2) playerSkin = xtheme.daplayer2Skin;
	}
	if (state == TRUE) 
	{
		SDL_BlitSurface(playerSkin, NULL, screen, &playerRect);
	}
	if (state == FALSE) 
	{
		DrawShadow(FALSE);
		SDL_BlitSurface(xtheme.game_background, &playerRect, screen, &playerRect);
	}
}
