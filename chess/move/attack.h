#pragma once

#include "../chess.h"
#include "../bitboard.h"

namespace chess
{

/************ table based attacks ************/
bitboard pawn_attack(const color player, const square sq);
bitboard knight_attack(const square sq);
bitboard king_attack(const square sq);
bitboard bishop_attack(const square sq, bitboard block_mask);
bitboard rook_attack(const square sq, bitboard block_mask);
bitboard queen_attack(const square sq, bitboard block_mask);


/************ on the fly attacks ************/
namespace otf
{

constexpr bitboard pawn_attack(const color player, const square sq)
{
    bitboard board = bit(sq);

    bitboard attacks = 0x0;
    if(player == color::white)
    {
        attacks |= (board >> 7) & not_file_a;
        attacks |= (board >> 9) & not_file_h;
    }
    else
    {
        attacks |= (board << 7) & not_file_h;
        attacks |= (board << 9) & not_file_a;
    }

    return attacks;
}

constexpr bitboard knight_attack(const square sq)
{
    bitboard board = bit(sq);
    bitboard attacks = 0x0;

    attacks |= (board >> 17) & not_file_h;
    attacks |= (board >> 15) & not_file_a;
    attacks |= (board >> 10) & not_file_gh;
    attacks |= (board >> 6)  & not_file_ab;

    attacks |= (board << 17) & not_file_a;
    attacks |= (board << 15) & not_file_h;
    attacks |= (board << 10) & not_file_ab;
    attacks |= (board << 6)  & not_file_gh;

    return attacks;
}

constexpr bitboard king_attack(const square sq)
{
    bitboard board = bit(sq);
    bitboard attacks = 0x0;

    attacks |= (board >> 7) & not_file_a;
    attacks |= (board >> 8);
    attacks |= (board >> 9) & not_file_h;

    attacks |= (board >> 1) & not_file_h;
    attacks |= (board << 1) & not_file_a;

    attacks |= (board << 7) & not_file_h;
    attacks |= (board << 8);
    attacks |= (board << 9) & not_file_a;

    return attacks;
}

constexpr bitboard bishop_attack(const square sq, const bitboard block_mask)
{
    bitboard attack = 0x0;

    int b_r = sq / 8;
    int b_f = sq % 8;

    int r = 0, f = 0;
    for(r = b_r + 1, f = b_f + 1; r <= 7 && f <= 7; r++, f++) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }
    for(r = b_r - 1, f = b_f + 1; r >= 0 && f <= 7; r--, f++) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }
    for(r = b_r + 1, f = b_f - 1; r <= 7 && f >= 0; r++, f--) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }
    for(r = b_r - 1, f = b_f - 1; r >= 0 && f >= 0; r--, f--) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }

    return attack;
}

constexpr bitboard rook_attack(const square sq, const bitboard block_mask)
{
    bitboard attack = 0x0;

    int b_r = sq / 8;
    int b_f = sq % 8;

    int r = 0, f = 0;
    for(r = b_r + 1, f = b_f; r <= 7; r++) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }
    for(r = b_r - 1, f = b_f; r >= 0; r--) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }
    for(r = b_r, f = b_f + 1; f <= 7; f++) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }
    for(r = b_r, f = b_f - 1; f >= 0; f--) { bitboard _tmp = (1ull << (r*8 + f)); attack |= _tmp; if(_tmp & block_mask) break; }

    return attack;
}

constexpr bitboard queen_attack(const square sq, const bitboard block_mask)
{
    return otf::bishop_attack(sq, block_mask) | otf::rook_attack(sq, block_mask);
}

}

};
