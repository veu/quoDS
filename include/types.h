/* Copyright (C)  2006-2009, Vlad Volodin <vest.84@gmail.com>
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

#ifndef _TYPES_H
#define	_TYPES_H

enum TGameState
{
    END_STATE, // Game was end
    PLAY_STATE // Game is playing
};

enum TPlayerOption
{
    HUMAN_OPTION,
    EASYAI_OPTION,
    HARDAI_OPTION
};

// Is needed for winner announcement

enum TWinnerState
{
    NONE_WINNER,
    BLUE_WINNER,
    RED_WINNER,
    DRAW_WINNER
};

static const char * TPLAYER_STRING[] = {"Blue", "Red"};
static const char * TPLAYEROPTION_STRING[] = {"Human", "Easy", "Hard"};

#endif	/* _TYPES_H */

