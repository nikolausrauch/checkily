#pragma once

#include <chess/board.h>
#include <chess/chess.h>
#include <chess/move.h>

#include <string>
#include <vector>

namespace ckly::iface
{

std::vector<std::string> tokenize(const std::string &t_input);

chess::square to_square(char file, char rank);

std::string parse_move(const chess::move mv);
chess::move parse_move(const std::string &mv, const chess::game_board& board);

void sanitize_msg(std::string& msg);
std::string strip_msg(std::string& msg);

}
