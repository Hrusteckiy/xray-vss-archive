////////////////////////////////////////////////////////////////////////////
//	Module 		: board_classic_othello_can_move.cpp
//	Created 	: 08.12.2004
//  Modified 	: 08.12.2004
//	Author		: Dmitriy Iassenev
//	Description : Calssic othello board can move implementation
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "board_classic_othello.h"

template <
	int increment, 
	CBoardClassicOthello::cell_type _color_to_move, 
	CBoardClassicOthello::cell_type opponent_color
>
IC	bool CBoardClassicOthello::can_move_in_direction(cell_type const *start_cell) const
{
    CBoardClassicOthello::cell_type const *current_cell = start_cell + increment;

	if (*current_cell != opponent_color)
		return			(false);

	current_cell		+= increment;
	if (*current_cell != opponent_color)
		return			(*current_cell == _color_to_move);
			
	current_cell		+= increment;
	if (*current_cell != opponent_color)
		return			(*current_cell == _color_to_move);
			
	current_cell		+= increment;
	if (*current_cell != opponent_color)
		return			(*current_cell == _color_to_move);
			
	current_cell		+= increment;
	if (*current_cell != opponent_color)
		return			(*current_cell == _color_to_move);
			
	current_cell		+= increment;
	if (*current_cell != opponent_color)
		return			(*current_cell == _color_to_move);
			
    current_cell		+= increment;
	return				(*current_cell == _color_to_move);
}
	
template <CBoardClassicOthello::cell_type _color_to_move>
IC	bool CBoardClassicOthello::can_move		(const cell_index &index) const
{
	const cell_type color_to_move	= _color_to_move;
	const cell_type	opponent_color	= (color_to_move == BLACK ? WHITE : BLACK);
	cell_type const *start_cell		= m_board + index;

	switch (flipping_directions[index]) {
		case 1 :
			return (
				can_move_in_direction<  1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< 10, color_to_move, opponent_color>(start_cell)
			);
		case 2 :
			return (
				can_move_in_direction< -1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< 10, color_to_move, opponent_color>(start_cell)
			);
		case 3 :
			return (
				can_move_in_direction< -1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  9, color_to_move, opponent_color>(start_cell)
			);
		case 4 :
			return (
				can_move_in_direction< -9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< 10, color_to_move, opponent_color>(start_cell)
			);
		case 5 :
			return (
				can_move_in_direction<-10, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< 10, color_to_move, opponent_color>(start_cell)
			);
		case 6 :
			return (
				can_move_in_direction<-10, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  9, color_to_move, opponent_color>(start_cell)
			);
		case 7 :
			return (
				can_move_in_direction< -9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  1, color_to_move, opponent_color>(start_cell)
			);
		case 8 :
			return (
				can_move_in_direction<-10, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -8, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -1, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction<  1, color_to_move, opponent_color>(start_cell)
			);
		case 9 :
			return (
				can_move_in_direction<-10, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -9, color_to_move, opponent_color>(start_cell) ||
				can_move_in_direction< -1, color_to_move, opponent_color>(start_cell)
			);
		default : NODEFAULT;
	}
#ifdef _DEBUG
	return		(false);
#endif
}

bool CBoardClassicOthello::can_move			(const cell_index &index) const
{
	if (color_to_move() == BLACK)
		return	(can_move<BLACK>(index));
	else
		return	(can_move<WHITE>(index));
}

template <CBoardClassicOthello::cell_type _color_to_move>
bool CBoardClassicOthello::can_move			() const
{
	for (cell_index i=0; i<8; ++i)
		for (cell_index j=0; j<8; ++j) {
			if (cell(i,j) != EMPTY)
				continue;

			if (can_move<_color_to_move>(index(i,j)))
				return	(true);
		}
	return				(false);
}

bool CBoardClassicOthello::can_move			() const
{
	if (color_to_move() == BLACK)
		return	(can_move<BLACK>());
	else
		return	(can_move<WHITE>());
}
