#pragma once

#include "board.h"

#include <string>
#include <vector>

namespace chess
{

/*************** FEN ***************/
constexpr const char* fen_startposition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

bool is_valid_fen(const std::string& str);
bool is_valid_fen(const std::vector<std::string>& str_tokens);

bool load_fen(game_board& board, const std::string &str);
std::string to_fen_string(const game_board& board);

}
