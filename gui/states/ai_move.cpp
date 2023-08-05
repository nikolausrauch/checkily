#include "ai_move.h"

#include "chess_app.h"

#include "states/human_turn.h"

#include <cmath>

ai_move_handler::ai_move_handler(chess_app& app)
    : m_app(app)
{

}

void ai_move_handler::on_enter()
{

}

void ai_move_handler::on_update(float dt)
{
    auto& statemachine = m_app.states();
    auto& game = m_app.game();
    auto& render = m_app.render();
    m_current_pos = render.world_interpolate_move(m_current_pos, m_final_pos, dt, m_speed);

    if(m_current_pos == m_final_pos)
    {
        /* game over by on board result */
        if(game.make_move(m_move) != game_module::result::unknown)
        {
            statemachine.transition(state::game_over);
            return;
        }

        /* transition to opponent turn */
        auto& other_player = game.player_info(game.board().player_move());
        if(other_player.is_ai())
        {
            statemachine.transition(state::ai_turn);
        }
        else
        {
            statemachine.handler(state::human_turn).set_params<human_turn_handler>(game.board().player_move());
            statemachine.transition(state::human_turn);
        }
    }
}

void ai_move_handler::on_exit()
{

}

void ai_move_handler::on_event(const sf::Event& t_event)
{

}

void ai_move_handler::on_render()
{
    auto& renderer = m_app.render();
    renderer.render_board();
    renderer.render_pieces(m_move.m_from);
    renderer.render_piece(m_move.m_piece, m_current_pos);
}

void ai_move_handler::on_gui()
{
    auto& gui = m_app.gui();
    gui.render_gameinfo();
}

void ai_move_handler::set_params(chess::move mv)
{
    m_move = mv;

    auto& render = m_app.render();
    m_current_pos = render.world_from_square(mv.m_from);
    m_final_pos = render.world_from_square(mv.m_to);

    auto diff = m_final_pos - m_current_pos;
    float length = std::sqrt(diff.x*diff.x + diff.y*diff.y);

    m_speed = 7.5f*length;
}
