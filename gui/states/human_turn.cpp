#include "human_turn.h"

#include "human_move.h"

#include "gui/chess_app.h"
#include "gui/render_module.h"

human_turn_handler::human_turn_handler(chess_app &app)
    : m_app(app), m_player(chess::white)
{

}

void human_turn_handler::on_enter()
{

}

void human_turn_handler::on_update(float dt)
{

}

void human_turn_handler::on_exit()
{

}

void human_turn_handler::on_event(const sf::Event &t_event)
{
    if(t_event.type == sf::Event::EventType::MouseButtonPressed)
    {
        if(t_event.mouseButton.button != sf::Mouse::Left) { return; };

        auto& renderer = m_app.render();
        auto& statemachine = m_app.states();
        auto& game = m_app.game();

        auto sq = renderer.square_from_mouse( sf::Vector2f(t_event.mouseButton.x, t_event.mouseButton.y) );
        if(sq == chess::not_a_square) { return; };

        const auto& board = game.board_pieces();
        if(board[sq] == chess::not_a_piece) { return; }
        if(chess::color_of(board[sq]) != m_player) { return; }

        auto& handler = statemachine.handler(state::human_move);
        handler.set_params<human_move_handler>(board[sq], sq);
        statemachine.transition(state::human_move);
    }
}

void human_turn_handler::on_render()
{
    auto& renderer = m_app.render();
    renderer.render_board();
    renderer.render_pieces();
}

void human_turn_handler::on_gui()
{
    auto& gui = m_app.gui();
    gui.render_gameinfo();
}

void human_turn_handler::set_params(chess::color t_player)
{
    m_player = t_player;
}
