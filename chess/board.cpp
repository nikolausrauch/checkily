#include "board.h"

#include "move/movegen.h"

namespace chess
{

namespace detail
{

constexpr std::array<bitboard, square::square_count> castling_mask =
{
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14,
};

}

const game_board::game_bitboards& game_board::boards() const
{
    return m_bitboards;
}

bitboard game_board::boards(const piece p) const
{
    return m_bitboards[p];
}

bitboard game_board::occupancy(const color p) const
{
    return m_occupancies[p];
}

color game_board::player_move() const
{
    return m_player_move;
}

square game_board::en_passant() const
{
    return m_en_passant_square;
}

flags game_board::castle_rights() const
{
    return m_castle_rights;
}

piece game_board::piece_on(const square sq) const
{
    auto sq_mask = bit(sq);

    for(auto i = 0u; i < piece::piece_count; i++)
    {
        if(m_bitboards[i] & sq_mask) return static_cast<piece>(i);
    }

    return not_a_piece;
}

void game_board::make(const move mv)
{
    bit_pop(m_bitboards[mv.m_piece], mv.m_from);
    bit_pop(m_occupancies[color_of(mv.m_piece)], mv.m_from);
    bit_set(m_occupancies[color_of(mv.m_piece)], mv.m_to);
    m_castle_rights &= detail::castling_mask[mv.m_from];
    m_castle_rights &= detail::castling_mask[mv.m_to];

    /* capture */
    if(mv.m_capture != piece::not_a_piece)
    {
        int offset = mv.m_en_passant ? (color_of(mv.m_piece) == white ? 8 : -8) : 0;
        bit_pop(m_bitboards[mv.m_capture], mv.m_to + offset);
        bit_pop(m_occupancies[color_of(mv.m_capture)], mv.m_to + offset);
    }

    /* promotion */
    if(mv.m_promote != piece::not_a_piece)
    {
        bit_set(m_bitboards[mv.m_promote], mv.m_to);
    }
    else
    {
        bit_set(m_bitboards[mv.m_piece], mv.m_to);
    }

    /* double push */
    m_en_passant_square = not_a_square;
    if(mv.m_double)
    {
        int offset = color_of(mv.m_piece) == white ? 8 : -8;
        m_en_passant_square = static_cast<square>(mv.m_to + offset);
    }

    /* castling */
    if(mv.m_castling)
    {
        auto rook = color_piece(m_player_move, piece::rook);
        auto sq_from = (bit(mv.m_to) & file_g) ? mv.m_to + 1 : mv.m_to - 2;
        auto sq_to = (bit(mv.m_to) & file_g) ? mv.m_to - 1 : mv.m_to + 1;

        bit_pop(m_bitboards[rook], sq_from);
        bit_set(m_bitboards[rook], sq_to);
        bit_pop(m_occupancies[m_player_move], sq_from);
        bit_set(m_occupancies[m_player_move], sq_to);
    }

    m_occupancies[color::both] = m_occupancies[color::white] | m_occupancies[color::black];
    m_player_move = opponent(m_player_move);
}

void game_board::unmake(const move mv, const square ep, flags cr)
{
    bit_set(m_bitboards[mv.m_piece], mv.m_from);
    bit_set(m_occupancies[color_of(mv.m_piece)], mv.m_from);
    bit_pop(m_occupancies[color_of(mv.m_piece)], mv.m_to);

    m_castle_rights = cr;
    m_en_passant_square = ep;

    /* capture */
    if(mv.m_capture != piece::not_a_piece)
    {
        int offset = mv.m_en_passant ? (color_of(mv.m_piece) == white ? 8 : -8) : 0;
        bit_set(m_bitboards[mv.m_capture], mv.m_to + offset);
        bit_set(m_occupancies[color_of(mv.m_capture)], mv.m_to + offset);
    }

    /* promotion */
    if(mv.m_promote != piece::not_a_piece)
    {
        bit_pop(m_bitboards[mv.m_promote], mv.m_to);
    }
    else
    {
        bit_pop(m_bitboards[mv.m_piece], mv.m_to);
    }

    /* castling */
    if(mv.m_castling)
    {
        auto rook = color_piece(opponent(m_player_move), piece::rook);
        auto sq_from = (bit(mv.m_to) & file_g) ? mv.m_to + 1 : mv.m_to - 2;
        auto sq_to = (bit(mv.m_to) & file_g) ? mv.m_to - 1 : mv.m_to + 1;

        bit_set(m_bitboards[rook], sq_from);
        bit_pop(m_bitboards[rook], sq_to);
        bit_set(m_occupancies[opponent(m_player_move)], sq_from);
        bit_pop(m_occupancies[opponent(m_player_move)], sq_to);
    }

    m_occupancies[color::both] = m_occupancies[color::white] | m_occupancies[color::black];
    m_player_move = opponent(m_player_move);
}

bool game_board::check() const
{
    auto player_defend = player_move();
    auto king_sq = static_cast<square>( bit_ffs(boards(color_piece(player_defend, piece::king))) );
    return is_attacked(*this, player_defend, king_sq);
}

bool game_board::check_mate() const
{
    auto move_list = generate_legal_moves(*this);
    return move_list.empty();
}

bool game_board::operator ==(const game_board& other) const
{
    bool same = m_player_move == other.m_player_move;
    same = same && m_en_passant_square == other.m_en_passant_square;
    same = same && m_castle_rights == other.m_castle_rights;

    for(unsigned int i = 0; i < m_bitboards.size(); i++)
    {
        same = same && m_bitboards[i] == other.m_bitboards[i];
    }

    for(unsigned int i = 0; i < m_occupancies.size(); i++)
    {
        same = same && m_occupancies[i] == other.m_occupancies[i];
    }

    return same;
}

}
