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

#include "aieasy.h"

AIEasy::AIEasy (const Player& player, const Board& board) : AIBase (player, board), firstStep (true) { }

AIEasy::~AIEasy () { }

TMove AIEasy::Step ()
{
    // blockMoves.clear ();

    if (firstStep)
    {
        firstStep = false;

        return RandomStep ();
    }

    if (WinStep () == ATTACK_MOVE)
        return ATTACK_MOVE;

    BuildOpponentMoves ();
    BuildPlayerMoves ();

#ifndef NDEBUG
    std::cout << "opponentMoves = " << opponentMoves.size () << std::endl;
    std::cout << "playerMoves = " << playerMoves.size () << std::endl;
#endif // NDEBUG

    if (BlockAsAttackStep () == ATTACK_MOVE)
        return ATTACK_MOVE;

    if ((opponentMoves.size () > 1) && (BlockWithBlockerStep () == BLOCK_MOVE))
        return BLOCK_MOVE;

    if (BlockWithAttackerStep () == ATTACK_MOVE)
        return ATTACK_MOVE;

    if (PlayerMoveStep () == ATTACK_MOVE)
        return ATTACK_MOVE;

    if (BlockOpponentPotential () == ATTACK_MOVE)
        return ATTACK_MOVE;

    if (StartPlayerPotential () == ATTACK_MOVE)
        return ATTACK_MOVE;

    return RandomStep (7);
}

TMove AIEasy::WinStep ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: WinStep" << std::endl;
#endif // NDEBUG

    int row1, col1, row2, col2, row3, col3, row4, col4, // corners of winner square
            rowDiffer, colDiffer; //col and row difference between 2 corners (1 and 2)

    for (row1 = 0; row1 < Board::MAX_ROW; row1++)
        for (col1 = 0; col1 < Board::MAX_COL; col1++)
        {
            if (!Board::IsValidSquare (row1, col1) ||
                    (board.getCell (row1, col1) != Board::PlayerToCell (player.getPlayer ())))
                continue;

            for (row2 = 0; row2 < Board::MAX_ROW; row2++)
                for (col2 = 0; col2 < Board::MAX_COL; col2++)
                {
                    if (!Board::IsValidSquare (row2, col2) ||
                            (board.getCell (row2, col2) != Board::PlayerToCell (player.getPlayer ())) ||
                            ((col1 == col2) && (row1 == row2)))
                        continue;

                    rowDiffer = row2 - row1;
                    colDiffer = col2 - col1;

                    row3 = row2 + colDiffer;
                    col3 = col2 - rowDiffer;

                    row4 = row1 + colDiffer;
                    col4 = col1 - rowDiffer;

                    if (!Board::IsValidSquare (row3, col3) ||
                            !Board::IsValidSquare (row4, col4))
                        continue;

                    if ((board.getCell (row3, col3) == Board::PlayerToCell (player.getPlayer ())) &&
                            (board.getCell (row4, col4) == EMPTY_CELL))
                    {
                        lastMove = TCoordCell (row4, col4);
                        return ATTACK_MOVE;
                    }
                    else if ((board.getCell (row3, col3) == EMPTY_CELL) &&
                            (board.getCell (row4, col4) == Board::PlayerToCell (player.getPlayer ())))
                    {
                        lastMove = TCoordCell (row3, col3);
                        return ATTACK_MOVE;
                    }
                }
        }
    return NONE_MOVE;
}

// Fills possible opponent's winning positions to opMoves[...]

void AIEasy::BuildOpponentMoves ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: BuildOpponentMoves" << std::endl;
#endif // NDEBUG

    int row1, col1, row2, col2, row3, col3, row4, col4, // corners of winner square
            rowDiffer, colDiffer; //col and row difference between 2 corners (1 and 2)
    TPlayer opponent = (player.getPlayer () == RED_PLAYER ? BLUE_PLAYER : RED_PLAYER);

    opponentMoves.clear ();

    for (row1 = 0; row1 < Board::MAX_ROW; row1++)
        for (col1 = 0; col1 < Board::MAX_COL; col1++)
        {
            if (!Board::IsValidSquare (row1, col1) ||
                    (board.getCell (row1, col1) != Board::PlayerToCell (opponent)))
                continue;

            for (row2 = 0; row2 < Board::MAX_ROW; row2++)
                for (col2 = 0; col2 < Board::MAX_COL; col2++)
                {
                    if (!Board::IsValidSquare (row2, col2) ||
                            (board.getCell (row2, col2) != Board::PlayerToCell (opponent)) ||
                            ((col1 == col2) && (row1 == row2)))
                        continue;

                    rowDiffer = row2 - row1;
                    colDiffer = col2 - col1;

                    row3 = row2 + colDiffer;
                    col3 = col2 - rowDiffer;

                    row4 = row1 + colDiffer;
                    col4 = col1 - rowDiffer;

                    if (!Board::IsValidSquare (row3, col3) ||
                            !Board::IsValidSquare (row4, col4))
                        continue;

                    if ((board.getCell (row3, col3) == Board::PlayerToCell (opponent)) &&
                            (board.getCell (row4, col4) == EMPTY_CELL))
                    {
                        opponentMoves.push_back (TCoordCell (row4, col4));
#ifndef NDEBUG
                        std::cout << "opponent = " << row4 << ", " << col4 << std::endl;
#endif // NDEBUG
                    }
                    else if ((board.getCell (row3, col3) == EMPTY_CELL) &&
                            (board.getCell (row4, col4) == Board::PlayerToCell (opponent)))
                    {
                        opponentMoves.push_back (TCoordCell (row3, col3));
#ifndef NDEBUG
                        std::cout << "opponent = " << row3 << ", " << col3 << std::endl;
#endif // NDEBUG
                    }
                }
        }
}

