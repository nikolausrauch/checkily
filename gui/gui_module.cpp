#include "gui_module.h"

#include "chess_app.h"
#include "states/ai_turn.h"
#include "states/human_turn.h"

#include <chess/chess_string.h>

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <imgui/imgui.h>
#include <iostream>

namespace detail
{

void center_text(const std::string& text)
{
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth   = ImGui::CalcTextSize(text.c_str()).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", text.c_str());
}

}

gui_module::gui_module(chess_app& app)
    : m_app(app),
      m_im_context(nullptr), m_window_size(640, 480),
      m_mouse_pressed{false, false, false}, m_mouse_scroll(0.0f),
      m_capture_keyboard(false), m_capture_mouse(false), m_mouse_hovering(false)
{
    auto* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    ImGui::StyleColorsDark();

    m_im_context = context;

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "imgui_impl_sfml";
    io.DisplaySize = ImVec2(m_window_size.x, m_window_size.y);

    io.KeyMap[ImGuiKey_Tab] =           sf::Keyboard::Tab;
    io.KeyMap[ImGuiKey_LeftArrow] =     sf::Keyboard::Left;
    io.KeyMap[ImGuiKey_RightArrow] =    sf::Keyboard::Right;
    io.KeyMap[ImGuiKey_UpArrow] =       sf::Keyboard::Up;
    io.KeyMap[ImGuiKey_DownArrow] =     sf::Keyboard::Down;
    io.KeyMap[ImGuiKey_PageUp] =        sf::Keyboard::PageUp;
    io.KeyMap[ImGuiKey_PageDown] =      sf::Keyboard::PageDown;
    io.KeyMap[ImGuiKey_Home] =          sf::Keyboard::Home;
    io.KeyMap[ImGuiKey_End] =           sf::Keyboard::End;
    io.KeyMap[ImGuiKey_Delete] =        sf::Keyboard::Delete;
    io.KeyMap[ImGuiKey_Space] =         sf::Keyboard::Space;
    io.KeyMap[ImGuiKey_Backspace] =     sf::Keyboard::BackSpace;
    io.KeyMap[ImGuiKey_Enter] =         sf::Keyboard::Return;
    io.KeyMap[ImGuiKey_Escape] =        sf::Keyboard::Escape;
    io.KeyMap[ImGuiKey_A] =             sf::Keyboard::A;
    io.KeyMap[ImGuiKey_C] =             sf::Keyboard::C;
    io.KeyMap[ImGuiKey_V] =             sf::Keyboard::V;
    io.KeyMap[ImGuiKey_X] =             sf::Keyboard::X;
    io.KeyMap[ImGuiKey_Y] =             sf::Keyboard::Y;
    io.KeyMap[ImGuiKey_Z] =             sf::Keyboard::Z;


    /*----------- load fonts -----------*/
    if(true)
    {
        ImFontConfig config;
        config.SizePixels = 26.0f;
        config.OversampleH = config.OversampleV = 2;
        config.PixelSnapH = true;
        io.Fonts->AddFontDefault(&config);
    }
    /*----------- load fonts -----------*/

    io.BackendRendererName = "imgui_impl_opengl";

    /*------------- Font Texture  ------------*/
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    /* Upload texture to graphics system */
    glGenTextures(1, &m_font_texture);
    glBindTexture(GL_TEXTURE_2D, m_font_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    io.Fonts->TexID = (ImTextureID) (intptr_t) m_font_texture;
    /*------------- Font Texture  ------------*/
}

gui_module::~gui_module()
{
    if(m_font_texture)
    {
        ImGuiIO& io = ImGui::GetIO();
        glDeleteTextures(1, &m_font_texture);
        io.Fonts->TexID = nullptr;
        m_font_texture = 0;
    }

    ImGui::DestroyContext(m_im_context);
}

void gui_module::on_event(const sf::Event& event)
{
    ImGuiIO& io = ImGui::GetIO();

    if(event.type == sf::Event::KeyPressed)
    {
        m_capture_keyboard = io.WantCaptureKeyboard;

        io.KeysDown[event.key.code] = true;
        io.KeyCtrl =    io.KeysDown[sf::Keyboard::LControl] || io.KeysDown[sf::Keyboard::RControl];
        io.KeyShift =   io.KeysDown[sf::Keyboard::LShift] || io.KeysDown[sf::Keyboard::RShift];
        io.KeyAlt =     io.KeysDown[sf::Keyboard::LAlt] || io.KeysDown[sf::Keyboard::RAlt];
        io.KeySuper =   false;
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        m_capture_keyboard = io.WantCaptureKeyboard;

        io.KeysDown[event.key.code] = false;
        io.KeyCtrl =    io.KeysDown[sf::Keyboard::LControl] || io.KeysDown[sf::Keyboard::RControl];
        io.KeyShift =   io.KeysDown[sf::Keyboard::LShift] || io.KeysDown[sf::Keyboard::RShift];
        io.KeyAlt =     io.KeysDown[sf::Keyboard::LAlt] || io.KeysDown[sf::Keyboard::RAlt];
        io.KeySuper =   false;
    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        m_capture_mouse = io.WantCaptureMouse;
        if(event.mouseButton.button >= 0 && event.mouseButton.button < 3)
        {
            m_mouse_pressed[event.mouseButton.button] = true;
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        m_capture_mouse = io.WantCaptureMouse;
        if(event.mouseButton.button >= 0 && event.mouseButton.button < 3)
        {
            m_mouse_pressed[event.mouseButton.button] = false;
        }
    }
    else if(event.type == sf::Event::MouseWheelScrolled)
    {
        m_capture_mouse = io.WantCaptureMouse;
        m_mouse_scroll += event.mouseWheelScroll.delta;
    }
    else if(event.type == sf::Event::TextEntered)
    {
        m_capture_mouse = io.WantCaptureKeyboard;
        if (event.text.unicode > 0 && event.text.unicode < 0x10000)
        {
            io.AddInputCharacter(static_cast<unsigned short>(event.text.unicode));
        }
    }
}

void gui_module::start(float dt)
{
    auto& window = m_app.render_window();
    ImGuiIO& io = ImGui::GetIO();
    assert(io.Fonts->IsBuilt());

    io.DisplaySize = ImVec2(window.getSize().x, window.getSize().y);
    io.DisplayFramebufferScale = ImVec2(1, 1); // TODO: careful check with sfml (https://github.com/SFML/SFML/issues/1299)
    io.DeltaTime = dt;

    if(window.hasFocus())
    {
        const auto& pos = sf::Mouse::getPosition(window);
        io.MousePos = ImVec2(pos.x, pos.y);
    }
    else
    {
        io.MousePos = ImVec2(-1, -1);
    }

    auto& mousePressed = m_mouse_pressed;
    io.MouseDown[0] = mousePressed[0] || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    io.MouseDown[1] = mousePressed[1] || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    io.MouseDown[2] = mousePressed[2] || sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);

    mousePressed[0] = false;
    mousePressed[1] = false;
    mousePressed[2] = false;

    io.MouseWheel = m_mouse_scroll;
    m_mouse_scroll = 0.0f;

    m_mouse_hovering = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
    m_capture_keyboard = ImGui::IsAnyItemActive();

    ImGui::NewFrame();
}

void gui_module::end()
{
    ImGui::Render();

    auto& window = m_app.render_window();
    ImDrawData* draw_data = ImGui::GetDrawData();

    if(!draw_data)
        return;

    if (draw_data->CmdListsCount == 0)
        return;


    ImGuiIO& io = ImGui::GetIO();
    int fb_width = static_cast<int>(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = static_cast<int>(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
    {
        return;
    }

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    /* set OpenGL states */
    window.pushGLStates();

    /* Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers, polygon fill. */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /* Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps. */
    glViewport(0, 0, static_cast<GLsizei>(fb_width), static_cast<GLsizei>(fb_height));
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(draw_data->DisplayPos.x, draw_data->DisplayPos.x + draw_data->DisplaySize.x, draw_data->DisplayPos.y + draw_data->DisplaySize.y, draw_data->DisplayPos.y, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    /* Will project scissor/clipping rectangles into framebuffer space */
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    /* Render command lists */
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            /* Project scissor/clipping rectangles into framebuffer space */
            ImVec4 clip_rect;
            clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
            clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
            clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
            clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

            if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
            {
                /* Apply scissor/clipping rectangle */
                glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

                /* Bind texture, Draw */
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    window.popGLStates();
}

void gui_module::render_gameinfo()
{
    auto& config = m_app.configs();
    auto& window = m_app.render_window();
    auto& game = m_app.game();

    render_playerinfo(game.player_info(chess::black), sf::Vector2f{static_cast<float>(window.getSize().x - config.gui_size), 0.0f});

    ImGui::SetNextWindowPos(ImVec2(window.getSize().x - config.gui_size, 128.0f));
    ImGui::SetNextWindowSize(ImVec2(config.gui_size, window.getSize().y - 2 * 128.0f));
    ImGui::Begin("##game_moves", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0, 0, 0, 0});
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.76f, 0.76f, 0.75f, 1.0f});

        ImGui::BeginTable("Moves", 3, ImGuiTableFlags_RowBg);
        ImGui::TableSetupColumn("##turn", ImGuiTableColumnFlags_WidthFixed, 48, 0);
        ImGui::TableSetupColumn("##white", ImGuiTableColumnFlags_WidthFixed, 84, 1);
        ImGui::TableSetupColumn("##black", ImGuiTableColumnFlags_WidthFixed, 84, 2);
        ImGui::TableNextRow(ImGuiTableRowFlags_None, 32);

        auto& moves = game.moves();
        for(unsigned int i = 0; i < moves.size();)
        {
            if(i % 2 == 0)
            {
                ImGui::TableSetColumnIndex(0);
                ImGui::TextColored({0.57f, 0.57f, 0.56f, 1.0f}, "%2d.", i / 2 + 1);
            }

            {
                auto& mv = moves[i++];
                ImGui::TableSetColumnIndex(1);
                ImGui::Button(chess::to_notation_string(mv.m_move, mv.m_check, mv.m_checkmate).c_str(), {84, 32});
            }

            if(i < moves.size())
            {
                auto& mv = moves[i++];
                ImGui::TableSetColumnIndex(2);
                ImGui::Button(chess::to_notation_string(mv.m_move, mv.m_check, mv.m_checkmate).c_str(), {84, 32});
                if(i != moves.size()) { ImGui::TableNextRow(ImGuiTableRowFlags_None, 32); }
            }
        }

        if (ImGui::GetScrollY() >= (ImGui::GetScrollMaxY() - 16))
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndTable();

        ImGui::PopStyleColor(2);
    }
    ImGui::End();

    render_playerinfo(game.player_info(chess::white), sf::Vector2f{static_cast<float>(window.getSize().x - config.gui_size), window.getSize().y - 128.0f});
}

void gui_module::render_playerinfo(const game_module::player& player, const sf::Vector2f& pos)
{
    auto& config = m_app.configs();
    auto& render = m_app.render();
    auto& game = m_app.game();

    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
    ImGui::SetNextWindowSize(ImVec2(config.gui_size, 128.0f));
    std::string window_id = player.name() + chess::player_names[player.color()];
    ImGui::Begin(window_id.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {-36.0f, 0.0f});

        ImGui::Text("%s", player.name().c_str());
        const auto& captured = player.captured();
        for(unsigned int i = 0; i < captured.size(); i++)
        {
            auto piece = static_cast<chess::piece>(i);
            const auto& sprite = render.sprite_piece(piece);
            for(int c = 0; c < captured[i]; c++)
            {
                render_sprite(sprite, {48, 48}); ImGui::SameLine(0.0f, (c < captured[i]-1) ? -1.0f : 0.0f);
            }
        }

        auto material = game.material_imbalance(player.color());
        if(material > 0) { ImGui::TextColored({0.76f, 0.76f, 0.75f, 1.0f}, "+%d", material); }

        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void gui_module::render_newgame()
{
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
        detail::center_text("New Game");
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

void gui_module::render_gameover()
{
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
        detail::center_text(result_strings[static_cast<int>(game.game_result())]);
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

void gui_module::render_sprite(const sf::Sprite& sprite, const sf::Vector2f& size)
{
    ImVec2 tex_size = ImVec2(sprite.getTexture()->getSize().x, sprite.getTexture()->getSize().y);

    auto tex_rect = sprite.getTextureRect();
    ImVec2 uv_start = ImVec2(tex_rect.left / tex_size.x, tex_rect.top / tex_size.y);
    ImVec2 uv_end = ImVec2( (tex_rect.left + tex_rect.width) / tex_size.x, (tex_rect.top + tex_rect.height) / tex_size.y);

    ImGui::Image(reinterpret_cast<void*>(sprite.getTexture()->getNativeHandle()), ImVec2{size.x, size.y}, uv_start, uv_end);
}
