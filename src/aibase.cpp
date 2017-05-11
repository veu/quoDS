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

#include <iostream>
#include <stdlib.h>

#include "aibase.h"

AIBase::AIBase (const Player& _player, const Board& _board) : player (_player),
board (_board) { }

AIBase::~AIBase () { }

TMove AIBase::RandomStep (int cellSpread)
{
#ifndef NDEBUG
    std::cout << "AIBase: RandomStep (" << cellSpread << ')' << std::endl;
#endif // NDEBUG

    if (player.getAttackLeft () == 0)
        return NONE_MOVE;

    int row, col;

    do
    {
        row = Board::MAX_ROW / 2;
        col = Board::MAX_COL / 2;

        row += (rand () % 2 ? 1 : -1) * (rand () % cellSpread + rand () % 2);
        col += (rand () % 2 ? 1 : -1) * (rand () % cellSpread + rand () % 2);

        if (!Board::IsValidSquare (row, col) ||
                board.getCell (row, col) != EMPTY_CELL)
            continue;
        break;
    }

    while (true);

    lastMove = TCoordCell (row, col);

    return ATTACK_MOVE;
}
