#pragma once

#include "state.h"

#include <chess/chess.h>
#include <chess/move.h>

class human_move_handler
{
public:
    human_move_handler(chess_app& t_app);

    void on_enter();
    void on_update();
    void on_exit();
    void on_event(const sf::Event& t_event);
    void on_render();
    void on_gui();
    void set_params(chess::piece t_piece, chess::square t_square);

private:
    chess_app& m_app;

    chess::piece m_piece;
    chess::square m_from;
    chess::move_list m_possible_moves;
};
