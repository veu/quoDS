/* Copyright (C) 2009, Rebecca König
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

#include <nds.h>
#include <stdio.h>

#include "options.h"

#include "top.h"

Options::Options() {
	this->players[0] = HUMAN_OPTION;
	this->players[1] = HUMAN_OPTION;
	this->aiMode = AUTO_OPTION;
	this->sound = ALL_OPTION;
	this->timeout = 0;
}

OptionsUI::OptionsUI(Options options, Audio *audio) {
	this->options = options;
	this->audio = audio;
	this->console = *consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 2, 0, false);
	this->selected = 0;
	this->changed = false;

	// init sub screen
	int id = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 2,0);
	decompress(topBitmap, bgGetGfxPtr(id),  LZ77Vram);

	consoleClear();
	iprintf("\x1b[10;3HBlue AI:\x1b[11;3HRed  AI:\x1b[12;3HAI Mode:\x1b[13;3HTimeout:\x1b[14;3HSound:");
	printCurrent();
	iprintf("\x1b[17;1HWelcome to quoDS\x1b[18;1H  version 1.2!");
	iprintf("\x1b[20;1H quod.23inch.de");
}

void
OptionsUI::printCurrent() {
	consoleSelect(&this->console);
	iprintf("\x1b[10;2H%s", selected==0 ? "\7" : " ");
	iprintf("\x1b[11;2H%s", selected==1 ? "\7" : " ");
	iprintf("\x1b[12;2H%s", selected==2 ? "\7" : " ");
	iprintf("\x1b[13;2H%s", selected==3 ? "\7" : " ");
	iprintf("\x1b[14;2H%s", selected==4 ? "\7" : " ");

	// AIs
	for (int i = 0; i < 2; i++) {
		iprintf((i == 0) ? "\x1b[10;12H" : "\x1b[11;12H");
		switch (options.players[i]) {
			case HUMAN_OPTION: iprintf("none"); break;
			case EASYAI_OPTION: iprintf("easy"); break;
			case HARDAI_OPTION: iprintf("hard"); break;
		}
	}

	// AI mode
	switch (options.aiMode) {
		case AUTO_OPTION: iprintf("\x1b[12;12Hauto"); break;
		case BUTTON_OPTION: iprintf("\x1b[12;12Hon A"); break;
	}

	// timeout
	if (options.timeout == 0)
		iprintf("\x1b[13;12Hnone  ");
	else
		iprintf("\x1b[13;12H%u sec", options.timeout);

	// sound
	switch (options.sound) {
		case ALL_OPTION:     iprintf("\x1b[14;12Hall    "); break;
		case MUSIC_OPTION:   iprintf("\x1b[14;12Hmusic  "); break;
		case EFFECTS_OPTION: iprintf("\x1b[14;12Heffects"); break;
	}

	if (this->changed) {
		iprintf("\x1b[16;1HSettings have been\x1b[17;1Hchanged and will");
		iprintf("\x1b[18;1Htake effect after\x1b[19;1Hthe next restart.");
	} else {
		iprintf("\x1b[16;1H                  \x1b[17;1H                ");
		iprintf("\x1b[18;1H                 \x1b[19;1H                 ");
	}
	iprintf("\x1b[20;1H                 ");
}

void
OptionsUI::navigate(int key) {
	if (key & KEY_UP)
		selected = selected == 0 ? 4 : (selected-1);
	else if (key & KEY_DOWN)
		selected = (selected+1) % 5;
	else {
		switch (selected) {
			case 0: case 1:
				switch (options.players[selected]) {
					case HUMAN_OPTION: options.players[selected] = key & KEY_RIGHT ? EASYAI_OPTION : HARDAI_OPTION; break;
					case EASYAI_OPTION: options.players[selected] = key & KEY_RIGHT ? HARDAI_OPTION : HUMAN_OPTION; break;
					case HARDAI_OPTION: options.players[selected] = key & KEY_RIGHT ? HUMAN_OPTION : EASYAI_OPTION; break;
				}
				this->changed = true;
				break;
			case 2:
				switch (options.aiMode) {
					case AUTO_OPTION: options.aiMode = BUTTON_OPTION; break;
					case BUTTON_OPTION: options.aiMode = AUTO_OPTION; break;
				}
				this->changed = true;
				break;
			case 3:
				if (key & KEY_RIGHT)
					options.timeout = (options.timeout + 10) % 70;
				else
					options.timeout = options.timeout>0 ? options.timeout - 10 : 60;
				this->changed = true;
				break;
			case 4:
				switch (options.sound) {
					case ALL_OPTION:
						if (key & KEY_RIGHT)
							options.sound = MUSIC_OPTION;
						else {
							options.sound =  EFFECTS_OPTION;
							audio->stopBgMusic();
						}
						break;
					case MUSIC_OPTION:
						if (key & KEY_LEFT)
							options.sound = ALL_OPTION;
						else {
							options.sound =  EFFECTS_OPTION;
							audio->stopBgMusic();
						}
						break;
					case EFFECTS_OPTION:
						options.sound = key & KEY_RIGHT ? ALL_OPTION : MUSIC_OPTION;
						audio->startBgMusic();
						break;
				}
				break;
		}
	}
	printCurrent();
}

Options
OptionsUI::getOptions() {
	this->changed = false;
	printCurrent();
	return this->options;
}
