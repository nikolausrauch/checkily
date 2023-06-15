#pragma once

#include "../board.h"
#include "../move.h"

namespace chess
{

bool is_attacked(const game_board& board, color defender, const square sq);

move_list generate_pseudo_legal_moves(const game_board& board, const piece man, const square sq);
move_list generate_pseudo_legal_moves(const game_board& board);

move_list generate_legal_moves(const game_board& board, const piece man, const square sq);
move_list generate_legal_moves(const game_board& board);

}
