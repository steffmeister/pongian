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
* pongian.h - contains very important stuff, defaults, defines, structs,...*
*             please read also the readme file.                          *
****************************************************************************/

#ifndef __PONGIAN_H_
#define __PONGIAN_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "yakwailog/yakwailog.hxx"

#include "brick.hxx"
#include "SFont/SFont.h"
#include "xtheme.hxx"
#include "ball.hxx"
#include "paddle.hxx"

#define PONGIAN_VERSION "v0.3.2-2"
#define PONGIAN_TITLE "pongian"
#define PONGIAN_CONF "pongian.conf"
#define BRETTL_LENGTH 50
#define BRETTL_HEIGHT 10
#define BALL_WIDTH  10
#define DEFAULT_SPEED  3
#define DEFAULT_BALL_SPEED 2

#define TICK_INTERVAL 15 // length of a game cycle, in milli-seconds

#define TRUE 1
#define FALSE 0
#define TWO 2
#define ESCAPE SDLK_ESCAPE
#define LEFT 4
#define STOP 5
#define RIGHT 6

#define KBD1     1
#define KBD2     2
#define MOUSE    3
#define JOYSTICK 4
#define PADDLE1_CTRL  KBD1
#define PADDLE2_CTRL  KBD2
#define MAXCTRL JOYSTICK

#define PLAYER1_LEFT  SDLK_LEFT
#define PLAYER1_RIGHT SDLK_RIGHT
#define PLAYER1_STOP  SDLK_DOWN
#define PLAYER1_FIRE  SDLK_UP
#define PLAYER2_LEFT  SDLK_a
#define PLAYER2_RIGHT SDLK_d
#define PLAYER2_STOP  SDLK_s
#define PLAYER2_FIRE  SDLK_w
#define GAMES2WIN 5
#define XMAX 640     //  pongian just works in this mode:
#define YMAX 480     //  640x480!
#define VIDEO_MODE SDL_DOUBLEBUF
#define VIDEO_BITS 16
#define OKE_KEY SDLK_RETURN

#define SHADOW_X 15  // fix: should this be theme-dependent?
#define SHADOW_Y 5

struct setup
{
	char id[3];
	int x;
	int y;
	unsigned char bpp;
	Uint32 flags;
	char themePath[50];
	char games2win;
	char p1name[20];
	char p2name[20];
	char p1ctrl;
	char p2ctrl;
	char soundEnabled;
};

struct environment
{
	struct setup settings;
	//char path[EXEC_PATH_LEN];
	char winner;
	char p1wins;
	char p2wins;
	char actualPlayer;
	char length;
	char height;
	char diameter;
	char saveSettings;
	char cpuPaddleEnabled;
	char tempName[20];
	char *execPath;
	char *fSkinPath;
	char *configFile;
	SDLKey p1Left;
	SDLKey p1Up;
	SDLKey p1Right;
	SDLKey p1Down;
	SDLKey p2Left;
	SDLKey p2Up;
	SDLKey p2Right;
	SDLKey p2Down;
	unsigned char actualLevel;
	char joyEnabled;
	char *tempChar;
	char *tempChar2;
	char ballShadow;
	char paddleShadow;
	SDL_Surface *iconPic;
	char *picFile;
};





//#include "ball.cpp"
//ball kugel;

void cleanUp(void);
void SetThings(void);
void ResetThings(void);
void ReallyResetThings(void);
void ShowPoints(void);
char game(void);
void Intro(void);
void Outro(void);
void quitPongian(char mode);
char mainMenue(char selected);
char optionsMenue(char selected);
char videoMenue(char selected);
char gameMenue(char selected);
char ctrlMenue(char selected);
void handleArgs(char argN, char *argT[]);
void initEnv(void);
void waitForKey(SDLKey key);
void winnerScreen(char playerd);
void aboutScreen(void);
char uniGameOrama(void);
Uint32 TimeLeft(void);
void saveSettings(void);
void loadSettings(void);
void inputScreen(char nameNr);
void announceGame(void);
void setControls(void);
void skinInfo(void);
void initSound(void);
int getCleanExecPath(char *pathAndExec);
void SFont_Read (SDL_Surface *Surface, const SFont_Font *Font, int x, int y, char *text);
char removeNewlineFromString(char *string);
char stringCopyN(char *string2, char *string1, int start, int end);
void redrawInputScreen(void);

extern SDL_Joystick *joystick;
extern yakwailog logger;
extern SDL_AudioSpec desired, obtained;
extern SDL_Surface *screen;
extern SDL_Event event;
extern struct environment pongian;
extern struct pongian_xtheme xtheme;
extern class ball kugel;
extern class brett player1;
extern class brett player2;

#endif

