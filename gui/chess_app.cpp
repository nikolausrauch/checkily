#include "chess_app.h"

#include "states/ai_turn.h"
#include "states/ai_move.h"
#include "states/human_turn.h"
#include "states/human_move.h"
#include "states/game_over.h"
#include "states/new_game.h"

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>

#include <cassert>
#include <iostream>

chess_app::chess_app(const config& configs)
    : m_configs(configs),
      m_window(sf::VideoMode{
               (2*configs.border_offset.x + configs.square_size.x*8) * configs.scale_board + configs.gui_size,
               (2*configs.border_offset.y + configs.square_size.y*8) * configs.scale_board}, "checkily")
{
    m_window.setVerticalSyncEnabled(true);
    m_window.setKeyRepeatEnabled(false);
    m_window.clear(sf::Color(39, 37, 34));
    m_window.display();

    /* initialize extension wrangler */
    if(!gladLoadGL())
    {
        std::cerr << "Failed to initialize OpenGL Context (GLAD)!" << std::endl;
    }

    /******************** modules ********************/
    m_game_module = std::make_unique<game_module>(*this);
    m_render_module = std::make_unique<render_module>(*this);
    m_sound_module = std::make_unique<sound_module>(*this);
    m_gui_module = std::make_unique<gui_module>(*this);

    /******************** setup states ********************/
    m_state_machine.register_handler<human_turn_handler>(state::human_turn, *this);
    m_state_machine.register_handler<human_move_handler>(state::human_move, *this);
    m_state_machine.register_handler<ai_turn_handler>(state::ai_turn, *this);
    m_state_machine.register_handler<ai_move_handler>(state::ai_move, *this);
    m_state_machine.register_handler<new_game_handler>(state::new_game, *this);
    m_state_machine.register_handler<game_over_handler>(state::game_over, *this);

    m_state_machine.register_transition(state::human_turn, state::human_move);
    m_state_machine.register_transition(state::human_move, state::human_turn);
    m_state_machine.register_transition(state::human_move, state::ai_turn);

    m_state_machine.register_transition(state::ai_turn, state::ai_move);
    m_state_machine.register_transition(state::ai_move, state::human_turn);
    m_state_machine.register_transition(state::ai_move, state::ai_turn);

    m_state_machine.register_transition(state::new_game, state::human_turn);
    m_state_machine.register_transition(state::new_game, state::ai_turn);
    m_state_machine.register_transition(state::ai_move, state::game_over);
    m_state_machine.register_transition(state::human_move, state::game_over);

    m_state_machine.start(state::new_game);
}

const chess_app::config& chess_app::configs()
{
    return m_configs;
}

sf::RenderWindow& chess_app::render_window()
{
    return m_window;
}

render_module& chess_app::render()
{
    assert(m_render_module);
    return *m_render_module;
}

game_module& chess_app::game()
{
    assert(m_game_module);
    return *m_game_module;
}

sound_module& chess_app::sound()
{
    assert(m_sound_module);
    return *m_sound_module;
}

gui_module& chess_app::gui()
{
    assert(m_gui_module);
    return *m_gui_module;
}

state_machine<state>& chess_app::states()
{
    return m_state_machine;
}

void chess_app::run()
{
    sf::Clock timer;

    while(m_window.isOpen())
    {
        sf::Event event;
        while( m_window.pollEvent(event) )
        {
            if(event.type == sf::Event::Closed)
            {
                m_window.close();
            }

            if(event.type == event.KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    m_window.close();
                }
            }

            m_state_machine.current_handler().on_event(event);
            m_gui_module->on_event(event);
        }

        float dt = timer.restart().asSeconds();
        m_state_machine.current_handler().on_update(dt);

        m_window.clear(sf::Color(39, 37, 34));
        {
            m_state_machine.current_handler().on_render();

            m_gui_module->start(dt);
            {
                m_state_machine.current_handler().on_gui();
            }
            m_gui_module->end();
        }
        m_window.display();
    }
}
