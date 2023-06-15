#pragma once

#include "chess.h"
#include "bitboard.h"
#include "move.h"

#include <array>
#include <string>

namespace chess
{

class game_board
{
public:
    using game_bitboards = std::array<bitboard, piece::piece_count>;
    using game_occupancy = std::array<bitboard, color::both + 1>;


    const game_bitboards& boards() const;
    bitboard boards(const piece p) const;
    bitboard occupancy(const color p) const;

    color player_move() const;
    square en_passant() const;
    flags castle_rights() const;

    piece piece_on(const square sq) const;

    void make(const move mv);
    void unmake(const move mv, const square ep, flags cr);

    bool check() const;
    bool check_mate() const;

    bool operator ==(const game_board& other) const;

private:
    game_bitboards m_bitboards{0ull, 0ull, 0ull, 0ull, 0ull, 0ull,
                                                         0ull, 0ull, 0ull, 0ull, 0ull, 0ull};

    game_occupancy m_occupancies{0ull, 0ull, 0ull};

    color m_player_move{color::white};
    square m_en_passant_square{square::not_a_square};
    flags m_castle_rights{castle_flags::all_castle};


    friend bool load_fen(game_board&, const std::string&);
};

}
