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

#ifndef _AIEASY_H
#define	_AIEASY_H

#include <vector>

#include "aibase.h"

typedef std::vector<TCoordCell> TCoordCellVector;

class AIEasy : public AIBase
{
public:
    AIEasy (const Player& player, const Board& board);
    virtual ~AIEasy ();

    virtual TMove Step ();

private:
    bool firstStep;

    TCoordCellVector opponentMoves;
    TCoordCellVector playerMoves;

    TMove WinStep ();
    TMove BlockAsAttackStep ();
    TMove BlockWithBlockerStep ();
    TMove BlockWithAttackerStep ();
    TMove PlayerMoveStep ();
    TMove BlockOpponentPotential ();
    TMove StartPlayerPotential ();

    void BuildOpponentMoves ();
    void BuildPlayerMoves ();
};

#endif	/* _AIEASY_H */
