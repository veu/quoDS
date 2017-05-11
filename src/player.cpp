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

#include "player.h"

Player::Player (TPlayer _color) : color (_color),
attackLeft (MAX_ATTACK),
blockLeft (MAX_BLOCK) { }

Player::~Player () { }

TPlayer Player::getPlayer () const
{
    return color;
}

int Player::getAttackLeft () const
{
    return attackLeft;
}

int Player::getBlockLeft () const
{
    return blockLeft;
}

int Player::DecreaseAttack ()
{
    if (attackLeft > 0)
        return --attackLeft;
    return 0;
}

// Should be use rarely

int Player::IncreaseAttack ()
{
    if (attackLeft < MAX_ATTACK)
        return ++attackLeft;
    return MAX_ATTACK;
}

int Player::DecreaseBlock ()
{
    if (blockLeft > 0)
        return --blockLeft;
    return 0;
}

// Should be use rarely

int Player::IncreaseBlock ()
{
    if (blockLeft < MAX_BLOCK)
        return ++blockLeft;
    return MAX_BLOCK;
}
