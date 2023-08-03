#include "game_module.h"

#include "chess_app.h"

#include <chess/chess_format.h>
#include <chess/move/movegen.h>

game_module::game_module(chess_app &app)
    : m_app(app),
      m_white{"Player #1", {}, {}, chess::white},
      m_black{"Player #2", {}, {}, chess::black},
      m_game_mode(mode::human_vs_human)
{
    m_moves.reserve(256);

    start_position();
    update_pieces();
}

void game_module::start_position()
{
    chess::load_fen(m_board, chess::fen_startposition);
    m_moves.clear();
    m_white = {m_white.m_name, {}, {}, chess::white};
    m_black = {m_black.m_name, {}, {}, chess::black};
}

void game_module::game_mode(mode play_mode)
{
    m_game_mode = play_mode;
}

void game_module::make_move(chess::move move)
{
    auto& player = (m_board.player_move() == chess::white) ? m_white : m_black;
    if(move.m_capture != chess::not_a_piece)
    {
        player.m_captured[move.m_capture]++;
    }

    m_board.make(move);
    m_moves.push_back(Record{move, m_board.check(), m_board.check_mate()});
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

const std::vector<game_module::Record>& game_module::moves() const
{
    return m_moves;
}

game_module::mode game_module::game_mode() const
{
    return m_game_mode;
}

const game_module::Player& game_module::player_white() const
{
    return m_white;
}

const game_module::Player& game_module::player_black() const
{
    return m_black;
}

const int game_module::material_imbalance(const Player& me)
{
    // TODO: this is so stupid
    const Player& other = &me == &m_white ? m_black : m_white;

    std::array<int, chess::piece_count> values =
    {
        1, 3, 0, 3, 5, 9,
        1, 3, 0, 3, 5, 9
    };

    auto captured_me  = 0;
    for(int i = 0; i < me.m_captured.size(); i++)
    {
        captured_me += values[i] * me.m_captured[i];
    }

    auto captured_other  = 0;
    for(int i = 0; i < other.m_captured.size(); i++)
    {
        captured_other += values[i] * other.m_captured[i];
    }

    return captured_me - captured_other;
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
