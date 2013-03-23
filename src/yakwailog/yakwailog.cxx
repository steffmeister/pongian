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
* yakwailog.cpp - the main source file (include this from your project!)   *
*              version 0.2b                                                *
****************************************************************************/

#include "yakwailog.hxx"

yakwailog :: yakwailog(void)
{
	set = 0;
	appName = NULL;
	logFile = NULL;
}

yakwailog :: ~yakwailog(void)
{}

char yakwailog :: init(char *application, char *filename, char mode)
{
	char ret = 0;
	appName = application;
	logMode = mode;
	if (logMode == 1) logFile = fopen(filename, "a");
	if (logMode == 2) logFile = stderr;
	if (logFile != NULL)
	{
		ret = 1;
		set = 1;
		if (logMode == 1) log(0, "yakwailog: initialized ------>");
	}
	return ret;
}

char yakwailog :: log(char value, const char *output, ...)
{
	char ret = 0;
	char lineHead[5];
	va_list args;
	va_start(args, output);
	if (set == 1)
	{	
		if ((value > YAKWAILOG_ERROR) || (value < YAKWAILOG_INFO)) value = YAKWAILOG_ERROR;
		if (value == YAKWAILOG_INFO) sprintf(lineHead, "(==)");
		if (value == YAKWAILOG_WARNING) sprintf(lineHead, "(WW)");
		if (value == YAKWAILOG_ERROR) sprintf(lineHead, "(EE)");
		fprintf(logFile, "%s %s: ", lineHead, appName);
		vfprintf(logFile, output, args);
		fprintf(logFile, "\n");
		ret = 1;
	}
	va_end(args);
	return ret;
}

char yakwailog :: end(void)
{
	char ret = 0;
	if (set == 1)
	{
		if (logMode == 1) fclose(logFile);
		ret = 1;
		set = 0;
	}
	return ret;
}
