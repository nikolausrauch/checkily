#pragma once

#include "uci/process.h"

#include <chess/chess.h>
#include <chess/board.h>
#include <chess/move.h>

#include <SFML/System/Time.hpp>

class chess_app;

class game_module
{
public:
    struct player
    {
        player(const std::string& name, chess::color color);

        const std::string& name() const;
        const std::array<unsigned short, chess::piece_count>& captured() const;
        const sf::Time& time() const;
        chess::color color() const;
        bool is_ai() const;

    private:
        bool new_game(uci_process& engine);

    private:
        std::string m_name{"Player"};
        std::array<unsigned short, chess::piece_count> m_captured;
        sf::Time m_time;
        chess::color m_color;

        bool m_ai;
        std::string m_filepath;

        friend class game_module;
        friend class new_game_handler;
    };

    enum class result : int
    {
        checkmate_white = 0,
        checkmate_black,
        resign_white,
        resign_black,
        timeout_white,
        timeout_black,
        stalemate,
        draw,
        unknown,
        result_count = unknown + 1
    };

public:
    game_module(chess_app& app);

    bool new_game();

    result make_move(chess::move move);

    chess::game_board& board();
    const chess::game_board& board() const;
    const chess::game_record& moves() const;
    result game_result() const;

    player& player_info(chess::color color);
    const player& player_info(chess::color color) const;

    uci_process& engine(chess::color color);
    uci_process& eval_engine();

    const int material_imbalance(chess::color color);

    const std::array<chess::piece, chess::square_count>& board_pieces() const;
    chess::move_list piece_moves(chess::piece piece, chess::square square) const;

private:
    void update_pieces();
    result check_onboard_result();

private:
    chess_app& m_app;

    result m_game_result;
    chess::game_board m_board;
    chess::game_record m_moves;
    std::array<chess::piece, chess::square_count> m_board_pieces;

    std::array<uci_process, 3> m_engines;
    std::array<player, chess::player_count> m_players;
};
