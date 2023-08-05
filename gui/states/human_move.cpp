#include "human_move.h"

#include "human_move.h"

#include "gui/chess_app.h"
#include "gui/render_module.h"
#include "states/human_turn.h"

human_move_handler::human_move_handler(chess_app& app)
    : m_app(app)
{

}

void human_move_handler::on_enter()
{

}

void human_move_handler::on_update(float dt)
{

}

void human_move_handler::on_exit()
{

}

void human_move_handler::on_event(const sf::Event& event)
{
    /* make move */
    if(event.type == sf::Event::EventType::MouseButtonReleased)
    {
        if(event.mouseButton.button != sf::Mouse::Left) { return; }

        auto& renderer = m_app.render();
        auto& statemachine = m_app.states();
        auto& game = m_app.game();

        auto sq = renderer.square_from_mouse( sf::Vector2f(event.mouseButton.x, event.mouseButton.y) );
        if(sq == chess::not_a_square)
        {
            statemachine.transition(state::human_turn);
            return;
        };

        /* check if move is valid */
        auto it = std::find_if(m_possible_moves.begin(), m_possible_moves.end(), [this, sq](auto move)
        {
            return move.m_from == m_from && move.m_to == sq;
        });

        if(it == m_possible_moves.end())
        {
            statemachine.transition(state::human_turn);
            return;
        }

        /* game over by on board result */
        if(game.make_move(*it) != game_module::result::unknown)
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

    /* cancel move */
    if(event.type == sf::Event::EventType::MouseButtonPressed)
    {
        if(event.mouseButton.button != sf::Mouse::Right) { return; }

        auto& statemachine = m_app.states();
        statemachine.transition(state::human_turn);
    }
}

void human_move_handler::on_render()
{
    auto& window = m_app.render_window();
    auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));

    auto& renderer = m_app.render();
    renderer.render_board();
    renderer.render_square_hover(mouse_pos);
    renderer.render_square_select(m_from);
    renderer.render_pieces(m_from);
    renderer.render_moves(m_possible_moves);
    renderer.render_piece_mouse(m_piece, mouse_pos);
}

void human_move_handler::on_gui()
{
    auto& gui = m_app.gui();
    gui.render_gameinfo();
}

void human_move_handler::set_params(chess::piece piece, chess::square square)
{
    m_piece = piece;
    m_from = square;

    auto& game = m_app.game();
    m_possible_moves = game.piece_moves(m_piece, m_from);
}
