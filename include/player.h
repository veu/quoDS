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

#ifndef _PLAYER_H
#define	_PLAYER_H

// Movement type

enum TMove
{
    NONE_MOVE,
    BLOCK_MOVE,
    ATTACK_MOVE
};

// Player's color

enum TPlayer
{
    BLUE_PLAYER,
    RED_PLAYER
};

class Player
{
public:
    Player (TPlayer color);
    ~Player ();

    TPlayer getPlayer () const;
    int getAttackLeft () const;
    int getBlockLeft () const;

    int DecreaseAttack ();
    int IncreaseAttack ();

    int DecreaseBlock ();
    int IncreaseBlock ();

    static const int MAX_ATTACK = 20; // Quads
    static const int MAX_BLOCK = 6; // Quasars

private:
    TPlayer color;
    int attackLeft;
    int blockLeft;
};

#endif	/* _PLAYER_H */
