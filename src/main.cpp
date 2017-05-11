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

#include <nds.h>

#include "game.h"
#include "audio.h"

int turn_timeout = -1;
// makes sure you don't accidentally mark two cells at once
bool touchable = false;

DsUI ui;

void onQuarterSecond() {
	touchable = 1;
	if (turn_timeout > 0)
		turn_timeout--;
}

int main(void) {
		timerStart(1, ClockDivider_256, 1<<15, onQuarterSecond);
		TCoordCell cell; TMove move;
		int pressed, held;

		Options options;
		Audio *audio = new Audio();
		OptionsUI options_ui(options, audio);

		Game *game = new Game(&ui, &options);

		// load background music
		if (options.sound & MUSIC_OPTION)
			audio->startBgMusic();

		while(1) {
			scanKeys();
			pressed = keysDown();
			held = keysHeld();

			// START -> new game
			if(pressed & KEY_START) {
				game->~Game();
				options = options_ui.getOptions();
				game = new Game(&ui, &options);
				if (options.timeout > 0) {
					turn_timeout = options.timeout*4;
					ui.showTime(turn_timeout/4);
				}
			}
			// D-PAD -> change options
			else if(pressed & (KEY_LEFT | KEY_RIGHT | KEY_UP | KEY_DOWN))
				options_ui.navigate(pressed);
			// TOUCH SCREEN -> turn
			else if (game->humansTurn()) {
				if ((pressed & KEY_TOUCH) && touchable) {
					ui.getMove(&cell, &move);
					game->turn(cell, move);
					touchable = false;
					if (options.timeout > 0)
						turn_timeout = options.timeout*4;
				}
				// turn timeout
				else if (turn_timeout == 0) {
					game->onTimeout();
					turn_timeout = -1;
				}
			}
			// A / auto -> AI turn
			else if (options.aiMode == AUTO_OPTION || (held|pressed) & KEY_A)
				game->startAI();
			if (game->running() && turn_timeout > 0) {
				ui.showTime(turn_timeout/4+1);
				if ((options.sound & EFFECTS_OPTION) &&
						(turn_timeout == 11 || turn_timeout == 7 || turn_timeout == 3))
					audio->startTimeoutSound();
			}
			swiWaitForVBlank();
		}

    return 0;
}
