#pragma once

#include <chess/chess.h>
#include <chess/board.h>
#include <chess/move.h>

#include <SFML/System/Time.hpp>

class chess_app;

class game_module
{
public:
    enum class mode
    {
        human_vs_human,
        human_vs_ai,
        ai_vs_ai
    };

    struct Player
    {
        std::string m_name{"Player"};
        std::array<unsigned short, chess::piece_count> m_captured;
        sf::Time m_time;
        chess::color m_color;
    };

    struct Record
    {
        chess::move m_move;
        bool m_check;
        bool m_checkmate;
    };

public:
    game_module(chess_app& app);

    void start_position();
    void game_mode(mode play_mode);

    void make_move(chess::move move);

    chess::game_board& board();
    const chess::game_board& board() const;
    const std::vector<Record>& moves() const;
    mode game_mode() const;

    const Player& player_white() const;
    const Player& player_black() const;

    const int material_imbalance(const Player& me);

    const std::array<chess::piece, chess::square_count>& board_pieces() const;
    chess::move_list piece_moves(chess::piece piece, chess::square square) const;

private:
    void update_pieces();

private:
    chess_app& m_app;

    chess::game_board m_board;
    std::array<chess::piece, chess::square_count> m_board_pieces;
    std::vector<Record> m_moves;

    Player m_white;
    Player m_black;

    mode m_game_mode;
};
