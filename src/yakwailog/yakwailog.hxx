/***************************************************************************
    yakwailog - a small and handsome logger
    Copyright (C) 2004, 2005 Stefan Zidar

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
* yakwailog.h - logman class and defines (v0.2b)                           *
****************************************************************************/

#ifndef __YAKWAILOG_
#define __YAKWAILOG_

#include <stdio.h>
#include <stdarg.h>


#define YAKWAILOG_INFO    0
#define YAKWAILOG_WARNING 1
#define YAKWAILOG_ERROR   2

class yakwailog
{
	public:
		yakwailog(void);
		~yakwailog(void);
		
		char init(char *application, char *filename, char mode);
		char log(char value, const char *output, ...);
		char end(void);
	
	private:
		char set;
		char *appName;
		FILE *logFile;
		char logMode;
};

/*
 * mode can be:
 * 0 = no logging
 * 1 = normal file logging
 * 2 = normal "console" debugging output
 */
 
#endif
