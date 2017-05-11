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

#include <stdlib.h>

#include "board.h"

Board::Board ()
{
    for (int row = 0; row < MAX_ROW; row++)
        for (int col = 0; col < MAX_COL; col++)
            cells[row][col] = EMPTY_CELL;
}

Board::~Board () { }

bool Board::IsValidSquare (const int row, const int col)
{
    return ((col >= 0) && (col < MAX_COL) && (row > 0) && (row < MAX_ROW - 1) ||
            (col > 0) && (col < MAX_COL - 1) && (row >= 0) && (row < MAX_ROW));
}

bool Board::IsValidSquare (const TCoordCell& cell)
{
    return Board::IsValidSquare (cell.row, cell.col);
}

TCell Board::PlayerToCell (const TPlayer player)
{
    return (player == BLUE_PLAYER ? BLUE_CELL : RED_CELL);
}

/* TODO: maybe I should place here an exeption, (for ex. EWrongCell) */
TCell Board::getCell (const int row, const int col) const
{
    return cells[row][col];
}

/* TODO: maybe I should place here an exeption, (for ex. EWrongCell) */
void Board::setCell (const int row, const int col, TCell cell)
{
    cells[row][col] = cell;
}

bool Board::FindSquare (TCoordCell *cell1,
                        TCoordCell *cell2,
                        TCoordCell *cell3,
                        TCoordCell *cell4,
                        TCell cell) const
{
    int _row1, _col1, _row2, _col2, _row3, _col3, _row4, _col4,
            rowDiffer, colDiffer;

    for (_row1 = 0; _row1 < MAX_ROW; _row1++)
        for (_col1 = 0; _col1 < MAX_COL; _col1++)
        {
            if (!IsValidSquare (_row1, _col1) || (cells[_row1][_col1] != cell))
                continue;

            for (_row2 = _row1; _row2 < MAX_ROW; _row2++)
                for (_col2 = _col1 + 1; _col2 < MAX_COL; _col2++)
                {
                    if (!IsValidSquare (_row2, _col2))
                        continue;
                    if (cells[_row2][_col2] != cell)
                        continue;

                    rowDiffer = _row2 - _row1;
                    colDiffer = _col2 - _col1;

                    _row3 = _row2 + colDiffer;
                    _col3 = _col2 - rowDiffer;

                    _row4 = _row1 + colDiffer;
                    _col4 = _col1 - rowDiffer;

                    if (!IsValidSquare (_row3, _col3) ||
                            (cells[_row3][_col3] != cell))
                        continue;
                    if (!IsValidSquare (_row4, _col4) ||
                            (cells[_row4][_col4] != cell))
                        continue;

                    cell1->row = _row1;
                    cell1->col = _col1;

                    cell2->row = _row2;
                    cell2->col = _col2;

                    cell3->row = _row3;
                    cell3->col = _col3;

                    cell4->row = _row4;
                    cell4->col = _col4;

                    return true;
                }
        }

    return false;
}
