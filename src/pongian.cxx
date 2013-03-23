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
* pongian.cpp - the main source file (you must compile this!)              *
*               please read also the readme file.                          *
****************************************************************************/

#include "pongian.hxx"

SDL_Surface *screen;
SDL_Event event;
SDL_Joystick *joystick;
yakwailog logger;
SDL_AudioSpec desired, obtained;
struct environment pongian;
struct pongian_xtheme xtheme;

ball kugel;
brett player1(1);
brett player2(2);

int main(int argNum, char *argA[])
{
	char menueReturn = 0;
	pongian.execPath = new char [strlen(argA[0]) + 2];
	strncpy(pongian.execPath, argA[0], strlen(argA[0]));
	getCleanExecPath(pongian.execPath);
	pongian.tempChar = new char [strlen(argA[0]) + 52];
	pongian.tempChar2 = new char [strlen(argA[0]) + 52];
	if (pongian.execPath[0] != '.')
	{
	    sprintf(pongian.tempChar, ".%s", pongian.execPath);
            sprintf(pongian.execPath, "%s", pongian.tempChar);
       	}
	sprintf(pongian.tempChar, "%s/pongian.log", pongian.execPath);
	if (argA[1] != NULL)
	{	
		if (strcmp(argA[1], "-l1") == 0) logger.init("pongian", pongian.tempChar, 1);
		if (strcmp(argA[1], "-l2") == 0) logger.init("pongian", pongian.tempChar, 2);
	} else
	{
		logger.init("pongian", pongian.tempChar, 0);
	}
	logger.log(0, "starting up %s", PONGIAN_VERSION);
	ReallyResetThings();
	loadSettings();
	handleArgs(argNum, argA);
	initEnv();
	setControls();
	logger.log(0, "start-up completed, running...");
	Intro();
//	if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
	while((menueReturn = mainMenue(menueReturn)) != 4)
	{
		if (menueReturn == 0)
		{
			pongian.actualLevel = 1;
			while (uniGameOrama() == 1)
			{
				pongian.actualLevel++;
			}
			pongian.actualLevel = 0;
			pongian.cpuPaddleEnabled = 0;
		}

		if (menueReturn == 1) uniGameOrama();
		if (menueReturn == 2) 
		{
//			if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
			optionsMenue(3);
		}
		if (menueReturn == 3) 
		{
//			if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
			aboutScreen();
		}
	}
//	if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
	Outro();
	quitPongian(1);
	return 0;
}


char uniGameOrama(void)
{
	char exitGame = 0, ret = 0;
	pongian.actualPlayer = 1 + rand()%2;
	if (pongian.actualLevel > 0)
	{
		switch(pongian.actualLevel) // check which level
		{
			case 1:
				sprintf(pongian.tempName, "Joey");
				pongian.cpuPaddleEnabled = 1;
			break;
			
			case 2:
				sprintf(pongian.tempName, "Al");
				pongian.cpuPaddleEnabled = 2;
			break;
		
			case 3:
				sprintf(pongian.tempName, "Trinity");
				pongian.cpuPaddleEnabled = 3;
			break;
		
			case 4:
				sprintf(pongian.tempName, "Noel");
				pongian.cpuPaddleEnabled = 4;
			break;
		}
	} else
	{
		sprintf(pongian.tempName, pongian.settings.p2name);
	}
	announceGame();
	do
	{
		ResetThings();
		pongian.winner = game();

		switch(pongian.winner)
		{
			case 1:
				pongian.p1wins++;
				pongian.actualPlayer = 2;
			break;
		
			case 2:
				pongian.p2wins++;
				pongian.actualPlayer = 1;
			break;
			
			case -1:
				pongian.actualPlayer = 0;
				exitGame = 1;
			break;
		}
		ShowPoints();
	} while (((pongian.p1wins < pongian.settings.games2win) && (pongian.p2wins < pongian.settings.games2win)) && exitGame == 0);

	if (pongian.p1wins == pongian.settings.games2win) { winnerScreen(1); ret = 1; }
	if (pongian.p2wins == pongian.settings.games2win) { winnerScreen(2); ret = 2; }
	if (exitGame == 1) ret = -1;
	pongian.p1wins = 0;
	pongian.p2wins = 0;
	SDL_WM_SetCaption(PONGIAN_TITLE, PONGIAN_TITLE);
	return ret;
}
	
void winnerScreen(char playerd)
{
	char tempString[50];
	SDL_BlitSurface(xtheme.game_background, NULL, screen, NULL);
	SFont_WriteCenter(screen, xtheme.selFont, 110, "game over");
	if (playerd == 1) 
	{
		sprintf(tempString, "and the winner is %s.", pongian.settings.p1name);
//	if ((pongian.actualLevel > 0) && (pongian.settings.soundEnabled)) PlaySound(&xtheme.gameoverWSND);
	}
	if (playerd == 2)
	{
		sprintf(tempString, "and the winner is %s.", pongian.tempName);
		if (pongian.cpuPaddleEnabled == 0) sprintf(tempString, "and the winner is %s.", pongian.settings.p2name);
	}
	SFont_WriteCenter(screen, xtheme.dslFont, 140, tempString);
	if ((pongian.actualLevel > 0) && (playerd == 2))
	{
		sprintf(tempString, "you have lost!");
		SFont_WriteCenter(screen, xtheme.dslFont, 300, tempString);
//	  if (pongian.settings.soundEnabled) PlaySound(&xtheme.gameoverLSND);
	}
	if ((pongian.actualLevel == 4) && (playerd == 1)) 
	{
		sprintf(tempString, "you have made it through!");
		SFont_WriteCenter(screen, xtheme.dslFont, 300, tempString);
//	  if (pongian.settings.soundEnabled) PlaySound(&xtheme.gameoverWSND);
	}

	SDL_Flip(screen);
	waitForKey(OKE_KEY);
}

