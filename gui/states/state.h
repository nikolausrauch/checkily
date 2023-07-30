#pragma once

enum class state
{
    human_turn,
    human_move,

    ai_turn,
    ai_move,
};

class chess_app;
namespace sf { class Event; }
