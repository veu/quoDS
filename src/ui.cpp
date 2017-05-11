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

#include <stdio.h>

#include "ui.h"

#include "pieces.h"

DsUI::DsUI() {

	powerOn(POWER_ALL_2D);
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_3_2D);
	lcdMainOnBottom();
	vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_BG_0x06020000, VRAM_C_SUB_BG_0x06200000, VRAM_D_LCD);

	decompress(piecesBitmap, &this->pieces,  LZ77Vram);

	// init main screen

	// set up text background
	console = *consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 0, 1, true);

	// set up bitmap background
	int bg = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 2, 1);

	this->bgPtr = (u16*) bgGetGfxPtr(bg);

	// draw background
	dmaFillHalfWords(this->pieces[0], bgPtr, 256*256*2);

	// draw player boxes
	u16 player_bg = this->pieces[0] + RGB15(4, 4, 4);
	int i;
	for (i = 21; i < 34; i++)
		dmaFillHalfWords(player_bg, &bgPtr[196+i*256], 54*2);
	for (i = 85; i < 98; i++)
		dmaFillHalfWords(player_bg, &bgPtr[196+i*256], 54*2);
}

void
DsUI::drawLine(int x1, int y1, int x2, int y2, u16 color) {
	int t, width = 3;

	if (abs(y2 - y1) >= abs(x2 - x1)) {
		if (abs(y1 - y2) == abs(x1 - x2))
			width++;
		if (y1 > y2) {
			t = y1; y1 = y2; y2 = t;
			t = x1; x1 = x2; x2 = t;
		}
		if (x1 == x2)
			for (int i = y1; i < y2+width; i++)
				dmaFillHalfWords(color, &bgPtr[x1 + i*256], width*2);
		else
			for (int i = y1; i <= y2; i++) {
				t = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
				dmaFillHalfWords(color, &bgPtr[t + i*256], width*2);
			}
	}
	else {
		if (x1 > x2) {
			t = y1; y1 = y2; y2 = t;
			t = x1; x1 = x2; x2 = t;
		}
		if (y1 == y2)
			for (int i = y1; i < width+y1; i++)
				dmaFillHalfWords(color, &bgPtr[x1 + i*256], (x2-x1)*2);
		else
			for (int i = x1; i <= x2; i++) {
				t = (i - x1) * (y2 - y1) / (x2 - x1) + y1;
				for (int k = t; k < t+width; k++)
					bgPtr[i + k*256] = color;
			}
	}
}


inline u8
DsUI::merge(u16 c1, u16 c2) {
	return ((((c1) & 31)+c2) > 31) ? 31 : ((c1) & 31)+c2;
}

void
DsUI::paintPiece(TCell id, int x, int y, bool highlight) {
	// draw piece
	for (int i = 0; i < 16; i++)
		dmaCopy(&this->pieces[i*64+id*16], &bgPtr[x+(y+i)*256], 32);

	// draw highlight
	if (highlight) {
		u16 color, addcolor;
		for (int i = 0; i < 8; i++)
			for (int k = 0; k < 8; k++) {
				color = this->pieces[i+k*16*4+id*16];
				addcolor = highlight_map[i+k*8];
				bgPtr[x+i+(y+k)*256] = ARGB16(1, merge(color, addcolor), merge(color>>5, addcolor), merge(color>>10, addcolor));
			}
	}
}

void
DsUI::markCell(TCoordCell cell, TCell mark, bool new_mark) {
	paintPiece(mark, cell.col*17 + 3, cell.row*17 + 3, new_mark);
}