void announceGame(void)
{
	char tempString[50];
	SDL_BlitSurface(xtheme.game_background, NULL, screen, NULL);
	SFont_WriteCenter(screen, xtheme.selFont, 110, "next game");
	sprintf(tempString, "%s versus %s", pongian.settings.p1name, pongian.tempName);
	SFont_WriteCenter(screen, xtheme.dslFont, 300, tempString);
	sprintf(pongian.tempChar, "pongian [ %s  %d : %d  %s ]", pongian.settings.p1name, pongian.p1wins, pongian.p2wins, pongian.tempName);
	SDL_WM_SetCaption(pongian.tempChar, PONGIAN_TITLE);
	SDL_Flip(screen);
	waitForKey(OKE_KEY);
}

void Intro(void)
{
	SDL_BlitSurface(xtheme.intro_background, NULL, screen, NULL);
//	if (pongian.settings.soundEnabled) PlaySound(&xtheme.introSND);
	SDL_Flip(screen);
	waitForKey(OKE_KEY);
}

void Outro(void)
{
	SDL_BlitSurface(xtheme.outro_background, NULL, screen, NULL);
	if (pongian.saveSettings) saveSettings();
	SDL_Flip(screen);
	waitForKey(OKE_KEY);
}

void ShowPoints(void)
{
	char tempString[50];
	SDL_BlitSurface(xtheme.game_background, NULL, screen, NULL);
	SFont_WriteCenter(screen, xtheme.selFont, 50, "statistics");
//	if ((pongian.cpuPaddleEnabled > 0) && (pongian.settings.soundEnabled) && (pongian.actualPlayer == 2)) PlaySound(&xtheme.winpointSND);
//	if ((pongian.cpuPaddleEnabled > 0) && (pongian.settings.soundEnabled) && (pongian.actualPlayer == 1)) PlaySound(&xtheme.loosepointSND);
	sprintf(tempString, "%s: %d", pongian.settings.p1name, pongian.p1wins);
	SFont_WriteCenter(screen, xtheme.dslFont, 110, tempString);
	sprintf(tempString, "%s: %d", pongian.tempName, pongian.p2wins);		
	SFont_WriteCenter(screen, xtheme.dslFont, 140, tempString);
	if (((pongian.p1wins != pongian.settings.games2win) && (pongian.p2wins != pongian.settings.games2win)) ||
			((pongian.actualLevel > 0) && (pongian.p1wins != 5)) && ((pongian.actualLevel > 0) && (pongian.p2wins != 5)))
	{
		if (pongian.actualPlayer == 1) sprintf(tempString, "%s's got the ball.", pongian.settings.p1name);
		if (pongian.actualPlayer == 2)
		{
			sprintf(tempString, "%s's got the ball.", pongian.tempName);
			if (pongian.cpuPaddleEnabled == 0) sprintf(tempString, "%s's got the ball.", pongian.settings.p2name);
		}
		if (pongian.actualPlayer != 0) SFont_WriteCenter(screen, xtheme.dslFont, 400, tempString);
	}
	sprintf(pongian.tempChar, "pongian [ %s  %d : %d  %s ]", pongian.settings.p1name, pongian.p1wins, pongian.p2wins, pongian.tempName);
	SDL_WM_SetCaption(pongian.tempChar, PONGIAN_TITLE);
	SDL_Flip(screen);
	waitForKey(OKE_KEY);
}

char game(void)
{
	char exitloop = 1;
	SDL_BlitSurface(xtheme.game_background, NULL, screen, NULL);
	SetThings();
	if ((pongian.actualPlayer <= 0) || (pongian.actualPlayer >= 3)) pongian.actualPlayer = 1;
	kugel.Do();
	do
	{
		SDL_PollEvent(&event);
		if (event.key.state == SDL_PRESSED)
		{
			if (event.key.keysym.sym == ESCAPE) exitloop = 0;
			if (event.key.keysym.sym == SDLK_p)
			{
				SDL_BlitSurface(xtheme.game_background, NULL, screen, NULL);
				SFont_WriteCenter(screen, xtheme.selFont, 200, "- paused -");
				SDL_Flip(screen);
				waitForKey(SDLK_p);
				SDL_BlitSurface(xtheme.game_background, NULL, screen, NULL);
				SDL_Flip(screen);
				SDL_Delay(50);
			}
		}

		player2.Draw(FALSE);
		kugel.Draw(FALSE);
		player1.Draw(FALSE);

		player2.control();
		kugel.Do();
		player1.control();

		player2.DrawShadow(TRUE);
		kugel.DrawShadow(TRUE);
		player1.DrawShadow(TRUE);
		
		player2.Draw(TRUE);
		kugel.Draw(TRUE);
		player1.Draw(TRUE);

		SDL_Flip(screen);
		SDL_Delay(TimeLeft());
		if (kugel.ballRect.y <= 0) exitloop = 2;

		if (kugel.ballRect.y >= pongian.settings.y) exitloop = 3;

	} while (exitloop == 1);

//	if (((exitloop == 2) || (exitloop == 3)) && (pongian.settings.soundEnabled)) PlaySound(&xtheme.crashSND);

	return exitloop - 1;
}

