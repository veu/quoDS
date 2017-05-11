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

#ifndef _AIHARD_H
#define	_AIHARD_H

#include <vector>
#include <map>

#include "aibase.h"

typedef std::map<int, int> TIntMap;

class AIHard : public AIBase
{
public:
    AIHard (const Player& player, const Board& board);
    virtual ~AIHard ();

    virtual TMove Step ();
private:
    static const int STANDARD_PRIORITY = 1;
    static const int SPOON_PRIORITY = 3;
    static const int WIN_PRIORITY = 1000;

    TIntMap playerImportant;
    TIntMap opponentImportant;

    void CheckImportant (TIntMap& important, TPlayer playerColor);
    void Pair (TIntMap& important, TPlayer playerColor,
               int row3, int col3, int row4, int col4);
    int InsertCell (TIntMap& important, int row, int col, int priority);
    TIntMap::iterator HasCritical ();
    TMove CheckOpponentAttack ();
    TMove CheckOpponentImportant ();
};

#endif	/* _AIHARD_H */
