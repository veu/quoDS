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

#ifndef _UI_H
#define	_UI_H

#include <nds.h>

#include "board.h"
#include "types.h"

static u8 highlight_map[64] = {
	  6,  6,  0,  0,  0,  0,  6,  6,
	  6, 17,  7,  1,  1,  7, 17,  6,
	  0,  7, 25, 17, 17, 25,  7,  0,
	  0,  1, 17, 26, 26, 17,  1,  0,
	  0,  1, 17, 26, 26, 17,  1,  0,
	  0,  7, 25, 17, 17, 25,  7,  0,
	  6, 17,  7,  1,  1,  7, 17,  6,
	  6,  6,  0,  0,  0,  0,  6,  6};

class GameUI
{
	public:
		virtual void showStartInfo(int cur) = 0;
		virtual void showTurnInfo(const Player& red, const Player& blue, int next) = 0;
		virtual void showSquare(TWinnerState state, TCoordCell& cell1, TCoordCell& cell2, TCoordCell& cell3, TCoordCell& cell4) = 0;
		virtual void showWinner(TWinnerState state) = 0;
		virtual void markCell(TCoordCell cell, TCell mark, bool new_mark) = 0;
};

class DsUI : public GameUI
{
	public:
		DsUI();

		void showStartInfo(int first);
		void showTurnInfo(const Player& red, const Player& blue, int next);
		void showSquare(TWinnerState state, TCoordCell& cell1, TCoordCell& cell2, TCoordCell& cell3, TCoordCell& cell4);
		void showWinner(TWinnerState state);
		void markCell(TCoordCell cell, TCell mark, bool new_mark);

		void getMove(TCoordCell *cell, TMove *move);
		void showTime(int time);
	private:
		u16 *bgPtr;
		u16 pieces[256*4];
		void paintPiece(TCell id, int x, int y, bool highlight);
		void drawLine(int x1, int y1, int x2, int y2, u16 color);
		inline u8 merge(u16 c1, u16 c2);
		PrintConsole console;
};

#endif	/* _UI_H */