Uint32 TimeLeft(void)
{
    static Uint32 next_time = 0;
    Uint32 now, ret = 0;
    
    now = SDL_GetTicks();
    if (next_time <= now)
	{
        next_time = now + TICK_INTERVAL;
        return(ret);
    }
    ret = next_time - now;
    return(ret);
}

void ResetThings(void)
{
	player1.playerRect.x = (pongian.settings.x/2) - (pongian.length/2);
	player1.playerRect.y = pongian.settings.y - pongian.height;
	player1.move = 0;
	player2.playerRect.x = (pongian.settings.x/2) - (pongian.length/2);
	player2.playerRect.y = 0;
	player2.move = 0;
	kugel.movex = 0;
	kugel.movey = 0;
}

void ReallyResetThings(void) //start-up inits (defaults)
{
	pongian.diameter = BALL_WIDTH;
	pongian.length = BRETTL_LENGTH;
	pongian.height = BRETTL_HEIGHT;
	pongian.settings.games2win = GAMES2WIN;
	pongian.actualPlayer = 0;
	pongian.actualLevel = 0;
	pongian.settings.x = XMAX;
	pongian.settings.y = YMAX;
	pongian.settings.bpp = VIDEO_BITS;
	pongian.settings.flags = VIDEO_MODE;
	pongian.settings.p1ctrl = PADDLE1_CTRL;
	pongian.settings.p2ctrl = PADDLE2_CTRL;
	pongian.settings.soundEnabled = TRUE;
	pongian.ballShadow = FALSE;
	pongian.paddleShadow = FALSE;
	pongian.saveSettings = TRUE;
	sprintf(pongian.settings.p1name, "player1");
	sprintf(pongian.settings.p1name, "%s", getenv("LOGNAME"));
	sprintf(pongian.tempChar, "%s/.%s", getenv("HOME"), PONGIAN_CONF);
	pongian.configFile = new char[strlen(pongian.tempChar) + 1];
	strcpy(pongian.configFile, pongian.tempChar);
	sprintf(pongian.settings.p2name, "player2");
	pongian.joyEnabled = 0;
}

void setControls(void)
{
	if ((pongian.settings.p1ctrl < KBD1) || (pongian.settings.p1ctrl > MAXCTRL)) pongian.settings.p1ctrl = KBD1;
	if ((pongian.settings.p2ctrl < KBD1) || (pongian.settings.p2ctrl > MAXCTRL)) pongian.settings.p2ctrl = KBD2;
	if ((pongian.settings.p1ctrl == JOYSTICK) && (SDL_NumJoysticks() == 0)) pongian.settings.p1ctrl = KBD1;
	if ((pongian.settings.p2ctrl == JOYSTICK) && (SDL_NumJoysticks() == 0)) pongian.settings.p2ctrl = KBD2;
	if (pongian.settings.p1ctrl == KBD1)
	{
	    pongian.p1Left = SDLK_LEFT;
	    pongian.p1Right = SDLK_RIGHT;
	    pongian.p1Up = SDLK_UP;
	    pongian.p1Down = SDLK_DOWN;
	} else if (pongian.settings.p1ctrl == KBD2)
	{
	    pongian.p1Left = SDLK_a;
	    pongian.p1Right = SDLK_d;
	    pongian.p1Up = SDLK_w;
	    pongian.p1Down = SDLK_s;
	}
	if (pongian.settings.p2ctrl == KBD1)
	{
	    pongian.p2Left = SDLK_LEFT;
	    pongian.p2Right = SDLK_RIGHT;
	    pongian.p2Up = SDLK_UP;
	    pongian.p2Down = SDLK_DOWN;
	} else if (pongian.settings.p2ctrl == KBD2)
	{
	    pongian.p2Left = SDLK_a;
	    pongian.p2Right = SDLK_d;
	    pongian.p2Up = SDLK_w;
	    pongian.p2Down = SDLK_s;
	}
	if ((pongian.settings.p1ctrl == MOUSE) || (pongian.settings.p2ctrl == MOUSE))
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);
	} else
	{
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}
}

void SetThings(void)
{
	player1.Move(STOP);
	player2.Move(STOP);
}

