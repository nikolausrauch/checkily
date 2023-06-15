#include "attack.h"

#include "magic.h"

#include <cstring>
#include <iostream>

namespace chess
{

/************ attacks from constexpr tables ************/
namespace attack_tables
{

constexpr std::array< std::array<bitboard, square::square_count>, color::player_count> pawn = []()
{
    std::array< std::array<bitboard, square::square_count>, color::player_count> table = {};
    for(auto sq = 0u; sq < square::square_count; sq++)
    {
        auto sq_ = static_cast<square>(sq);
        table[white][sq] = otf::pawn_attack(white, sq_);
        table[black][sq] = otf::pawn_attack(black, sq_);
    }

    return table;
}();

constexpr std::array<bitboard, square::square_count> knight = []()
{
    std::array<bitboard, square::square_count> table = {};
    for(auto sq = 0u; sq < square::square_count; sq++)
    {
        table[sq] = otf::knight_attack(static_cast<square>(sq));
    }

    return table;
}();

constexpr std::array<bitboard, square::square_count> king = []()
{
    std::array<bitboard, square::square_count> table = {};
    for(auto sq = 0u; sq < square::square_count; sq++)
    {
        table[sq] = otf::king_attack(static_cast<square>(sq));
    }

    return table;
}();

template<std::size_t size>
struct slider
{
    magic::entry m_param;
    std::array< bitboard, size > m_attacks;
};

constexpr std::array<slider<512>, square::square_count> bishop = []()
{
    std::array<slider<512>, square::square_count> table = {};

    for(auto sq = 0u; sq < square::square_count; sq++)
    {
        auto sq_ = static_cast<square>(sq);
        auto& entry = table[sq];
        entry.m_param.m_magic = magic::bishop_magic(sq_);
        entry.m_param.m_mask = magic::bishop_mask(sq_);
        entry.m_param.m_bits = magic::bishop_bits(sq_);

        int size = 1 << entry.m_param.m_bits;
        for(int index = 0; index < size; index++)
        {
            bitboard occup = magic::occupancy(index, entry.m_param.m_mask);
            auto magic_index = (occup * entry.m_param.m_magic) >> (64 - entry.m_param.m_bits);
            entry.m_attacks[magic_index] = otf::bishop_attack(sq_, occup);
        }
    }

    return table;
}();

constexpr std::array<slider<4096>, square::square_count> rook = []()
{
    std::array<slider<4096>, square::square_count> table = {};

    for(auto sq = 0u; sq < square::square_count; sq++)
    {
        auto sq_ = static_cast<square>(sq);
        auto& entry = table[sq];

        entry.m_param.m_magic = magic::rook_magic(sq_);
        entry.m_param.m_mask = magic::rook_mask(sq_);
        entry.m_param.m_bits = magic::rook_bits(sq_);

        int size = 1 << entry.m_param.m_bits;
        for(int index = 0; index < size; index++)
        {
            bitboard occup = magic::occupancy(index, entry.m_param.m_mask);
            auto magic_index = (occup * entry.m_param.m_magic) >> (64 - entry.m_param.m_bits);
            entry.m_attacks[magic_index] = otf::rook_attack(sq_, occup);
        }
    }

    return table;
}();

}


/************ attack interface ************/
bitboard pawn_attack(const color player, const square sq)
{
    return attack_tables::pawn[player][sq];
}

bitboard knight_attack(const square sq)
{
    return attack_tables::knight[sq];
}

bitboard king_attack(const square sq)
{
    return attack_tables::king[sq];
}

bitboard bishop_attack(const square sq, bitboard block_mask)
{
    const auto& entry = attack_tables::bishop[sq];

    block_mask &= entry.m_param.m_mask;
    block_mask *= entry.m_param.m_magic;
    block_mask >>= 64 - entry.m_param.m_bits;

    return entry.m_attacks[block_mask];
}

bitboard rook_attack(const square sq, bitboard block_mask)
{
    const auto& entry = attack_tables::rook[sq];

    block_mask &= entry.m_param.m_mask;
    block_mask *= entry.m_param.m_magic;
    block_mask >>= 64 - entry.m_param.m_bits;

    return entry.m_attacks[block_mask];
}

bitboard queen_attack(const square sq, bitboard block_mask)
{
    return bishop_attack(sq, block_mask) | rook_attack(sq, block_mask);
}

}
