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

#include "game.h"

Game::Game(GameUI *ui, Options *options) {
	this->ui = ui;
	this->options = options;
	board = new Board();
	cur = 0;
	players[0] = new Player(BLUE_PLAYER);
	players[1] = new Player(RED_PLAYER);

	for (int i = 0; i < 2; i++) {
		ais[i] = NULL;
		if (options->players[i] == EASYAI_OPTION)
			ais[i] = new AIEasy (*players[i], *board);
		else if (options->players[i] == HARDAI_OPTION)
			ais[i] = new AIHard (*players[i], *board);
	}

	lastCell = TCoordCell(-1, -1);
	lastMark = EMPTY_CELL;
	state = PLAY_STATE;
	ui->showStartInfo(cur);
}

Game::~Game() {
	delete board;
	delete players[0];
	delete players[1];
	delete ais[0];
	delete ais[1];
}

void
Game::turn(TCoordCell cell, TMove move) {
	if (state != PLAY_STATE)
		return;
	if (!board->IsValidSquare(cell) || board->getCell(cell.row, cell.col) != EMPTY_CELL)
		return;

	TCell mark;
	if (move == BLOCK_MOVE) {
		if (players[cur]->getBlockLeft() == 0)
			return;
		mark = BLOCK_CELL;
		players[cur]->DecreaseBlock();
	}
	else {
		if (players[cur]->getAttackLeft() == 0)
			return;
		mark = Board::PlayerToCell(players[cur]->getPlayer());
		players[cur]->DecreaseAttack();
	}

	board->setCell(cell.row, cell.col, mark);

	if (mark != BLOCK_CELL) {
		if (lastMark != EMPTY_CELL)
			ui->markCell(lastCell, lastMark, false);
		lastMark = mark;
		lastCell = cell;
	}
	ui->markCell(cell, mark, mark != BLOCK_CELL);

	if (mark != BLOCK_CELL) {
		if (!checkEnd())
			cur = (cur + 1) % 2;
	}

	ui->showTurnInfo(*players[0], *players[1], cur);
}

void Game::startAI ()
{
	if (state != PLAY_STATE || options->players[cur] == HUMAN_OPTION)
		return;

	TMove move = ais[cur]->Step();
	turn(ais[cur]->getLastMove(), move);
}

bool
Game::checkEnd() {
	TCoordCell cell1, cell2, cell3, cell4;
	TWinnerState wstate = NONE_WINNER;
	TCell currentPlayerCell = Board::PlayerToCell(players[cur]->getPlayer());
	bool square = false;

	if (board->FindSquare(&cell1, &cell2, &cell3, &cell4, currentPlayerCell)) {
		wstate = (players[cur]->getPlayer() == BLUE_PLAYER ? BLUE_WINNER :
						(players[cur]->getPlayer() == RED_PLAYER ? RED_WINNER : NONE_WINNER));
		square = true;
	}
	else if ((players[0]->getAttackLeft() == 0) && (players[1]->getAttackLeft() == 0)) {
		if (players[0]->getBlockLeft() > players[1]->getBlockLeft())
			wstate = BLUE_WINNER;
		else if (players[0]->getBlockLeft () < players[1]->getBlockLeft ())
			wstate = RED_WINNER;
		else
			wstate = DRAW_WINNER;
	}

	if (wstate == NONE_WINNER)
		return false;

	state = END_STATE;
	ui->markCell(lastCell, lastMark, false);
	if (square)
		ui->showSquare(wstate, cell1, cell2, cell3, cell4);
	ui->showWinner(wstate);
	return true;
}

bool
Game::humansTurn() {
	return state == PLAY_STATE && options->players[cur] == HUMAN_OPTION;
}

bool
Game::running() {
	return state == PLAY_STATE;
}

void
Game::onTimeout() {
	state = END_STATE;
	TWinnerState wstate = (players[cur]->getPlayer() == BLUE_PLAYER ? RED_WINNER :
						(players[cur]->getPlayer() == RED_PLAYER ? BLUE_WINNER : NONE_WINNER));
	ui->showWinner(wstate);
	options->timeout = 0;
	return ;
}