void handleArgs(char argN, char *argT[])
{
	char argC = 0;
	
	while (argC < argN)
	{
		if (strcmp(argT[argC],"-nc") == 0)
		{
			pongian.saveSettings = FALSE;
			logger.log(0, "settings will NOT be saved.");
		}
	
		if (strcmp(argT[argC],"-f") == 0)
		{
			pongian.settings.flags |= SDL_FULLSCREEN;
			logger.log(0, "will start in fullscreen mode...");
		}

		if (strcmp(argT[argC],"-b8") == 0)
		{
			pongian.settings.bpp = 8;
			logger.log(0, "will start in 8bit color mode...");
		}

		if (strcmp(argT[argC],"-b16") == 0)
		{
			pongian.settings.bpp = 16;
			logger.log(0, "will start in 16bit color mode...");
		}

		if (strcmp(argT[argC],"-b24") == 0)
		{
			pongian.settings.bpp = 24;
			logger.log(0, "will start in 24bit color mode...");
		}

		if (strcmp(argT[argC],"-nj") == 0)
		{
			pongian.joyEnabled = -1;
			logger.log(0, "joystick support disabled.");
		}
		
		if (strcmp(argT[argC],"-ns") == 0)
		{
			pongian.settings.soundEnabled = 0;
			logger.log(0, "sound support disabled.");
		}

		if (strcmp(argT[argC],"-t") == 0)
		{
			if (argT[argC + 1] == NULL)
			{
				logger.log(2, "seems that you've forgotton to insert the [dir] var: see --help");
				quitPongian(2);
			}

			if (strlen(argT[argC + 1]) < 50)
			{
				strcpy(pongian.settings.themePath, argT[argC + 1]);
				argC++;
				logger.log(0, "will try %s skin...", pongian.settings.themePath);
			} else
			{
				logger.log(2, "sorry, pal. the name of your theme is too long.");
			}
		}
		if (strcmp(argT[argC],"--help") == 0)
		{
			printf("---------------------------------------------------------------------------\n");
			printf(" pongian command line options....\n");
			printf(" pongian [options]\n");
			printf(" options are: -f fullscreen Mode\n");
			printf("              -b[8|16|24] start in 8/16/24bit color mode (default: %d)\n", VIDEO_BITS);
			printf("              -t [dir] where dir is the theme directory\n");
			printf("              -nc do not save settings at end.\n");
			printf("              -nj disable joystick support.\n");
			printf("              -ns disable sound support.\n");
			printf("---------------------------------------------------------------------------\n");
			printf("pongian version %s.\n", PONGIAN_VERSION);
			quitPongian(2);
		}
		argC++;
	}

	if (strlen(pongian.settings.themePath) == 0) strcpy(pongian.settings.themePath, "default");
        sprintf(pongian.tempChar, "%s/themes/%s", pongian.execPath, pongian.settings.themePath);
	pongian.fSkinPath = new char [strlen(pongian.tempChar) + 1];
	strcpy(pongian.fSkinPath, pongian.tempChar);
}

void initSound(void)
{
  if (pongian.settings.soundEnabled)
  {
  	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
   	{
   		fprintf(stderr, "sound disabled.\n");
   		pongian.settings.soundEnabled = FALSE;
   	} else
   	{
   		pongian.settings.soundEnabled = TRUE;
   	}
   }
 /*	if (pongian.settings.soundEnabled)
 	{
		desired.freq = 44100;
		desired.format = AUDIO_S16;
		desired.samples = 4096;
		desired.channels = 2;
		desired.callback = AudioCallback;
		desired.userdata = NULL;
		if (SDL_OpenAudio(&desired, &obtained) < 0)
		{
			fprintf(stderr, "unable to open audio device: %s\n", SDL_GetError());
			exit(1);
		}
		ClearPlayingSounds();
		SDL_PauseAudio(0);
	}*/
}

void initEnv(void)
{
	pongian.picFile = new char [strlen(pongian.execPath) + 20];

	/* Initialize the SDL library */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0) 
	{
    	logger.log(2, "couldn't init SDL!!\n");
		quitPongian(-1);
	}
 
	initSound();
 
	/* Clean up on exit */
	atexit(SDL_Quit);
	sprintf(pongian.picFile, "%s/pongian.png", pongian.execPath);
	
	pongian.iconPic = IMG_Load(pongian.picFile);
	
	SDL_WM_SetIcon(pongian.iconPic, NULL);
	
	screen = SDL_SetVideoMode(pongian.settings.x, pongian.settings.y, pongian.settings.bpp, pongian.settings.flags);
	if (screen == NULL) 
	{
		logger.log(2, "couldn't set video to %dx%dx%d!!",pongian.settings.x, pongian.settings.y, pongian.settings.bpp);
    	quitPongian(-1);
	}

	SDL_WM_SetCaption(PONGIAN_TITLE, "pongian");
	SDL_ShowCursor(SDL_DISABLE);
	switch(xtheme_loadTheme(&xtheme))
	{
		case 0:
			logger.log(2, "there was a problem with the skin... go check it!");	
			quitPongian(-2);
		break;
			
		case 3:
			logger.log(1, "disabling sound.");
			xtheme_freeTheme(&xtheme);
			SDL_QuitSubSystem(SDL_INIT_AUDIO);
	 		pongian.settings.soundEnabled = 0;
			xtheme_loadTheme(&xtheme);
		break;
	}
	
	if ((SDL_NumJoysticks() > 0) && (pongian.joyEnabled == FALSE))
	{
		joystick = SDL_JoystickOpen(0);
		if (SDL_JoystickNumButtons(joystick) >= 2) 
		{
			pongian.joyEnabled = TRUE;
			SDL_JoystickEventState(SDL_ENABLE);
			logger.log(0, "joystick support is enabled.");
		}
	} 
}

