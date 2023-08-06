#include "game_over.h"

#include "gui/chess_app.h"

#include <imgui/imgui.h>

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

    auto& game = m_app.game();

    static constexpr std::array<const char*, static_cast<int>(game_module::result::result_count)> result_strings =
    {
            "Black wins by checkmate",
            "White wins by checkmate",
            "Black wins - White resigns",
            "White wins - Black resigns",
            "Black wins - Timeout White",
            "White wins - Timeout Black",
            "Draw - Stalemate",
            "Draw",
            "ERROR! No decisive result!"
    };

    ImGui::SetNextWindowPos(ImVec2(128, 256));
    ImGui::SetNextWindowSize(ImVec2(480, 128));
    ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
    {
        gui.render_center_text(result_strings[static_cast<int>(game.game_result())]);
        ImGui::Separator();

        ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 64);
        ImGui::Separator();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x/2 - (2*148 + 24) / 2);
        if(ImGui::Button("new game", {148, 48}))
        {
            m_app.states().start(state::new_game);
        }
        ImGui::SameLine();
        if(ImGui::Button("exit", {148, 48}))
        {
            m_app.render_window().close();
        }
    }
    ImGui::End();
}
