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

#include <map>
#include <iostream>
#include <stdlib.h>

#include "aihard.h"

AIHard::AIHard (const Player& player, const Board& board) : AIBase (player, board) { }

AIHard::~AIHard () { }

TMove AIHard::Step ()
{
#ifndef NDEBUG
    std::cout << "AIHard: Step" << std::endl;
#endif // NDEBUG

    CheckImportant (playerImportant, player.getPlayer ());

    TIntMap::iterator pos = HasCritical ();
    if (pos != playerImportant.end ())
    {
        lastMove = TCoordCell (pos->first / Board::MAX_ROW, pos->first % Board::MAX_COL);
        return ATTACK_MOVE;
    }

    CheckImportant (opponentImportant,
                    player.getPlayer () == RED_PLAYER ? BLUE_PLAYER : RED_PLAYER);

    TMove step = CheckOpponentAttack ();
    if (step != NONE_MOVE)
        return step;

    if (CheckOpponentImportant () == ATTACK_MOVE)
        return ATTACK_MOVE;

    RandomStep ();

    return ATTACK_MOVE;
}

// Checks move which can make possible square for "important" player

void AIHard::CheckImportant (TIntMap& important, TPlayer playerColor)
{
#ifndef NDEBUG
    std::cout << "AIHard: CheckImportant" << std::endl;
#endif // NDEBUG

    int row1, col1, row2, col2, row3, col3, // corners of winner square
            rowDiffer, colDiffer; //col and row difference between 2 corners (1 and 2)

    important.clear ();

    for (row1 = 0; row1 < Board::MAX_ROW; row1++)
        for (col1 = 0; col1 < Board::MAX_COL; col1++)
        {
            if (!Board::IsValidSquare (row1, col1) ||
                    (board.getCell (row1, col1) != Board::PlayerToCell (playerColor)))
                continue;

            for (row2 = 0; row2 < Board::MAX_ROW; row2++)
                for (col2 = 0; col2 < Board::MAX_COL; col2++)
                {
                    if (!Board::IsValidSquare (row2, col2) ||
                            (board.getCell (row2, col2) != Board::PlayerToCell (playerColor)) ||
                            ((col1 == col2) && (row1 == row2)))
                        continue;

                    if (TCoordCell (row2, col2) < TCoordCell (row1, col1))
                        continue;

                    rowDiffer = row2 - row1;
                    colDiffer = col1 - col2;

                    Pair (important, playerColor,
                          row1 + colDiffer, col1 + rowDiffer,
                          row2 + colDiffer, col2 + rowDiffer);
                    Pair (important, playerColor,
                          row1 - colDiffer, col1 - rowDiffer,
                          row2 - colDiffer, col2 - rowDiffer);

                    if ((rowDiffer + colDiffer) % 2 == 0)
                    {
                        row3 = (row1 + row2 + colDiffer) / 2;
                        col3 = (col1 + col2 + rowDiffer) / 2;

                        Pair (important, playerColor,
                              row3, col3,
                              row3 - colDiffer, col3 - rowDiffer);

                    }

                    if ((rowDiffer % 2 == 0) && (colDiffer % 2 == 0))
                    {
                        int col = (col1 + col2) / 2,
                                row = (row1 + row2) / 2;

                        if (board.getCell (row, col) == EMPTY_CELL)
                        {
                            int row5 = (row1 + row2 - colDiffer) / 2,
                                    col5 = (col1 + col2 - rowDiffer) / 2;

                            if (board.getCell (row5, col5) == Board::PlayerToCell (playerColor))
                            {
#ifndef NDEBUG
                                std::cout << "AIHard: CheckImportant: Spoon!!!" << std::endl;
                                std::cout << row1 << ", " << col1 << " |\n"
                                        << row << ", " << col << " - spoon!\n"
                                        << row2 << ", " << col2 << " |\n"
                                        << row5 << ", " << col5 << std::endl;
#endif // NDEBUG
                                InsertCell (important, row, col, SPOON_PRIORITY);
                            }
                        }
                    }
                }
        }
}

// Checks possible moves, to make a square (from 2 or 3 corners)
// and put them into "important" map with step priority

