#pragma once

#include "state.h"

class game_over_handler
{
public:
    game_over_handler(chess_app& app);

    void on_enter();
    void on_update(float dt);
    void on_exit();
    void on_event(const sf::Event& t_event);
    void on_render();
    void on_gui();

private:
    chess_app& m_app;
};
