#include "game_module.h"

#include "chess_app.h"

#include <chess/chess_format.h>
#include <chess/move/movegen.h>

game_module::game_module(chess_app &app)
    : m_app(app), m_game_mode(mode::human_vs_human)
{
    start_position();
    update_pieces();
}

void game_module::start_position()
{
    chess::load_fen(m_board, chess::fen_startposition);
    m_moves.clear();
}

void game_module::game_mode(mode play_mode)
{
    m_game_mode = play_mode;
}

void game_module::make_move(chess::move move)
{
    m_board.make(move);
    m_moves.push_back(move);
    update_pieces();

    auto& sound = m_app.sound();
    sound.play(move);
}

chess::game_board& game_module::board()
{
    return m_board;
}

const chess::game_board& game_module::board() const
{
    return m_board;
}

const chess::move_list& game_module::moves() const
{
    return m_moves;
}

game_module::mode game_module::game_mode() const
{
    return m_game_mode;
}

const std::array<chess::piece, chess::square_count>& game_module::board_pieces() const
{
    return m_board_pieces;
}

chess::move_list game_module::piece_moves(chess::piece piece, chess::square square) const
{
    return chess::generate_legal_moves(m_board, piece, square);
}

void game_module::update_pieces()
{
    std::fill(m_board_pieces.begin(), m_board_pieces.end(), chess::piece::not_a_piece);
    for(auto sq = 0u; sq < chess::square_count; sq++)
    {
        m_board_pieces[sq] = m_board.piece_on(static_cast<chess::square>(sq));
    }
}
