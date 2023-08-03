#pragma once

#include "render_module.h"
#include "game_module.h"
#include "sound_module.h"
#include "gui_module.h"
#include "state_machine.h"
#include "states/state.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include <string>

class chess_app
{
public:
    struct config
    {
        /***** rendering *****/
        unsigned int scale_board{6u};
        sf::Vector2i border_offset{4, 4};
        sf::Vector2i square_size{15, 15};
        std::string textures{"assets/pixel/"};

        /****** sound ******/
        bool sound_mute{true};
        float sound_volume{100.0f};
        std::string sounds{"assets/sound/"};

        /****** gui ******/
        unsigned int gui_size{512};
    };

public:
    chess_app(const config& conf);

    const config& configs();
    sf::RenderWindow& render_window();

    render_module& render();
    game_module& game();
    sound_module& sound();
    gui_module& gui();
    state_machine<state>& states();

    void run();

private:
    config m_configs;
    sf::RenderWindow m_window;

    std::unique_ptr<render_module> m_render_module;
    std::unique_ptr<game_module> m_game_module;
    std::unique_ptr<sound_module> m_sound_module;
    std::unique_ptr<gui_module> m_gui_module;

    state_machine<state> m_state_machine;
};
