#pragma once

#include <chess/chess.h>
#include <chess/board.h>
#include <chess/move.h>

class chess_app;

class game_module
{
public:
    enum class mode
    {
        human_vs_human,
        human_vs_ai,
        ai_vs_ai
    };

public:
    game_module(chess_app& app);

    void start_position();
    void game_mode(mode play_mode);

    void make_move(chess::move move);

    chess::game_board& board();
    const chess::game_board& board() const;
    const chess::move_list& moves() const;
    mode game_mode() const;

    const std::array<chess::piece, chess::square_count>& board_pieces() const;
    chess::move_list piece_moves(chess::piece piece, chess::square square) const;

private:
    void update_pieces();

private:
    chess_app& m_app;

    chess::game_board m_board;
    std::array<chess::piece, chess::square_count> m_board_pieces;
    chess::move_list m_moves;

    mode m_game_mode;
};