char mainMenue(char selected)
{
	char tempChosen = 0, chose = 0;

	while (chose == 0)
	{
		if (tempChosen == 0)
		{
			SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
			if (selected == 0)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 140, "start game");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 140, "start game");
			}

			if (selected == 1)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 170, "single game");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 170, "single game");
			}

			if (selected == 2)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 200, "options");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 200, "options");
			}

			if (selected == 3)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 230, "about");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 230, "about");
			}


			if (selected == 4)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 260, "exit");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 260, "exit");
			}
			SDL_Flip(screen);
			tempChosen = 1;
		}
		SDL_Delay(50);
		if (SDL_PollEvent(&event) && (event.type == SDL_KEYDOWN))
		{
			if (event.key.keysym.sym == SDLK_UP)
			{
				selected--;
				tempChosen = 0;
			}

			if (event.key.keysym.sym == SDLK_DOWN)
			{
				selected++;
				tempChosen = 0;
			}

			if (selected > 4) selected = 0;
			if (selected < 0) selected = 4;

			if (event.key.keysym.sym == OKE_KEY) chose = 1;
		}
	}
	return selected;
}

char optionsMenue(char selected)
{
	char tempChosen = 0, chose = 0;

	while (chose == 0)
	{
		if (tempChosen == 0)
		{
			SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
			if (selected == 0)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 140, "system");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 140, "system");
			}

			if (selected == 1)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 170, "game");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 170, "game");
			}

			if (selected == 2)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 200, "controls");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 200, "controls");
			}

			if (selected == 3)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 230, "back");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 230, "back");
			}
			SDL_Flip(screen);
			tempChosen = 1;
		}
		SDL_Delay(50);
		if (SDL_PollEvent(&event) && (event.type == SDL_KEYDOWN))
		{
			if (event.key.keysym.sym == SDLK_UP)
			{
				selected--;
				tempChosen = 0;
			}

			if (event.key.keysym.sym == SDLK_DOWN)
			{
				selected++;
				tempChosen = 0;
			}

			if (selected > 3) selected = 0;
			if (selected < 0) selected = 3;

			if (event.key.keysym.sym == OKE_KEY)
			{
				if (selected == 0)
				{
//					if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
					videoMenue(3);
					tempChosen = 0;
				}
				
				if (selected == 1)
				{
//					if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
					gameMenue(3);
					tempChosen = 0;
				}
				
				if (selected == 2)
				{
//					if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
					ctrlMenue(2);
					tempChosen = 0;
				}

				if (selected == 3) 
				{
//					if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
					chose = 1;
				}
			}
		}
	}
	return selected;
}

char videoMenue(char selected)
{
	char tempChosen = 0, chose = 0;

	while (chose == 0)
	{
		if (tempChosen == 0)
		{
			SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
			if (selected == 0)
			{
				if (screen->flags == SDL_FULLSCREEN)
				{
					SFont_WriteCenter(screen, xtheme.selFont, 140, "switch window");
				} else
				{
					SFont_WriteCenter(screen, xtheme.selFont, 140, "switch fullscreen");
				}
			} else
			{
				if (screen->flags == SDL_FULLSCREEN)
				{
					SFont_WriteCenter(screen, xtheme.dslFont, 140, "switch window");
				} else
				{
					SFont_WriteCenter(screen, xtheme.dslFont, 140, "switch fullscreen");
				}
			}

			if (selected == 1)
			{
				if (pongian.settings.soundEnabled)
				{
					SFont_WriteCenter(screen, xtheme.selFont, 170, "switch sound off");
				} else
				{
					SFont_WriteCenter(screen, xtheme.selFont, 170, "switch sound on");
				}
			} else
			{
				if (pongian.settings.soundEnabled)
				{
					SFont_WriteCenter(screen, xtheme.dslFont, 170, "switch sound off");
				} else
				{
					SFont_WriteCenter(screen, xtheme.dslFont, 170, "switch sound on");
				}
			}

			if (selected == 2)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 200, "theme info");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 200, "theme info");
			}

			if (selected == 3)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 230, "back");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 230, "back");
			}
			SDL_Flip(screen);
			tempChosen = 1;
		}
		SDL_Delay(50);
		if (SDL_PollEvent(&event) && (event.type == SDL_KEYDOWN))
		{
			if (event.key.keysym.sym == SDLK_UP)
			{
				selected--;
				tempChosen = 0;
			}

			if (event.key.keysym.sym == SDLK_DOWN)
			{
				selected++;
				tempChosen = 0;
			}

			if (selected > 3) selected = 0;
			if (selected < 0) selected = 3;

			if (event.key.keysym.sym == OKE_KEY)
			{

				if (selected == 0)
				{
				    #ifndef WIN32
				        pongian.settings.flags ^= SDL_FULLSCREEN;
			            screen = SDL_SetVideoMode(screen->w, screen->h, screen->format->BitsPerPixel,	pongian.settings.flags);
					    if (screen == NULL) 
					    {
						     logger.log(2, "Couldn't toggle to window/fullscreen mode.");
						     quitPongian(-5);
                        }
					#else
					    SFont_WriteCenter(screen, xtheme.selFont, 280, "this switch does not work in win32!");
					    SDL_Flip(screen);
					    waitForKey(OKE_KEY);
                    #endif
					tempChosen = 0;
				}

				if (selected == 1)
				{
					SFont_WriteCenter(screen, xtheme.selFont, 280, "please wait...");
					SDL_Flip(screen);
					if (pongian.settings.soundEnabled)
					{
						xtheme_freeTheme(&xtheme);
						SDL_QuitSubSystem(SDL_INIT_AUDIO);
						pongian.settings.soundEnabled = 0;
						xtheme_loadTheme(&xtheme);
					} else
					{
						xtheme_freeTheme(&xtheme);
						pongian.settings.soundEnabled = 1;
						initSound();
						xtheme_loadTheme(&xtheme);
					}
					tempChosen = 0;
				}

				if (selected == 2) 
				{
//					if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
					skinInfo();
					tempChosen = 0;
				}
				
				if (selected == 3) 
				{
//					if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
					chose = 1;
				}
				
			}
		}
	}
	if (screen->flags == SDL_FULLSCREEN)
	{
		pongian.settings.flags = SDL_FULLSCREEN;
	} else
	{
		pongian.settings.flags = SDL_SWSURFACE;
	}

	return selected;
}