void AIEasy::BuildPlayerMoves ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: BuildPlayerMoves" << std::endl;
#endif // NDEBUG

    int row1, col1, row2, col2, row3, col3, row4, col4, // corners of winner square
            rowDiffer, colDiffer; //col and row difference between 2 corners (1 and 2)

    playerMoves.clear ();

    for (row1 = 0; row1 < Board::MAX_ROW; row1++)
        for (col1 = 0; col1 < Board::MAX_COL; col1++)
        {
            if (!Board::IsValidSquare (row1, col1) ||
                    (board.getCell (row1, col1) != Board::PlayerToCell (player.getPlayer ())))
                continue;

            for (row2 = 0; row2 < Board::MAX_ROW; row2++)
                for (col2 = 0; col2 < Board::MAX_COL; col2++)
                {
                    if (!Board::IsValidSquare (row2, col2) ||
                            (board.getCell (row2, col2) != Board::PlayerToCell (player.getPlayer ())) ||
                            ((col1 == col2) && (row1 == row2)))
                        continue;

                    rowDiffer = row2 - row1;
                    colDiffer = col2 - col1;

                    row3 = row2 + colDiffer;
                    col3 = col2 - rowDiffer;

                    row4 = row1 + colDiffer;
                    col4 = col1 - rowDiffer;

                    if (!Board::IsValidSquare (row3, col3) ||
                            !Board::IsValidSquare (row4, col4))
                        continue;

                    if ((board.getCell (row3, col3) == EMPTY_CELL) &&
                            (board.getCell (row4, col4) == EMPTY_CELL))
                    {
                        playerMoves.push_back (TCoordCell (row3, col3));
                        playerMoves.push_back (TCoordCell (row4, col4));
#ifndef NDEBUG
                        std::cout << "player = " << row3 << ", " << col3 << std::endl;
                        std::cout << "player = " << row4 << ", " << col4 << std::endl;
#endif // NDEBUG
                    }
                }
        }
}

/* 
 * Blocks opponent's possible move by attack quad. (Block Offensively)
 * It means if, there are only one opponent's possible winning cell, and
 * it is one of the "good" player's steps, it'll be attacked.
 */

TMove AIEasy::BlockAsAttackStep ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: BlockAsAttackStep" << std::endl;
#endif // NDEBUG

    if (opponentMoves.size () != 1 || playerMoves.empty ())
        return NONE_MOVE;

    for (TCoordCellVector::iterator i = playerMoves.begin (); i != playerMoves.end (); i++)
        if (*i == opponentMoves.front ())
        {
            lastMove = *i;
            return ATTACK_MOVE;
        }

    return NONE_MOVE;
}

// Block possible winning positions in random order

TMove AIEasy::BlockWithBlockerStep ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: BlockWithBlockerStep" << std::endl;
#endif // NDEBUG

    if (opponentMoves.empty () ||
            ((opponentMoves.size () == 1) && playerMoves.empty ()) ||
            player.getBlockLeft () == 0)
        return NONE_MOVE;

    if (player.getBlockLeft () > 0)
    {
        TCoordCellVector::size_type randomMove = rand () % opponentMoves.size ();
        lastMove = opponentMoves[randomMove];

        return BLOCK_MOVE;
    }
    return NONE_MOVE;
}

// Block possible winning positions in random order with Attack coin

