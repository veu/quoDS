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

#ifndef _BOARD_H
#define	_BOARD_H

#include "player.h"

#ifdef	__cplusplus
extern "C"
{
#endif

// Cell type on board

enum TCell
{
    EMPTY_CELL = 0,
    RED_CELL = 1,
    BLUE_CELL = 2,
    BLOCK_CELL = 3
};

// Structure, defined cell's position on the board

struct TCoordCell
{
    int row;
    int col;

    TCoordCell (const int r = 0, const int c = 0) : row (r), col (c) { }

    TCoordCell & operator = (const TCoordCell & cell)
    {
        row = cell.row;
        col = cell.col;
        return *this;
    }
};

inline bool operator == (const TCoordCell& c1, const TCoordCell& c2)
{
    return ((c1.row == c2.row) && (c1.col == c2.col));
}

inline bool operator< (const TCoordCell& c1, const TCoordCell& c2)
{
    if (c1.row < c2.row)
        return true;
    else if (c1.row == c2.row)
        return (c1.col < c2.col);
    else
        return false;
}

// this type is needed for std::map(TCoordCell, int, TCoordCellCompare)

struct TCoordCellCompare
{

    bool operator()(const TCoordCell& c1, const TCoordCell & c2) const
    {
        return (c1 < c2);
    }
};

class Board
{
public:
    Board ();
    ~Board ();

    TCell getCell (const int row, const int col) const;
    void setCell (const int row, const int col, TCell cell);

    bool FindSquare (TCoordCell *cell1,
                     TCoordCell *cell2,
                     TCoordCell *cell3,
                     TCoordCell *cell4,
                     TCell cell) const;

    static const int MAX_ROW = 11;
    static const int MAX_COL = 11;

    static bool IsValidSquare (const int row, const int col);
    static bool IsValidSquare (const TCoordCell& cell);
    static TCell PlayerToCell (const TPlayer player);

private:
    TCell cells[MAX_ROW][MAX_COL];
};

#ifdef	__cplusplus
}
#endif

#endif	/* _BOARD_H */

