#pragma once

#include "chess.h"

#include <vector>

namespace chess
{

struct __attribute__((packed)) move
{
    square m_from : 7;
    square m_to : 7;

    piece m_piece : 4;
    piece m_capture : 4;
    piece m_promote : 4;

    flags m_double : 1;
    flags m_en_passant : 1;
    flags m_castling : 1;

    bool operator ==(move other)
    {
        return  m_from == other.m_from &&
                m_to == other.m_to &&
                m_piece == other.m_piece &&
                m_capture == other.m_capture &&
                m_promote == other.m_promote &&
                m_double == other.m_double &&
                m_en_passant == other.m_en_passant &&
                m_castling == other.m_castling;
    };
};
static_assert(sizeof(move) == sizeof(unsigned int), "Move does not have size of an integer!");

enum move_flags
{
    no_move_flag = 0,
    double_push = 1 << 0,
    en_passant = 1 << 1,
    castling = 1 << 2
};

constexpr move encode_move(const int from, const int to, const int man, const int capture, const int promote, flags mv_flags)
{
    return move
    {
        .m_from = static_cast<square>(from),
        .m_to = static_cast<square>(to),
        .m_piece = static_cast<piece>(man),
        .m_capture = static_cast<piece>(capture),
        .m_promote = static_cast<piece>(promote),
        .m_double = (mv_flags & move_flags::double_push) ? 1 : 0,
        .m_en_passant = (mv_flags & move_flags::en_passant) ? 1 : 0,
        .m_castling = (mv_flags & move_flags::castling) ? 1 : 0
    };
}

struct record
{
    chess::move m_move;
    bool m_check;
    bool m_checkmate;
};

using game_record = std::vector<record>;
using move_list = std::vector<move>;
inline static constexpr chess::move illegal_move = chess::encode_move(0, 0, 0, 0, 0, 0);

}
