#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace chess
{

/*********************************** BITBOARD / BITS ***********************************/
using bitboard = std::uint64_t;

#define bit(bit_index) (1ull << (bit_index))
#define bit_set(board, bit_index) ( (board) |= (1ull << (bit_index) ))
#define bit_get(board, bit_index) ( (board) & (1ull << (bit_index) ))
#define bit_pop(board, bit_index) ( (board) &= ~(1ull << (bit_index) ))
#define bit_count(bitboard) __builtin_popcountll( (bitboard) )
#define bit_ffs(bitboard) (__builtin_ffsll( (bitboard) ) - 1)

#define board_clamp(board) ((board) < 0 ? 0 : (board))

/* rank masks */
constexpr bitboard rank_8 = bit(0) | bit(1) | bit(2) | bit(3) | bit(4) | bit(5) | bit(6) | bit(7);
constexpr bitboard rank_7 = rank_8 << 8;
constexpr bitboard rank_2 = rank_8 << 48;
constexpr bitboard rank_1 = rank_8 << 56;

/* file masks */
constexpr bitboard file_a = bit(0) | bit(8) | bit(16) | bit(24) | bit(32) | bit(40) | bit(48) | bit(56);
constexpr bitboard file_b = file_a << 1;
constexpr bitboard file_c = file_a << 2;
constexpr bitboard file_g = file_a << 6;
constexpr bitboard file_h = file_a << 7;

constexpr bitboard not_file_a = ~file_a;
constexpr bitboard not_file_h = ~file_h;
constexpr bitboard not_file_ab = ~(file_a | file_b);
constexpr bitboard not_file_gh = ~(file_g | file_h);


}
