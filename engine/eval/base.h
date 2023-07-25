#pragma once

#include <chess/board.h>
#include <chess/chess.h>

#include <array>

namespace ckly
{

// debug
// https://www.chessprogramming.org/Simplified_Evaluation_Function
// https://www.chessprogramming.org/Evaluation

using score = int;

using piece_square_table = std::array<score, chess::square::square_count>;
using postion_score = std::array<piece_square_table, chess::piece_count>;
using material_score = std::array<score, chess::piece::piece_count>;

enum class table_names
{
    codemonkeyking,
    michniewski
};
template <table_names Name>
struct eval_table;


using evaluation_func = score (const chess::game_board &board);

}
