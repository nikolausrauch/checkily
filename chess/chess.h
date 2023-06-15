#pragma once

namespace chess
{

enum square : unsigned int
{
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    not_a_square,
    square_count = not_a_square
};


enum color : int
{
    white = 0, black = 1, both = 2, player_count = 2
};
inline constexpr color opponent(const color t_player) { return static_cast<color>(!t_player); }


enum piece : unsigned int
{
    pawn = 0, knight = 1, king = 2, bishop = 3, rook = 4, queen = 5,

    w_pawn = 0, w_knight = 1, w_king = 2, w_bishop = 3, w_rook = 4 , w_queen = 5,
    b_pawn = 6, b_knight = 7, b_king = 8, b_bishop = 9, b_rook = 10, b_queen = 11,
    not_a_piece = 12, piece_count = 12
};
inline constexpr piece color_piece(const color player, const piece man) { return static_cast<piece>(player*6 + man); }
inline constexpr color color_of(const piece man) { return static_cast<color>(man / 6); }

using flags = int;
enum castle_flags : flags
{
    no_castle   = 0,
    w_king_side = 1 << 0,
    w_queen_side= 1 << 1,
    b_king_side = 1 << 2,
    b_queen_side= 1 << 3,
    all_castle = w_king_side | w_queen_side | b_king_side | b_queen_side
};

};
