#pragma once

#include "bitboard.h"
#include "board.h"
#include "chess.h"
#include "move.h"

#include <array>

namespace chess
{

constexpr std::array<const char*, square::square_count + 1> square_names =
{
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "none"
};

constexpr std::array<char, piece::piece_count + 1> piece_names =
{
    'P', 'N', 'K', 'B', 'R', 'Q',
    'p', 'n', 'k', 'b', 'r', 'q', '.'
};

constexpr std::array<const char*, color::player_count + 1> player_names =
{
    "white", "black", "both"
};

constexpr chess::bitboard to_bitboard(const char* bits)
{
    chess::bitboard board_rep = 0ull;
    for(int i = 0; bits[i] != '\0'; i++)
    {
        board_rep |= bits[i] == '1' ? bit(i) : 0ull;
    }

    return board_rep;
}

std::string to_string(const bitboard bboard);
std::string to_string(const game_board& board);
std::string to_string(const move mv);
std::string to_string(const move_list& mv_list);
std::array<char, 64> to_char_array(const game_board& board);

}
