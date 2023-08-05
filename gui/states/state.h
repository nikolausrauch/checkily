#pragma once

enum class state
{
    human_turn,
    human_move,

    ai_turn,
    ai_move,

    new_game,
    game_over
};

class chess_app;
namespace sf { class Event; }
