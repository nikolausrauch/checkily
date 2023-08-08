#pragma once

#include "state.h"

class new_game_handler
{
public:
    new_game_handler(chess_app& app);

    void on_enter();
    void on_update(float dt);
    void on_exit();
    void on_event(const sf::Event& t_event);
    void on_render();
    void on_gui();

private:
    void render_newgame();
    void render_engine_settings();

private:
    chess_app& m_app;
};
