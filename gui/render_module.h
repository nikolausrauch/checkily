#pragma once

#include <chess/chess.h>
#include <chess/move.h>

#include <SFML/Graphics.hpp>

#include <array>


class chess_app;

class render_module
{
    enum tex_id
    {
        tex_pieces = 0,
        tex_board,
        tex_ui,
        tex_count
    };

    enum sprite_id
    {
        sprite_board = 0,
        sprite_move,
        sprite_attack,
        sprite_w_tile_high,
        sprite_b_tile_high,
        sprite_select,
        sprite_count
    };

public:
    render_module(chess_app& app);

    sf::Sprite& sprite_piece(const chess::piece t_piece);
    sf::Sprite& sprite(const sprite_id t_id);

    void render_scale(float scale);

    void render_board();
    void render_pieces(chess::square moving_piece_sq = chess::square::not_a_square);
    void render_piece(chess::piece t_piece, const sf::Vector2f& pos);
    void render_moves(const chess::move_list& t_moves);
    void render_square_hover(const sf::Vector2f& pos);
    void render_square_select(chess::square t_sq);

    sf::Vector2i tile_from_square(const chess::square t_square) const;
    chess::square square_from_tile(const sf::Vector2i& pos) const;
    bool is_board_tile(const sf::Vector2i& pos) const;

    sf::Vector2f world_from_square(const chess::square t_square) const;
    sf::Vector2f world_from_tile(const sf::Vector2i& tile) const;

    sf::Vector2i tile_from_mouse(const sf::Vector2f& mouse_pos) const;
    chess::square square_from_mouse(const sf::Vector2f& mouse_pos) const;

    bool is_white_square(const sf::Vector2i& tile) const;
    bool is_black_square(const sf::Vector2i& tile) const;
    bool is_white_square(const chess::square t_square) const;
    bool is_black_square(const chess::square t_square) const;

private:
    chess_app& m_app;

    std::array<sf::Texture, tex_id::tex_count> m_textures;
    std::array<sf::Sprite, chess::piece_count> m_sprite_pieces;
    std::array<sf::Sprite, sprite_id::sprite_count> m_sprites;
};
