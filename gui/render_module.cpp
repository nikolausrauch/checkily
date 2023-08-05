#include "render_module.h"

#include "chess_app.h"

#include <cmath>
#include <iostream>

render_module::render_module(chess_app& app)
    : m_app(app)
{
    const auto& configs = m_app.configs();

    /*********** setup piece sprites ***********/
    if(m_textures[tex_id::tex_pieces].loadFromFile(configs.textures + "pieces.png"))
    {
        /* pieces */
        for(int i = 0; i < 12; i++)
        {
            sf::IntRect tex_rect
            {
                configs.square_size.x * static_cast<int>(i), 0,
                configs.square_size.x, configs.square_size.y
            };

            m_sprite_pieces[i].setTexture(m_textures[tex_id::tex_pieces]);
            m_sprite_pieces[i].setTextureRect(tex_rect);
        }
    }
    else
    {
        std::cerr << "Couldn't load texture " << configs.textures + "pieces.png" << std::endl;
    }


    /*********** setup board sprite ***********/
    if(m_textures[tex_id::tex_board].loadFromFile(configs.textures + "board.png"))
    {
        m_sprites[sprite_id::sprite_board].setTexture(m_textures[tex_id::tex_board]);
    }
    else
    {
        std::cerr << "Couldn't load texture " << configs.textures + "board.png" << std::endl;
    }


    /*********** setup ui sprites ***********/
    if(m_textures[tex_id::tex_ui].loadFromFile(configs.textures + "ui.png"))
    {
        /* ui elements */
        for(auto i = 1; i < sprite_id::sprite_count; i++)
        {
            sf::IntRect tex_rect
            {
                configs.square_size.x * static_cast<int>(i-1), 0,
                configs.square_size.x, configs.square_size.y
            };

            m_sprites[i].setTexture(m_textures[tex_id::tex_ui]);
            m_sprites[i].setTextureRect(tex_rect);
        }
    }

    render_scale(static_cast<float>(configs.scale_board));
}

sf::Sprite& render_module::sprite_piece(const chess::piece piece)
{
    return m_sprite_pieces[piece];
}

sf::Sprite& render_module::sprite(const sprite_id id)
{
    return m_sprites[id];
}

void render_module::render_scale(float scale)
{
    for(auto& sprite : m_sprites)
    {
        if(sprite.getTexture() != nullptr)
        {
            sprite.setScale(scale, scale);
        }
    }

    for(auto& sprite : m_sprite_pieces)
    {
        if(sprite.getTexture() != nullptr)
        {
            sprite.setScale(scale, scale);
        }
    }

    if(m_sprites[sprite_id::sprite_board].getTexture() != nullptr)
    {
        auto& config = m_app.configs();
        auto& render_window = m_app.render_window();
        auto board_size = sf::Vector2f(m_textures[tex_id::tex_board].getSize());
        render_window.setSize(sf::Vector2u(scale*board_size.x + config.gui_size, scale*board_size.y));
        render_window.setView(sf::View({0.0f, 0.0f, scale*board_size.x + config.gui_size, scale*board_size.y}));
    }
}

void render_module::render_board()
{
    auto& render_window = m_app.render_window();
    render_window.draw(m_sprites[sprite_id::sprite_board]);
}

void render_module::render_pieces(chess::square moving_piece_sq)
{
    auto& render_window = m_app.render_window();
    auto& board_pieces = m_app.game().board_pieces();

    for(auto i = 0u; i < board_pieces.size(); i++)
    {
        auto piece = board_pieces[i];
        if(piece == chess::not_a_piece || i == moving_piece_sq) continue;

        auto& sprite = m_sprite_pieces[piece];
        sprite.setPosition( world_from_square(static_cast<chess::square>(i)) );
        render_window.draw(sprite);
    }
}

void render_module::render_piece(chess::piece t_piece, const sf::Vector2f& pos)
{
    auto& sprite = sprite_piece(t_piece);
    sprite.setPosition(pos);
    m_app.render_window().draw(sprite);
}

void render_module::render_piece_mouse(chess::piece t_piece, const sf::Vector2f &pos)
{
    auto& sprite = sprite_piece(t_piece);
    sprite.setPosition(pos - (sf::Vector2f(m_app.configs().square_size) * 0.5f * static_cast<float>(m_app.configs().scale_board)));
    m_app.render_window().draw(sprite);
}

