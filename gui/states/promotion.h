#pragma once

#include "state.h"

#include <chess/move.h>

class promotion_handler
{
public:
    promotion_handler(chess_app& app);

    void on_enter();
    void on_update(float dt);
    void on_exit();
    void on_event(const sf::Event& t_event);
    void on_render();
    void on_gui();
    void set_params(chess::move mv);

private:
    chess_app& m_app;
    chess::move m_move;
};
