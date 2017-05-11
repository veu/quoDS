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

#ifndef _AIBASE_H
#define	_AIBASE_H

#include "player.h"
#include "board.h"

class AIBase
{
public:
    AIBase (const Player& player, const Board& board);
    virtual ~AIBase ();

    virtual TMove Step () = 0;

    inline TCoordCell getLastMove () const
    {
        return lastMove;
    }

protected:
    const Player& player;
    const Board& board;

    TCoordCell lastMove;

    TMove RandomStep (int cellSpread = 5);
};

#endif	/* _AIBASE_H */