void render_module::render_moves(const chess::move_list& t_moves)
{
    auto& game_board = m_app.game().board_pieces();

    auto& sprite_move = sprite(sprite_id::sprite_move);
    auto& sprite_attack = sprite(sprite_id::sprite_attack);
    auto& render_window = m_app.render_window();
    for(const auto& move : t_moves)
    {
        if(game_board[move.m_to] == chess::not_a_piece)
        {
            sprite_move.setPosition( world_from_square(move.m_to) );
            render_window.draw(sprite_move);
        }
        else
        {
            sprite_attack.setPosition( world_from_square(move.m_to) );
            render_window.draw(sprite_attack);
        }
    }
}

void render_module::render_square_hover(const sf::Vector2f& pos)
{
    auto sq = square_from_mouse(pos);
    auto& sprite_high = sprite(is_white_square(sq) ? sprite_id::sprite_w_tile_high : sprite_id::sprite_b_tile_high );
    sprite_high.setPosition(world_from_square(sq));

    auto& render_window = m_app.render_window();
    render_window.draw(sprite_high);
}

void render_module::render_square_select(chess::square t_sq)
{
    auto& sprite_select = sprite(sprite_id::sprite_select);
    sprite_select.setPosition(world_from_square(t_sq));

    auto& render_window = m_app.render_window();
    render_window.draw(sprite_select);
}

sf::Vector2i render_module::tile_from_square(const chess::square t_square) const
{
    return sf::Vector2i(t_square % 8, t_square / 8);
}

chess::square render_module::square_from_tile(const sf::Vector2i& pos) const
{
    if(!is_board_tile(pos)) { return chess::square::not_a_square; }
    return static_cast<chess::square>(pos.y * 8 + pos.x);
}

bool render_module::is_board_tile(const sf::Vector2i& pos) const
{
    return (0 <= pos.x && pos.x <= 7) && (0 <= pos.y && pos.y <= 7);
}

sf::Vector2f render_module::world_from_square(const chess::square t_square) const
{
    return world_from_tile( tile_from_square(t_square) );
}

sf::Vector2f render_module::world_from_tile(const sf::Vector2i& tile) const
{
    auto& configs = m_app.configs();
    auto border_offset = sf::Vector2f(configs.border_offset) * static_cast<float>(configs.scale_board);
    auto square_size =  sf::Vector2f(configs.square_size) * static_cast<float>(configs.scale_board);

    return sf::Vector2f(tile.x * square_size.x, tile.y * square_size.y) + border_offset;
}

sf::Vector2f render_module::world_interpolate_move(const sf::Vector2f& start, const sf::Vector2f& end, float dt, float speed = 120.0f)
{
    sf::Vector2f diff = end - start;
    float length_diff = std::sqrt(diff.x*diff.x + diff.y*diff.y);
    auto dir = diff / length_diff;

    sf::Vector2f move = dir * dt * speed;
    float length_move = std::sqrt(move.x*move.x + move.y*move.y);
    return length_move > length_diff ? end : start + move;
}

sf::Vector2i render_module::tile_from_mouse(const sf::Vector2f& mouse_pos) const
{
    auto& configs = m_app.configs();
    auto border_offset = sf::Vector2f(configs.border_offset) * static_cast<float>(configs.scale_board);
    auto square_size =  sf::Vector2f(configs.square_size) * static_cast<float>(configs.scale_board);

    sf::Vector2f tile = (mouse_pos - border_offset);
    tile.x /= square_size.x;
    tile.y /= square_size.y;

    tile.x = tile.x < 0.0f ? -1 : tile.x;
    tile.y = tile.y < 0.0f ? -1 : tile.y;

    return sf::Vector2i(tile);
}

chess::square render_module::square_from_mouse(const sf::Vector2f& mouse_pos) const
{
    return square_from_tile( tile_from_mouse(mouse_pos) );
}

bool render_module::is_white_square(const sf::Vector2i &tile) const
{
    return (tile.x % 2 == 0 && tile.y % 2 == 0) || (tile.x % 2 == 1 && tile.y % 2 == 1);
}

bool render_module::is_black_square(const sf::Vector2i& tile) const
{
    return !is_white_square(tile);
}

bool render_module::is_white_square(const chess::square t_square) const
{
    return is_white_square(tile_from_square(t_square));
}

bool render_module::is_black_square(const chess::square t_square) const
{
    return is_black_square(tile_from_square(t_square));
}
