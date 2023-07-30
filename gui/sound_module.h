#pragma once

#include <chess/move.h>

#include <SFML/Audio.hpp>

#include <array>

class chess_app;

class sound_module
{
public:
    enum sound_id
    {
        sound_move = 0,
        sound_capture,
        sound_check,
        sound_castle,
        sound_check_mate,
        sound_count
    };

    sound_module(chess_app& app);
    sf::Sound& sound(sound_id id);

    void play(sound_id id);
    void play(chess::move move);

private:
    chess_app& m_app;

    std::array<sf::SoundBuffer, sound_id::sound_count> m_soundbuffers;
    std::array<sf::Sound, sound_id::sound_count> m_sounds;
};
