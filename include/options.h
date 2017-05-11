/* Copyright (C) 2009, Rebecca König
 * Copyright (C) gquod 2006-2009, Vlad Volodin <vest.84@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _OPTIONS_H
#define	_OPTIONS_H

#include <nds.h>

#include "audio.h"
#include "types.h"

enum TAiModeOption
{
    AUTO_OPTION = 0, // AIs play automatically
    BUTTON_OPTION = 1 // User has to press a button
};

enum TSoundOption
{
    MUSIC_OPTION = 1, // play music only
    EFFECTS_OPTION = 2, // play sound effects only
    ALL_OPTION = 3 // play everything
};



class Options
{
	public:
		TPlayerOption players[2];
		TAiModeOption aiMode;
		TSoundOption sound;
		u8 timeout;
		Options();
};

class OptionsUI
{
	public:
		OptionsUI(Options options, Audio *audio);
		void navigate(int key);
		Options getOptions();
	private:
		void printCurrent();
		Options options;
		Audio *audio;
		PrintConsole console;
		int selected;
		bool changed;
};

#endif	/* _OPTIONS_H */