char gameMenue(char selected)
{
	char tempChosen = 0, chose = 0;
	char tempChar[50];

	while (chose == 0)
	{
		if (tempChosen == 0)
		{
			SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
			sprintf(tempChar, "games to win: %d", pongian.settings.games2win);
			if (selected == 0)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 140, tempChar);
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 140, tempChar);
			}
			sprintf(tempChar, "player 1's name: %s", pongian.settings.p1name);
			if (selected == 1)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 170, tempChar);
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 170, tempChar);
			}
			sprintf(tempChar, "player 2's name: %s", pongian.settings.p2name);
			if (selected == 2)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 200, tempChar);
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 200, tempChar);
			}

			if (selected == 3)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 230, "back");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 230, "back");
			}
			SDL_Flip(screen);
			tempChosen = 1;
		}
		SDL_Delay(50);
		if (SDL_PollEvent(&event) && (event.type == SDL_KEYDOWN))
		{
			if (event.key.keysym.sym == SDLK_UP)
			{
				selected--;
				tempChosen = 0;
			}

			if (event.key.keysym.sym == SDLK_DOWN)
			{
				selected++;
				tempChosen = 0;
			}

			if (selected > 3) selected = 0;
			if (selected < 0) selected = 3;

			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				if (selected == 0)
				{
					pongian.settings.games2win++;
					if (pongian.settings.games2win > 101) pongian.settings.games2win = 1;
					tempChosen = 0;
				}
			}

			if (event.key.keysym.sym == SDLK_LEFT)
			{
				if (selected == 0)
				{
					pongian.settings.games2win--;
					if (pongian.settings.games2win < 1) pongian.settings.games2win = 101;
					tempChosen = 0;
				}
			}

			if (event.key.keysym.sym == OKE_KEY)
			{
				if (selected == 1)
				{
					inputScreen(1);
					tempChosen = 0;
				}

				if (selected == 2)
				{
					inputScreen(2);
					tempChosen = 0;
				}

				if (selected == 3) 
				{
//					if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
					chose = 1;
				}
			}
		}
	}
	return selected;
}

