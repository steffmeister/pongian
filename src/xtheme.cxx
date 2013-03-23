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
* xtheme.c - the eXtended pongian THEME "engine"                           *
****************************************************************************/

#include "xtheme.hxx"

char xtheme_freeTheme(struct pongian_xtheme *sg2free)
{
	SDL_FreeSurface(sg2free->menue_background);
	SDL_FreeSurface(sg2free->game_background);
	SDL_FreeSurface(sg2free->intro_background);
	SDL_FreeSurface(sg2free->outro_background);
	SDL_FreeSurface(sg2free->daballSkin);
	SDL_FreeSurface(sg2free->daballShad);
	SDL_FreeSurface(sg2free->daplayer1Skin);
	SDL_FreeSurface(sg2free->daplayer2Skin);
	SDL_FreeSurface(sg2free->daplayerShad);
	SFont_FreeFont(sg2free->selFont);
	SFont_FreeFont(sg2free->dslFont);
/*	if (pongian.settings.soundEnabled)
	{
		free(sg2free->introSND.samples);
		free(sg2free->paddlebangSND.samples);
		free(sg2free->wallbangSND.samples);
		free(sg2free->gameoverWSND.samples);
		free(sg2free->gameoverLSND.samples);
		free(sg2free->menueenterSND.samples);
		free(sg2free->menueleaveSND.samples);
		free(sg2free->winpointSND.samples);
		free(sg2free->loosepointSND.samples);
		free(sg2free->shotSND.samples);
		free(sg2free->crashSND.samples);
	}*/
	return 1;
}

