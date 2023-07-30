#include "sound_module.h"

#include "chess_app.h"

#include <iostream>

sound_module::sound_module(chess_app& app)
    : m_app(app)
{

    auto& configs = m_app.configs();

    /*********** setup sounds ***********/
    std::array<std::pair<sound_id, const char*>, sound_id::sound_count> sound_files =
    {{
        {sound_id::sound_move, "move.wav"},
        {sound_id::sound_capture, "capture.wav"},
        {sound_id::sound_check, "check.wav"},
        {sound_id::sound_castle, "castle.wav"},
        {sound_id::sound_check_mate, "check_mate.wav"}
    }};

    for(auto file : sound_files)
    {
        if(m_soundbuffers[file.first].loadFromFile(configs.sounds + file.second))
        {
            m_sounds[file.first].setBuffer(m_soundbuffers[file.first]);
            m_sounds[file.first].setLoop(false);
        }
        else
        {
            std::cerr << "Couldn't load sound file " << configs.sounds + file.second << std::endl;
        }
    }
}

sf::Sound& sound_module::sound(sound_id id)
{
    return m_sounds[id];
}

void sound_module::play(sound_id id)
{
    m_sounds[id].play();
}

void sound_module::play(chess::move move)
{
    auto& game = m_app.game();

    if(game.board().check())
    {
        if(game.board().check_mate())
        {
            play(sound_check_mate);
        }
        else
        {
            play(sound_check);
        }
    }
    else
    {
        if(move.m_capture != chess::not_a_piece)
        {
            play(sound_capture);
        }
        else
        {
            play(sound_move);
        }
    }
}
