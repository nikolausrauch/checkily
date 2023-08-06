#pragma once

#include "game_module.h"

#include <SFML/System/Vector2.hpp>

#include <array>


namespace sf { class Sprite; class Texture; class RenderWindow; class Event; }
struct ImGuiContext;
struct ImPlotContext;
struct ImFont;
class chess_app;

class gui_module
{
public:
    gui_module(chess_app& app);
    ~gui_module();

    void on_event(const sf::Event& event);
    void start(float dt);
    void end();

    void render_gameinfo();
    void render_playerinfo(const game_module::player& player, const sf::Vector2f& pos);

    void render_sprite(const sf::Sprite& sprite, const sf::Vector2f& size);
    bool render_sprite_button(const sf::Sprite& sprite, const sf::Vector2f& size, const char* id = "##legacy_imgui");
    void render_center_text(const std::string& text);

private:
    chess_app& m_app;

    ImGuiContext* m_im_context;
    ImPlotContext* im_plot_context;
    sf::Vector2u m_window_size;

    unsigned int m_font_texture;

    std::array<bool, 3> m_mouse_pressed;
    float m_mouse_scroll;

    bool m_capture_keyboard;
    bool m_capture_mouse;
    bool m_mouse_hovering;
};