void
DsUI::showStartInfo(int first) {
	int i, k;

	// draw player info pieces
	paintPiece(BLUE_CELL, 211, 37, false);
	paintPiece(BLOCK_CELL, 211, 37+23, false);
	paintPiece(RED_CELL, 211, 101, false);
	paintPiece(BLOCK_CELL, 211, 101+23, false);

	// redraw board background
	for (i = 0; i < 256; i++)
		dmaFillHalfWords(this->pieces[0], &bgPtr[i*256], 190*2);

	// redraw board

	// second row
	for (i = 0; i < 11; i++)
		paintPiece(EMPTY_CELL, i*17 + 3, 20, false);
	// copy to first row
	for (i = 0; i < 17; i++)
		dmaCopy(&bgPtr[3+(i+20)*256], &bgPtr[20+(i+3)*256], 2*17*9);
	// copy to last row
	for (i = 0; i < 16; i++)
		dmaCopy(&bgPtr[3+(i+20)*256], &bgPtr[20+(i+173)*256], 2*17*9);
	// copy to remaining rows
	for (i = 17; i < 9*17; i++)
		dmaCopy(&bgPtr[3+(i+3)*256], &bgPtr[3+(i+20)*256], 2*17*11);

	// print piece count
	consoleSelect(&console);
	consoleClear();
	iprintf("\x1b[3;25H%s%s:\x1b[5;29H20\x1b[8;30H6", first == 0 ? "\7" : " ", TPLAYER_STRING[0]);
	iprintf("\x1b[11;25H%s%s:\x1b[13;29H20\x1b[16;30H6", first == 1 ? "\7" : " ", TPLAYER_STRING[1]);
}

void
DsUI::showTurnInfo(const Player& red, const Player& blue, int next) {
	consoleSelect(&console);
	iprintf("\x1b[5;29H%2d\x1b[8;30H%d", red.getAttackLeft(), red.getBlockLeft());
	iprintf("\x1b[13;29H%2d\x1b[16;30H%d", blue.getAttackLeft(), blue.getBlockLeft());
	iprintf("\x1b[3;25H%s\x1b[11;25H%s", next == 0 ? "\7" : " ", next == 1 ? "\7" : " ");
}

void
DsUI::showSquare(TWinnerState state, TCoordCell& cell1, TCoordCell& cell2, TCoordCell& cell3, TCoordCell& cell4) {
	u16 color = (state == BLUE_WINNER) ? ARGB16(1, 0, 0, 31) : ARGB16(1, 31, 0, 0);
	drawLine(cell1.col*17+10, cell1.row*17+10, cell2.col*17+10, cell2.row*17+10, color);
	drawLine(cell2.col*17+10, cell2.row*17+10, cell3.col*17+10, cell3.row*17+10, color);
	drawLine(cell3.col*17+10, cell3.row*17+10, cell4.col*17+10, cell4.row*17+10, color);
	drawLine(cell4.col*17+10, cell4.row*17+10, cell1.col*17+10, cell1.row*17+10, color);
}

void
DsUI::showWinner(TWinnerState state) {
	consoleSelect(&console);
	switch (state) {
		case BLUE_WINNER:
			iprintf("\x1b[20;25HBlue \x1b[21;25Hwon!  ");
			break;
		case RED_WINNER:
			iprintf("\x1b[20;25HRed  \x1b[21;25Hwon!  ");
			break;
		case DRAW_WINNER:
			iprintf("\x1b[20;25HDraw!\x1b[21;25H      ");
	}
}

void
DsUI::showTime(int time) {
	consoleSelect(&this->console);
	iprintf("\x1b[20;25HLeft:\x1b[21;25H%2d sec", time);
}

void
DsUI::getMove(TCoordCell *cell, TMove *move) {
  touchPosition touch;
  // read the touchscreen coordinates
	touchRead(&touch);
	if (touch.px < 3 || touch.py < 3) {
		cell->col = -1; cell->row = -1; *move = NONE_MOVE;
	}
	else {
		cell->col = (touch.px - 3) / 17;
		cell->row = (touch.py - 3) / 17;
		*move = (keysHeld() & (KEY_L | KEY_R)) ? BLOCK_MOVE : ATTACK_MOVE;
	}
}
