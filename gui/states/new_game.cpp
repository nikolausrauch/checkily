#include "new_game.h"

#include "gui/chess_app.h"


new_game_handler::new_game_handler(chess_app& app)
    : m_app(app)
{

}

void new_game_handler::on_enter()
{

}

void new_game_handler::on_update(float dt)
{

}

void new_game_handler::on_exit()
{

}

void new_game_handler::on_event(const sf::Event& t_event)
{

}

void new_game_handler::on_render()
{
    auto& renderer = m_app.render();
    renderer.render_board();
    renderer.render_pieces();
}

void new_game_handler::on_gui()
{
    auto& gui = m_app.gui();
    gui.render_gameinfo();
    gui.render_newgame();
}
