#include "game_over.h"

#include "gui/chess_app.h"


game_over_handler::game_over_handler(chess_app& app)
    : m_app(app)
{

}

void game_over_handler::on_enter()
{

}

void game_over_handler::on_update(float dt)
{

}

void game_over_handler::on_exit()
{

}

void game_over_handler::on_event(const sf::Event& t_event)
{

}

void game_over_handler::on_render()
{
    auto& renderer = m_app.render();
    renderer.render_board();
    renderer.render_pieces();
}

void game_over_handler::on_gui()
{
    auto& gui = m_app.gui();
    gui.render_gameinfo();
    gui.render_gameover();
}
