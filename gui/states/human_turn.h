#pragma once

#include "state.h"

#include <chess/chess.h>

class human_turn_handler
{
public:
    human_turn_handler(chess_app& app);

    void on_enter();
    void on_update();
    void on_exit();
    void on_event(const sf::Event& t_event);
    void on_render();
    void set_params(chess::color t_player);

private:
    chess_app& m_app;
    chess::color m_player;
};
