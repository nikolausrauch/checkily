#include "promotion.h"

#include "chess_app.h"
#include "states/human_turn.h"

#include <imgui/imgui.h>

promotion_handler::promotion_handler(chess_app& app)
    : m_app(app)
{

}

void promotion_handler::on_enter()
{

}

void promotion_handler::on_update(float dt)
{

}

void promotion_handler::on_exit()
{

}

void promotion_handler::on_event(const sf::Event& t_event)
{

}

void promotion_handler::on_render()
{
    auto& game = m_app.game();
    auto& render = m_app.render();
    render.render_board();
    render.render_pieces(m_move.m_from);
    render.render_piece(chess::color_piece(game.board().player_move(), chess::pawn), m_move.m_to);
}

void promotion_handler::on_gui()
{
    auto& game = m_app.game();
    auto& gui = m_app.gui();
    auto& render = m_app.render();
    auto& window = m_app.render_window();

    gui.render_gameinfo();

    bool selected = false;

    auto render_pos = render.world_from_square(m_move.m_to);
    render_pos.y = std::min<float>(window.getSize().y - 96*4 - 18*4, render_pos.y);
    ImGui::SetNextWindowPos(ImVec2(render_pos.x, render_pos.y));
    ImGui::Begin("##promotion", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
    {
        {
            auto piece = chess::color_piece(game.board().player_move(), chess::queen);
            const auto& sprite = render.sprite_piece(piece);
            if(gui.render_sprite_button(sprite, {96, 96}, "##queen_promote"))
            {
                selected = true;
                m_move.m_promote = piece;
            }
        }

        {
            auto piece = chess::color_piece(game.board().player_move(), chess::knight);
            const auto& sprite = render.sprite_piece(piece);
            if(gui.render_sprite_button(sprite, {96, 96}, "##knight_promote"))
            {
                selected = true;
                m_move.m_promote = piece;
            }
        }

        {
            auto piece = chess::color_piece(game.board().player_move(), chess::rook);
            const auto& sprite = render.sprite_piece(piece);
            if(gui.render_sprite_button(sprite, {96, 96}, "##rook_promote"))
            {
                selected = true;
                m_move.m_promote = piece;
            }
        }

        {
            auto piece = chess::color_piece(game.board().player_move(), chess::bishop);
            const auto& sprite = render.sprite_piece(piece);
            if(gui.render_sprite_button(sprite, {96, 96}, "##bishop_promote"))
            {
                selected = true;
                m_move.m_promote = piece;
            }
        }
    }
    ImGui::End();

    if(selected)
    {
        auto& statemachine = m_app.states();

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

void promotion_handler::set_params(chess::move mv)
{
    m_move = mv;
}
