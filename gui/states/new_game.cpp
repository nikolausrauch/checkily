#include "new_game.h"

#include "gui/chess_app.h"
#include "states/human_turn.h"

#include <imgui/imgui.h>

#include <iostream>

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

    auto& game = m_app.game();

    auto render_ai_select = [&](const std::string& name, game_module::player& player)
    {
        ImGui::PushID(name.c_str());

        ImGui::TextColored({0.76f, 0.76f, 0.75f, 1.0f}, "%s", name.c_str());
        ImGui::Checkbox("chess engine:", &player.m_ai);
        if(player.is_ai())
        {
            ImGui::SameLine();
            if(game.engine(player.color()).uciok())
            { ImGui::TextColored({0.0f, 0.8f, 0.0f, 1.0f}, " uciok"); }
            else
            { ImGui::TextColored({0.8f, 0.0f, 0.0f, 1.0f}, " not ready"); }

            ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - 96);
            ImGui::InputText("", player.m_filepath.data(), player.m_filepath.size());
            ImGui::SameLine();
            if(ImGui::Button("run"))
            {
                auto& engine_ = game.engine(player.color());
                engine_.print_stdout(true);
                if(!engine_.execute(player.m_filepath))
                {
                    std::cerr << "Couldn't start engine with path " << player.m_filepath << std::endl;
                }
            }
        }

        ImGui::PopID();
    };

    ImGui::SetNextWindowPos(ImVec2(64, 128));
    ImGui::SetNextWindowSize(ImVec2(640, 480));
    ImGui::Begin("New Game", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
    {
        gui.render_center_text("New Game");
        ImGui::Separator();
        render_ai_select("Player White:", game.player_info(chess::white));

        ImGui::Separator();
        render_ai_select("Player Black:", game.player_info(chess::black));


        ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 64);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x/2 - (2*148 + 24) / 2);

        bool is_ready_white = !game.player_info(chess::white).is_ai() || (game.player_info(chess::white).is_ai() && game.engine(chess::white).uciok());
        auto is_ready_black = !game.player_info(chess::black).is_ai() || (game.player_info(chess::black).is_ai() && game.engine(chess::black).uciok());

        if(!is_ready_black || !is_ready_white) { ImGui::BeginDisabled(); }
        if(ImGui::Button("start", {148, 48}))
        {
            auto& statemachine = m_app.states();

            auto game_ok = game.new_game();
            if(game_ok)
            {
                if(game.player_info(chess::white).is_ai())
                {
                    statemachine.start(state::ai_turn);
                }
                else
                {
                    statemachine.handler(state::human_turn).set_params<human_turn_handler>(game.board().player_move());
                    statemachine.transition(state::human_turn);
                }
            }
            else
            {
                std::cerr << "Couldn't start new game. Possible that engine didn't respond to isready command." << std::endl;
            }
        }
        if(!is_ready_black || !is_ready_white) { ImGui::EndDisabled(); }
        ImGui::SameLine();
        if(ImGui::Button("exit", {148, 48}))
        {
            m_app.render_window().close();
        }
    }
    ImGui::End();
}