char ctrlMenue(char selected)
{
	char tempChosen = 0, chose = 0;
	char tempChar[50];

	while (chose == 0)
	{
		if (tempChosen == 0)
		{
			SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
    	if (pongian.settings.p1ctrl == KBD1) 
			{
		  	sprintf(tempChar, "%s control: cursors", pongian.settings.p1name);
		  } else if (pongian.settings.p1ctrl == KBD2)
		  {
		    sprintf(tempChar, "%s control: w, a, s, d", pongian.settings.p1name);
			} else if (pongian.settings.p1ctrl == MOUSE)
			{
				sprintf(tempChar, "%s control: mouse", pongian.settings.p1name);
			} else if (pongian.settings.p1ctrl == JOYSTICK)
			{
				sprintf(tempChar, "%s control: joystick", pongian.settings.p1name);
			} else
			{
			  sprintf(tempChar, "%s control: unknown", pongian.settings.p1name);
			}

			if (selected == 0)
			{
			    SFont_WriteCenter(screen, xtheme.selFont, 140, tempChar);
			} else
			{
			    SFont_WriteCenter(screen, xtheme.dslFont, 140, tempChar);
			}

			if (pongian.settings.p2ctrl == KBD1) 
			{
			    sprintf(tempChar, "%s control: cursors", pongian.settings.p2name);
			} else if (pongian.settings.p2ctrl == KBD2)
			{
			    sprintf(tempChar, "%s control: w, a, s, d", pongian.settings.p2name);
			} else if (pongian.settings.p2ctrl == MOUSE)
			{
					sprintf(tempChar, "%s control: mouse", pongian.settings.p2name);
			} else if (pongian.settings.p2ctrl == JOYSTICK)
			{
					sprintf(tempChar, "%s control: joystick", pongian.settings.p2name);
			} else
			{
			    sprintf(tempChar, "%s control: unknown", pongian.settings.p2name);
			}			
			
			if (selected == 1)
			{

				SFont_WriteCenter(screen, xtheme.selFont, 170, tempChar);
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 170, tempChar);
			}

			if (selected == 2)
			{
				SFont_WriteCenter(screen, xtheme.selFont, 200, "back");
			} else
			{
				SFont_WriteCenter(screen, xtheme.dslFont, 200, "back");

			}
			SDL_Flip(screen);
			tempChosen = 1;
		}
		SDL_Delay(50);
		if (SDL_PollEvent(&event) && (event.type == SDL_KEYDOWN))
		{
			if (event.key.keysym.sym == SDLK_UP)
			{
				selected--;
				tempChosen = 0;
			}

			if (event.key.keysym.sym == SDLK_DOWN)
			{
				selected++;
				tempChosen = 0;
			}

			if (selected > 2) selected = 0;
			if (selected < 0) selected = 2;

			if ((event.key.keysym.sym == SDLK_RIGHT) || (event.key.keysym.sym == OKE_KEY))
			{
				if (selected == 0)
				{
					pongian.settings.p1ctrl++;
					if ((pongian.settings.p1ctrl == JOYSTICK) && (pongian.joyEnabled != 1)) pongian.settings.p1ctrl = KBD1;
					tempChosen = 0;
				}
				if (selected == 1)
				{
					pongian.settings.p2ctrl++;
					if ((pongian.settings.p2ctrl == JOYSTICK) && (pongian.joyEnabled != 1)) pongian.settings.p2ctrl = KBD1;
					tempChosen = 0;
				}
			}

			if (event.key.keysym.sym == SDLK_LEFT)
			{
				if (selected == 0)
				{
					pongian.settings.p1ctrl--;
					if (pongian.settings.p1ctrl < KBD1) pongian.settings.p1ctrl = JOYSTICK;
					if ((pongian.settings.p1ctrl == JOYSTICK) && (pongian.joyEnabled != 1)) pongian.settings.p1ctrl = MOUSE;
					tempChosen = 0;
				}
				if (selected == 1)
				{
					pongian.settings.p2ctrl--;
					if (pongian.settings.p2ctrl < KBD1) pongian.settings.p2ctrl = JOYSTICK;
					if ((pongian.settings.p2ctrl == JOYSTICK) && (pongian.joyEnabled != 1)) pongian.settings.p2ctrl = MOUSE;
					tempChosen = 0;
				}
			}

			if ((event.key.keysym.sym == OKE_KEY) && (selected == 2)) 
			{
//				if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
				chose = 1;
			}
		}
	}
	setControls();
	return selected;
}

/**************************************************************/
/* originally coded by _georg, modified by steff.             */
/**************************************************************/
void SFont_Read (SDL_Surface *Surface, const SFont_Font *Fontzie, int x, int y, char *text)
{
    unsigned int j = 0;
    char Exit = FALSE, key;

    SFont_Write(Surface, Fontzie, x - SFont_TextWidth(Fontzie, "> "), y, ">");
    SDL_Flip(screen);

    for (; (Exit != TRUE);)
    {
	 		SDL_PollEvent(&event);
			if (event.key.type == SDL_KEYDOWN)
			{
				key = event.key.keysym.sym;
				if (key == SDLK_ESCAPE)
				{
					Exit = TRUE;
				} else if (key == SDLK_BACKSPACE)
				{
					if (j > 0)
					{
						j--;
						*(text + j) = 0;
	
						redrawInputScreen();
						SFont_Write(Surface, Fontzie, x, y, text);
						SFont_Write(Surface, Fontzie, x - SFont_TextWidth(Fontzie, "> "), y, ">");
						SDL_Flip(screen);
						while(event.key.type != SDL_KEYUP)
						{
							SDL_PollEvent(&event);
						}
					}
				} else if (key == SDLK_RETURN)
				{
					text = '\0';
					Exit = TRUE;
				} else
				{
					*(text + j) = key;
					*(text + j + 1) = '\0';
					if (j < 18) j++;

					redrawInputScreen();
					SFont_Write (Surface, Fontzie, x, y, text);
					SFont_Write(Surface, Fontzie, x - SFont_TextWidth(Fontzie, "> "), y, ">");
					SDL_Flip(screen);
					while(event.key.type != SDL_KEYUP)
					{
						SDL_PollEvent(&event);
					}
				}
			}
		}
}

void inputScreen(char nameNr)
{
//	if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueenterSND);
	redrawInputScreen();
	if (event.key.state == SDL_PRESSED)
	{
		while (event.key.state == SDL_PRESSED)
		{
			SDL_PollEvent(&event);
		}
	}
	if (nameNr == 1) SFont_Read(screen, xtheme.dslFont, 70, 200, pongian.settings.p1name);
	if (nameNr == 2) SFont_Read(screen, xtheme.dslFont, 70, 200, pongian.settings.p2name);
//	if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
}