char xtheme_loadTheme(struct pongian_xtheme *sg2create)
{
	FILE *confFile;
	char ret = 0;
	char confFilename[strlen(pongian.fSkinPath) + 20];
	sprintf(confFilename, "%s/theme.conf", pongian.fSkinPath);
	if ((confFile = fopen(confFilename, "r")) != NULL)
	{
		ret = 1;
		char lineForLine[80];
		while (!feof(confFile))
		{
			fgets(lineForLine, 79, confFile);
			if (strncmp(lineForLine, "FONT_SELECTED = ", 16) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 16, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "FONT_SELECTED '%s'\n", pongian.tempChar2);
				sg2create->slFont = IMG_Load(pongian.tempChar2);
				sg2create->selFont = SFont_InitFont(sg2create->slFont);
			}
			
			if (strncmp(lineForLine, "FONT_DESELECTED = ", 18) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 18, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "FONT_DESELECTED '%s'\n", pongian.tempChar2);
				sg2create->dsFont = IMG_Load(pongian.tempChar2);
				sg2create->dslFont = SFont_InitFont(sg2create->dsFont);
				//SFont_WriteCenter(screen, sg2create->dslFont, 200, "loading pongian...");
				//SDL_Flip(screen);
			}
			
			if (strncmp(lineForLine, "INTRO_BACKGROUND = ", 19) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 19, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "INTRO_BACKGROUND '%s'\n", pongian.tempChar2);
				sg2create->intro_background = SDL_DisplayFormat(IMG_Load(pongian.tempChar2));
			}

			if (strncmp(lineForLine, "OUTRO_BACKGROUND = ", 19) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 19, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "OUTRO_BACKGROUND '%s'\n", pongian.tempChar2);
				sg2create->outro_background = SDL_DisplayFormat(IMG_Load(pongian.tempChar2));
			}
			
			if (strncmp(lineForLine, "MENUE_BACKGROUND = ", 19) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 19, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "MENUE_BACKGROUND '%s'\n", pongian.tempChar2);
				sg2create->menue_background = SDL_DisplayFormat(IMG_Load(pongian.tempChar2));
			}

			if (strncmp(lineForLine, "GAME_BACKGROUND = ", 18) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 18, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "GAME_BACKGROUND '%s'\n", pongian.tempChar2);
				sg2create->game_background = SDL_DisplayFormat(IMG_Load(pongian.tempChar2));
				//sg2create->game_background = SDL_DisplayFormat(sg2create->game_background);
			}
			
			if (strncmp(lineForLine, "BALL_SKIN = ", 12) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 12, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "BALL_SKIN '%s'\n", pongian.tempChar2);
				sg2create->daballSkin = IMG_Load(pongian.tempChar2);
			}
			
			if (strncmp(lineForLine, "PLAYER1_SKIN = ", 15) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 15, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "PLAYER1_SKIN '%s'\n", pongian.tempChar2);
				sg2create->daplayer1Skin = IMG_Load(pongian.tempChar2);
				//sg2create->daplayer1Skin = SDL_DisplayFormat(sg2create->daplayer1Skin);
			}
						
			if (strncmp(lineForLine, "PLAYER2_SKIN = ", 15) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 15, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "PLAYER2_SKIN '%s'\n", pongian.tempChar2);
				sg2create->daplayer2Skin = IMG_Load(pongian.tempChar2);
				//sg2create->daplayer2Skin = SDL_DisplayFormat(sg2create->daplayer2Skin);
			}

			if (strncmp(lineForLine, "BALL_SHADOW = ", 14) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 14, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "BALL_SHADOW '%s'\n", pongian.tempChar2);
				sg2create->daballShad = IMG_Load(pongian.tempChar2);
				if (sg2create->daballShad != NULL) pongian.ballShadow = TRUE;
			}

			if (strncmp(lineForLine, "PADDLE_SHADOW = ", 16) == 0)
			{
				stringCopyN(pongian.tempChar, lineForLine, 16, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				//fprintf(stderr, "PADDLE_SHADOW '%s'\n", pongian.tempChar2);
				sg2create->daplayerShad = IMG_Load(pongian.tempChar2);
				if (sg2create->daplayerShad != NULL) pongian.paddleShadow = TRUE;
			}
			
			if (strncmp(lineForLine, "THEME_AUTHOR = ", 15) == 0)
			{
				stringCopyN(xtheme.author, lineForLine, 15, strlen(lineForLine));
				removeNewlineFromString(xtheme.author);
			}
			
			if (strncmp(lineForLine, "THEME_NAME = ", 13) == 0)
			{
				stringCopyN(xtheme.name, lineForLine, 13, strlen(lineForLine));
				removeNewlineFromString(xtheme.name);
			}
			
			if (strncmp(lineForLine, "THEME_VERSION = ", 16) == 0)
			{
				stringCopyN(xtheme.version, lineForLine, 16, strlen(lineForLine));
				removeNewlineFromString(xtheme.version);
			}
/*			
			if ((strncmp(lineForLine, "INTRO_SND = ", 12) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 12, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->introSND) != 0)
				{
					logger.log(2, "introsnd error -> sound disabled");
					ret = 3;
				}
			}
			
			if ((strncmp(lineForLine, "PADDLE_BANG_SND = ", 18) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 18, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->paddlebangSND) != 0)
				{
					logger.log(2, "paddlebangsnd error -> sound disabled");
					ret = 3;
				}
			}
			
			if ((strncmp(lineForLine, "WALL_BANG_SND = ", 16) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 16, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->wallbangSND) != 0)
				{
					logger.log(2, "wallbangsnd error -> sound disabled");
					ret = 3;
				}
			}
			
			if ((strncmp(lineForLine, "GAME_OVER_LOOSE_SND = ", 22) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 22, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->gameoverLSND) != 0)
				{
					logger.log(2, "gameoversnd error -> sound disabled");
					ret = 3;
				}
			}
			
			if ((strncmp(lineForLine, "GAME_OVER_WIN_SND = ", 20) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 20, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->gameoverWSND) != 0)
				{
					logger.log(2, "gameoversnd error -> sound disabled");
					ret = 3;
				}
			}
			
			if ((strncmp(lineForLine, "MENUE_ENTER_SND = ", 18) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 18, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->menueenterSND) != 0)
				{
					logger.log(2, "menueentersnd error -> sound disabled");
					ret = 3;
				}
			}

			if ((strncmp(lineForLine, "MENUE_LEAVE_SND = ", 18) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 18, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->menueleaveSND) != 0)
				{
					logger.log(2, "menueleavesnd error -> sound disabled");
					ret = 3;
				}
			}

			if ((strncmp(lineForLine, "GAME_WIN_POINT_SND = ", 21) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 21, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->winpointSND) != 0)
				{
					logger.log(2, "winpointsnd error -> sound disabled");
					ret = 3;
				}
			}

			if ((strncmp(lineForLine, "GAME_LOOSE_POINT_SND = ", 23) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 23, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->loosepointSND) != 0)
				{
					logger.log(2, "loosepointsnd error -> sound disabled");
					ret = 3;
				}
			}

			if ((strncmp(lineForLine, "GAME_BALL_SHOT_SND = ", 21) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 21, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->shotSND) != 0)
				{
					logger.log(2, "shotsnd error -> sound disabled");
					ret = 3;
				}
			}

			if ((strncmp(lineForLine, "GAME_BALL_CRASH_SND = ", 22) == 0) && pongian.settings.soundEnabled)
			{
				stringCopyN(pongian.tempChar, lineForLine, 22, strlen(lineForLine));
				removeNewlineFromString(pongian.tempChar);
				sprintf(pongian.tempChar2, "%s/%s", pongian.fSkinPath, pongian.tempChar);
				if (LoadAndConvertSound(pongian.tempChar2, &obtained, &sg2create->crashSND) != 0)
				{
					logger.log(2, "crashsnd error -> sound disabled");
					ret = 3;
				}
			}*/
			
		}
		fclose(confFile);
	} else
	{
		logger.log(YAKWAILOG_ERROR, "wasn't able to open xtheme config file: %s.", confFilename);
	}
	return ret;
}

