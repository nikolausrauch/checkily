#include "ai_turn.h"

#include "gui/chess_app.h"
#include "gui/game_module.h"
#include "states/ai_move.h"

ai_turn_handler::ai_turn_handler(chess_app& app)
    : m_app(app)
{

}

void ai_turn_handler::on_enter()
{
    auto& game = m_app.game();
    auto& engine = game.engine(game.board().player_move());
    engine.position(game.board(), game.moves());
    engine.go();
}

void ai_turn_handler::on_update(float dt)
{
    auto& game = m_app.game();
    chess::move mv = chess::illegal_move;
    auto& engine =  m_app.game().engine(game.board().player_move());

    if(engine.try_bestmove(mv))
    {
        auto& statemachine = m_app.states();

        statemachine.handler(state::ai_move).set_params<ai_move_handler>(mv);
        statemachine.transition(state::ai_move);
    }
}

void ai_turn_handler::on_exit()
{

}

void ai_turn_handler::on_event(const sf::Event& t_event)
{

}

void ai_turn_handler::on_render()
{
    auto& renderer = m_app.render();
    renderer.render_board();
    renderer.render_pieces();
}

void ai_turn_handler::on_gui()
{
    auto& gui = m_app.gui();
    gui.render_gameinfo();
}