void redrawInputScreen(void)
{
	SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
	SFont_WriteCenter(screen, xtheme.selFont, 100, "insert name");
}

void aboutScreen(void)
{
	SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
	sprintf(pongian.tempChar, "%s", PONGIAN_VERSION);
	SFont_WriteCenter(screen, xtheme.selFont, 75, "pongian");
	SFont_WriteCenter(screen, xtheme.dslFont, 107, pongian.tempChar);
	SFont_WriteCenter(screen, xtheme.dslFont, 150, "is brought to you by");
	SFont_WriteCenter(screen, xtheme.dslFont, 180, "Stefan Zidar");
	SFont_WriteCenter(screen, xtheme.selFont, 280, "it uses");
	SFont_WriteCenter(screen, xtheme.dslFont, 330, "SDL 1.2 (www.libsdl.org)");
	SFont_WriteCenter(screen, xtheme.dslFont, 360, "SFont 2.0");
	SFont_WriteCenter(screen, xtheme.dslFont, 390, "SDL_image 1.2");
	SDL_Flip(screen);
	waitForKey(OKE_KEY);
//	if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
}

void waitForKey(SDLKey key)
{
	char quit = FALSE;
	while (!quit)
	{
		if (SDL_PollEvent(&event) && (event.key.keysym.sym == key) && (event.type == SDL_KEYDOWN)) quit = TRUE;
		SDL_Delay(50);
	}
}

char stringCopyN(char *string2, char *string1, int start, int end)
{
	int counter = start;
	char ret = FALSE;
	int counter2 = 0;
	if (end > start)
	{
		while (counter <= end)
		{
			*(string2 + counter2) = *(string1 + counter);
			counter2++;
			counter++;
		}
		*(string2 + counter2) = '\0';
		ret = TRUE;
	}
	return ret;
}

char removeNewlineFromString(char *string)
{
	unsigned char stringLen = strlen(string);
	unsigned char counter = 0;
	unsigned char counter2 = 0;
	char newString[stringLen];
	while (counter2 < stringLen)
	{
		if (string[counter2] == '\n') counter2++;
		newString[counter] = string[counter2];
		counter++;
		counter2++;
	}
	strcpy(string, newString);
	return 1;
}

int getCleanExecPath(char *pathAndExec)
{
	int lenge = strlen(pathAndExec), exitLoop = 0;
	while (exitLoop == 0)
	{
		if (*(pathAndExec + lenge) == '/') exitLoop = 1;
		lenge--;
		if (lenge < 0) exitLoop = 1;
	}
	*(pathAndExec + lenge + 1) = '\0';
	return lenge;
}


void skinInfo(void)
{
	SDL_BlitSurface(xtheme.menue_background, NULL, screen, NULL);
	SFont_WriteCenter(screen, xtheme.selFont, 110, xtheme.name);
	SFont_WriteCenter(screen, xtheme.dslFont, 150, "is brought to you by");
	SFont_WriteCenter(screen, xtheme.dslFont, 190, xtheme.author);
	SFont_WriteCenter(screen, xtheme.selFont, 320, "version");
	SFont_WriteCenter(screen, xtheme.dslFont, 350, xtheme.version);
	SDL_Flip(screen);
	waitForKey(OKE_KEY);
//	if (pongian.settings.soundEnabled) PlaySound(&xtheme.menueleaveSND);
}

void saveSettings(void)
{
	FILE *settFile;
	if ((settFile = fopen(pongian.configFile, "w")) != NULL)
	{
		pongian.settings.id[0] = 'P';
		pongian.settings.id[1] = 'c';
		pongian.settings.id[2] = 'F';
		fwrite(&pongian.settings, sizeof(pongian.settings), 1, settFile);
		fclose(settFile);
	} else
	{
		logger.log(1, "couldn't open config file '%s' for writing.", pongian.configFile);
	}
}

void loadSettings(void)
{
	FILE *settFile;
	if ((settFile = fopen(pongian.configFile, "r")) != NULL)
	{
		fread(&pongian.settings, sizeof(pongian.settings), 1, settFile);
		if (!((pongian.settings.id[0] == 'P') && (pongian.settings.id[1] == 'c') && (pongian.settings.id[2] == 'F')))
		{
			logger.log(1, "your config file is buggy, using defaults.");
			ReallyResetThings();			
		}
		fclose(settFile);
	} else
	{
		logger.log(1, "couldn't open config file '%s' for reading.", pongian.configFile);
	}
}

void cleanUp(void)
{
	if (pongian.settings.soundEnabled)	SDL_CloseAudio();
	if (pongian.joyEnabled == 1) SDL_JoystickClose(joystick);
	SDL_FreeSurface(screen);
	xtheme_freeTheme(&xtheme);
	SDL_FreeSurface(pongian.iconPic);
	delete [] pongian.picFile;
	delete [] pongian.fSkinPath;
}

void quitPongian(char mode)
{
	logger.log(0, "exiting, good bye!");
	if (mode != 2) cleanUp();
	logger.end();
	delete [] pongian.execPath;
	delete [] pongian.configFile;
	delete [] pongian.tempChar;
	delete [] pongian.tempChar2;
	exit(mode);
}
