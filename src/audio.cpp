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

#include "audio.h"

// note: the next line assumes that the background music is the first
mm_ds_sample bgSample = {2995, 202533, 0, 1, 1<<9, (void*)&soundbank_bin};
// sample rate handle volume panning
mm_sound_effect bgSound = { (mm_word)&bgSample, 1<<10, 0, 200, 127};
mm_sound_effect toSound = { SFX_TIMEOUT, 1<<10, 0, 120, 127};

Audio::Audio() {
	mmInitDefaultMem( (mm_addr)soundbank_bin );
	mmLoadEffect(SFX_BGMUSIC);
	mmLoadEffect(SFX_TIMEOUT);
	bgMusic = -1;
}

void
Audio::startBgMusic() {
	if (bgMusic == -1)
		bgMusic = mmEffectEx(&bgSound);
}

void
Audio::stopBgMusic() {
	if (bgMusic != -1) {
		mmEffectCancel(bgMusic);
		bgMusic = -1;
	}
}

void
Audio::startTimeoutSound() {
	mmEffectEx(&toSound);
}
