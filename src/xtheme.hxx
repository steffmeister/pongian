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
* xtheme.h - eXtended pongian THEME: THE struct                            *
*            implented in v0.3pre-2 and later.                             *
****************************************************************************/

#ifndef __PONGIAN_XTHEME_H_
#define __PONGIAN_XTHEME_H_

#include <SDL/SDL.h>
#include "SFont/SFont.h"
#include "pongian.hxx"
#include "yakwailog/yakwailog.hxx"

struct pongian_xtheme
{
  SDL_Surface *menue_background;
  SDL_Surface *game_background;
  SDL_Surface *intro_background;
  SDL_Surface *outro_background;
  SDL_Surface *daballSkin;
  SDL_Surface *daballShad;
  SDL_Surface *daplayer1Skin;
  SDL_Surface *daplayer2Skin;
  SDL_Surface *daplayerShad;
  SDL_Surface *slFont; //myFont01
  SDL_Surface *dsFont; //myFont02
  SFont_Font *selFont; //Font01
  SFont_Font *dslFont; //Font02
  char author[50];
  char name[50];
  char version[50];
  char configVer[5];
//  sound_t introSND, paddlebangSND, wallbangSND, gameoverWSND, gameoverLSND;
//  sound_t menueenterSND, menueleaveSND, winpointSND, loosepointSND, shotSND;
//  sound_t crashSND;
};

char xtheme_freeTheme(struct pongian_xtheme *sg2free);
char xtheme_loadTheme(struct pongian_xtheme *sg2create);


#endif