TMove AIEasy::BlockWithAttackerStep ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: BlockWithAttackerStep" << std::endl;
#endif // NDEBUG

    if (opponentMoves.empty ())
        return NONE_MOVE;

    if (player.getAttackLeft () > 0)
    {
        TCoordCellVector::size_type randomMove = rand () % opponentMoves.size ();
        lastMove = opponentMoves[randomMove];

        return ATTACK_MOVE;
    }
    return NONE_MOVE;
}

// Attacks randomly one of the possible winning positions

TMove AIEasy::PlayerMoveStep ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: PlayerMoveStep" << std::endl;
#endif // NDEBUG

    if (playerMoves.empty () || (player.getAttackLeft () == 0))
        return NONE_MOVE;

    TCoordCellVector::size_type randomMove = rand () % playerMoves.size ();
    lastMove = playerMoves[randomMove];

    return ATTACK_MOVE;
}

// Player try to predict opponent's move. It looks his possible winning positions and remember them

TMove AIEasy::BlockOpponentPotential ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: BlockOpponentPotential" << std::endl;
#endif // NDEBUG

    int row1, col1, row2, col2, row3, col3, row4, col4, // corners of winner square
            rowDiffer, colDiffer; //col and row difference between 2 corners (1 and 2)
    TPlayer opponent = (player.getPlayer () == RED_PLAYER ? BLUE_PLAYER : RED_PLAYER);

    playerMoves.clear ();

    for (row1 = 0; row1 < Board::MAX_ROW; row1++)
        for (col1 = 0; col1 < Board::MAX_COL; col1++)
        {
            if (!Board::IsValidSquare (row1, col1) ||
                    (board.getCell (row1, col1) != Board::PlayerToCell (opponent)))
                continue;

            for (row2 = 0; row2 < Board::MAX_ROW; row2++)
                for (col2 = 0; col2 < Board::MAX_COL; col2++)
                {
                    if (!Board::IsValidSquare (row2, col2) ||
                            (board.getCell (row2, col2) != Board::PlayerToCell (opponent)) ||
                            ((col1 == col2) && (row1 == row2)))
                        continue;

                    rowDiffer = row2 - row1;
                    colDiffer = col2 - col1;

                    row3 = row2 + colDiffer;
                    col3 = col2 - rowDiffer;

                    row4 = row1 + colDiffer;
                    col4 = col1 - rowDiffer;

                    if (!Board::IsValidSquare (row3, col3) ||
                            !Board::IsValidSquare (row4, col4))
                        continue;

                    if ((board.getCell (row3, col3) == EMPTY_CELL) &&
                            (board.getCell (row4, col4) == EMPTY_CELL))
                    {
                        playerMoves.push_back (TCoordCell (row3, col3));
                        playerMoves.push_back (TCoordCell (row4, col4));
                    }
                }
        }

    return PlayerMoveStep ();
}

// Player builds possible square from one corner

TMove AIEasy::StartPlayerPotential ()
{
#ifndef NDEBUG
    std::cout << "AIEasy: StartPlayerPotential" << std::endl;
#endif // NDEBUG

    int row1, col1, row2, col2, row3, col3, row4, col4, // corners of winner square
            rowDiffer, colDiffer; //col and row difference between 2 corners (1 and 2)

    playerMoves.clear ();

    for (row1 = 0; row1 < Board::MAX_ROW; row1++)
        for (col1 = 0; col1 < Board::MAX_COL; col1++)
        {
            if (!Board::IsValidSquare (row1, col1) ||
                    (board.getCell (row1, col1) != Board::PlayerToCell (player.getPlayer ())))
                continue;

            for (row2 = 0; row2 < Board::MAX_ROW; row2++)
                for (col2 = 0; col2 < Board::MAX_COL; col2++)
                {
                    if (!Board::IsValidSquare (row2, col2) ||
                            (board.getCell (row2, col2) != EMPTY_CELL) ||
                            ((col1 == col2) && (row1 == row2)))
                        continue;

                    rowDiffer = row2 - row1;
                    colDiffer = col2 - col1;

                    row3 = row2 + colDiffer;
                    col3 = col2 - rowDiffer;

                    row4 = row1 + colDiffer;
                    col4 = col1 - rowDiffer;

                    if (!Board::IsValidSquare (row3, col3) ||
                            !Board::IsValidSquare (row4, col4))
                        continue;

                    if ((board.getCell (row3, col3) == EMPTY_CELL) &&
                            (board.getCell (row4, col4) == EMPTY_CELL))
                    {
                        playerMoves.push_back (TCoordCell (row2, col2));
                        playerMoves.push_back (TCoordCell (row3, col3));
                        playerMoves.push_back (TCoordCell (row4, col4));
                    }
                }
        }

    return PlayerMoveStep ();
}