void AIHard::Pair (TIntMap& important, TPlayer playerColor,
                   int row3, int col3, int row4, int col4)
{
#ifndef NDEBUG
    std::cout << "AIHard: Pair" << std::endl;
#endif // NDEBUG

    if (!Board::IsValidSquare (row3, col3) ||
            !Board::IsValidSquare (row4, col4))
        return;

    if ((board.getCell (row3, col3) == EMPTY_CELL) &&
            (board.getCell (row4, col4) == EMPTY_CELL))
    {
        InsertCell (important, row3, col3, STANDARD_PRIORITY);
        InsertCell (important, row4, col4, STANDARD_PRIORITY);
    }

    if ((board.getCell (row3, col3) == EMPTY_CELL) &&
            (board.getCell (row4, col4) == Board::PlayerToCell (playerColor)))
        InsertCell (important, row3, col3, WIN_PRIORITY);

    if ((board.getCell (row3, col3) == Board::PlayerToCell (playerColor)) &&
            (board.getCell (row4, col4) == EMPTY_CELL))
        InsertCell (important, row4, col4, WIN_PRIORITY);
}

// Insert cell to important map with current coordinats and priority

int AIHard::InsertCell (TIntMap& important, int row, int col, int priority)
{
#ifndef NDEBUG
    std::cout << "AIHard: InsertCell(" << row << ", " << col << ", " << priority << ") = ";
#endif // NDEBUG

    int new_priority = important[row * Board::MAX_COL + col];
    new_priority += priority;
    important[row * Board::MAX_COL + col] = new_priority;

#ifndef NDEBUG
    std::cout << new_priority << std::endl;
#endif // NDEBUG

    return new_priority;
}

// If current player has one move to win, he steps it

TIntMap::iterator AIHard::HasCritical ()
{
#ifndef NDEBUG
    std::cout << "AIHard: HasCritical" << std::endl;
#endif // NDEBUG

    for (TIntMap::iterator pos = playerImportant.begin (); pos != playerImportant.end (); pos++)
    {
        if (pos->second >= WIN_PRIORITY)
            return pos;
    }
    return playerImportant.end ();
}

// Check possible Opopnent Attack steps with which he can win, or his future
// steps have a high priority

TMove AIHard::CheckOpponentAttack ()
{
#ifndef NDEBUG
    std::cout << "AIHard: CheckOpponentAttack" << std::endl;
#endif // NDEBUG

    if (player.getAttackLeft () == 0)
        return NONE_MOVE;

    int maxPriority = -1, blockCell;

    // if opponent can win, trying to block him
    for (TIntMap::iterator pos = opponentImportant.begin (); pos != opponentImportant.end (); pos++)
        if (pos->second >= WIN_PRIORITY)
        {
            int currentPriority = 0;
            if (playerImportant[pos->first] > 0)
                currentPriority = playerImportant[pos->first];
            if (currentPriority > maxPriority)
            {
                blockCell = pos->first;
                maxPriority = currentPriority;
            }
        }

    if (maxPriority < 0)
        return NONE_MOVE;

    for (TIntMap::iterator pos = opponentImportant.begin (); pos != opponentImportant.end (); pos++)
        if ((pos->first != blockCell) && (pos->second >= WIN_PRIORITY))
        {
            if (player.getBlockLeft () > 0)
            {
                lastMove = TCoordCell (pos->first / Board::MAX_ROW,
                                       pos->first % Board::MAX_COL);
                return BLOCK_MOVE;
            }
        }

    lastMove = TCoordCell (blockCell / Board::MAX_ROW,
                           blockCell % Board::MAX_COL);
    return ATTACK_MOVE;
}

TMove AIHard::CheckOpponentImportant ()
{
#ifndef NDEBUG
    std::cout << "AIHard: CheckOpponentImportant" << std::endl;
#endif // NDEBUG

    if (player.getAttackLeft () == 0)
        return NONE_MOVE;

    int maxPriority = -1, blockCell;

    for (TIntMap::iterator pos = playerImportant.begin (); pos != playerImportant.end (); pos++)
    {
        int currentPriority = pos->second;
        if (opponentImportant[pos->first] > 0)
            currentPriority += opponentImportant[pos->first];
        if (currentPriority > maxPriority)
        {
            maxPriority = currentPriority;
            blockCell = pos->first;
        }
    }

    for (TIntMap::iterator pos = opponentImportant.begin (); pos != opponentImportant.end (); pos++)
    {
        int currentPriority = pos->second;
        if (playerImportant[pos->first] > 0)
            currentPriority += playerImportant[pos->first];
        if (currentPriority > maxPriority)
        {
            maxPriority = currentPriority;
            blockCell = pos->first;
        }
    }

    if (maxPriority < 0)
        return NONE_MOVE;

    lastMove = TCoordCell (blockCell / Board::MAX_ROW,
                           blockCell % Board::MAX_COL);
    return ATTACK_MOVE;
}
