#include "chess_app.h"

#include "states/human_turn.h"
#include "states/human_move.h"

#include <SFML/Window/Event.hpp>

#include <cassert>

chess_app::chess_app(const config& configs)
    : m_configs(configs),
      m_window(sf::VideoMode{
               (2*configs.border_offset.x + configs.square_size.x*8) * configs.scale_board,
               (2*configs.border_offset.y + configs.square_size.y*8) * configs.scale_board}, "checkily")
{
    m_window.setKeyRepeatEnabled(false);

    /******************** modules ********************/
    m_game_module = std::make_unique<game_module>(*this);
    m_render_module = std::make_unique<render_module>(*this);
    m_sound_module = std::make_unique<sound_module>(*this);

    /******************** setup states ********************/
    m_state_machine.register_handler<human_turn_handler>(state::human_turn, *this);
    m_state_machine.register_handler<human_move_handler>(state::human_move, *this);

    m_state_machine.register_transition(state::human_turn, state::human_move);
    m_state_machine.register_transition(state::human_move, state::human_turn);

    m_state_machine.handler(state::human_turn).set_params<human_turn_handler>(m_game_module->board().player_move());
    m_state_machine.start(state::human_turn);
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

state_machine<state>& chess_app::states()
{
    return m_state_machine;
}

void chess_app::run()
{
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
        }

        m_state_machine.current_handler().on_update();

        m_window.clear();
        {
            m_state_machine.current_handler().on_render();
        }
        m_window.display();
    }
}
