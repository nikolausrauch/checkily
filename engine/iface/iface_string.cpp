#include "iface_string.h"

#include <chess/move/movegen.h>
#include <chess/chess_string.h>

#include <cassert>
#include <regex>

namespace ckly::iface
{

std::vector<std::string> tokenize(const std::string &t_input)
{
    const std::regex pattern("\\S+");
    std::vector<std::string> tokens(std::sregex_token_iterator(t_input.begin(), t_input.end(), pattern),
                                    std::sregex_token_iterator());

    return tokens;
}

chess::square to_square(char file, char rank)
{
    auto file_ = file - 'a';
    auto rank_ = rank - '1';

    if(0 > file_ || file_ > 7 || 0 > rank_ || rank_ > 7) {  return chess::square::not_a_square; }

    return static_cast<chess::square>( (7-rank_)*8 + file_);
}


chess::move parse_move(const std::string &mv, const chess::game_board& board)
{
    using namespace chess;

    if(mv.size() < 4) { return illegal_move; }

    square from = to_square(mv[0], mv[1]);
    square to = to_square(mv[2], mv[3]);
    if(from == not_a_square || to == not_a_square) { return illegal_move; }

    auto mvs = generate_legal_moves(board);

    std::vector<move> matching_moves;
    std::copy_if(mvs.begin(), mvs.end(), std::back_insert_iterator(matching_moves),
                 [from, to](auto mv){ return from == mv.m_from && to == mv.m_to; });

    if(matching_moves.empty()) { return illegal_move; }
    if(matching_moves.size() == 1) { return matching_moves.front(); }

    assert(matching_moves.front().m_promote != chess::not_a_piece);
    if(mv.size() != 5) { return illegal_move; }

    auto promotion = not_a_piece;
    switch (mv[4])
    {
    case 'Q':
    case 'q': promotion = color_piece(board.player_move(), queen); break;
    case 'B':
    case 'b': promotion = color_piece(board.player_move(), bishop); break;
    case 'R':
    case 'r': promotion = color_piece(board.player_move(), rook); break;
    case 'K':
    case 'k': promotion = color_piece(board.player_move(), knight); break;
    default: return illegal_move;
    }

    auto it = std::find_if(matching_moves.begin(), matching_moves.end(),
                           [promotion](auto mv){ return mv.m_promote == promotion; });
    return *it;
}

std::string parse_move(const chess::move mv)
{
    const char prom_char = mv.m_promote != chess::not_a_piece ? chess::piece_names[mv.m_promote] : '\0';
    return std::string(chess::square_names[mv.m_from]) + chess::square_names[mv.m_to] + prom_char;
}

void sanitize_msg(std::string& msg)
{
    msg.erase( std::remove_if(msg.begin(), msg.end(), [](char ch)
    {
        return !(std::isprint(static_cast<unsigned char>(ch)) || ch == '\n' || ch == '\r');
    }), msg.end());
}

std::string strip_msg(std::string& msg)
{
    /* TODO: this is really wierd, but I have some issues with the output from pipes */
    sanitize_msg(msg);
    auto last_newline = msg.find_last_of('\n');

    if(last_newline != std::string::npos)
    {
        auto sub_str = msg.substr(0, last_newline);
        msg = msg.substr(last_newline + 1, msg.find_first_of('\0'));
        return sub_str;
    }

    return {};
}

}
