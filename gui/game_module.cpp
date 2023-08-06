#include "game_module.h"

#include "chess_app.h"

#include <chess/chess_format.h>
#include <chess/move/movegen.h>

game_module::game_module(chess_app &app)
    : m_app(app),
      m_players{ { {"Player White", chess::white}, {"Player Black", chess::black} } }
{
    m_moves.reserve(256);
    new_game();
}

bool game_module::new_game()
{
    chess::load_fen(m_board, chess::fen_startposition);
    update_pieces();

    m_game_result = result::unknown;
    m_moves.clear();

    if(!m_players[chess::white].new_game(engine(chess::white))) { return false; }
    if(!m_players[chess::black].new_game(engine(chess::black))) { return false; }

    return true;
}

game_module::result game_module::make_move(chess::move move)
{
    auto& player = m_players[m_board.player_move()];
    if(move.m_capture != chess::not_a_piece)
    {
        player.m_captured[move.m_capture]++;
    }

    m_board.make(move);
    m_moves.push_back(chess::record{move, m_board.check(), m_board.check_mate()});
    update_pieces();

    auto& sound = m_app.sound();
    sound.play(move);

    return check_onboard_result();
}

chess::game_board& game_module::board()
{
    return m_board;
}

const chess::game_board& game_module::board() const
{
    return m_board;
}

const chess::game_record& game_module::moves() const
{
    return m_moves;
}

game_module::result game_module::game_result() const
{
    return m_game_result;
}

game_module::player& game_module::player_info(chess::color color)
{
    return m_players[color];
}

const game_module::player& game_module::player_info(chess::color color) const
{
    return m_players[color];
}

uci_process& game_module::engine(chess::color color)
{
    return m_engines[color];
}

uci_process& game_module::eval_engine()
{
    return m_engines[2];
}

const int game_module::material_imbalance(chess::color color)
{
    const player& me = m_players[color];
    const player& other = m_players[chess::opponent(color)];

    static constexpr std::array<int, chess::piece_count> values =
    {
        1, 3, 0, 3, 5, 9,
        1, 3, 0, 3, 5, 9
    };

    auto captured_me  = 0;
    for(unsigned int i = 0; i < me.m_captured.size(); i++)
    {
        captured_me += values[i] * me.m_captured[i];
    }

    auto captured_other  = 0;
    for(unsigned int i = 0; i < other.m_captured.size(); i++)
    {
        captured_other += values[i] * other.m_captured[i];
    }

    return captured_me - captured_other;
}

const std::array<chess::piece, chess::square_count>& game_module::board_pieces() const
{
    return m_board_pieces;
}

chess::move_list game_module::piece_moves(chess::piece piece, chess::square square) const
{
    return chess::generate_legal_moves(m_board, piece, square);
}

void game_module::update_pieces()
{
    std::fill(m_board_pieces.begin(), m_board_pieces.end(), chess::piece::not_a_piece);
    for(auto sq = 0u; sq < chess::square_count; sq++)
    {
        m_board_pieces[sq] = m_board.piece_on(static_cast<chess::square>(sq));
    }
}

game_module::result game_module::check_onboard_result()
{
    if(m_board.check_mate())
    {
        m_game_result = m_board.player_move() == chess::white ? result::checkmate_white : result::checkmate_black;
        return m_game_result;
    }

    auto moves = chess::generate_legal_moves(m_board);
    if(moves.empty() && !m_board.check_mate())
    {
        m_game_result = result::stalemate;
        return m_game_result;
    }

    m_game_result = result::unknown;
    return m_game_result;
}

game_module::player::player(const std::string& name, chess::color color)
    : m_name(name), m_captured(), m_time(sf::microseconds(0)), m_color(color), m_ai(false), m_filepath(512, '\0')
{

}

const std::string& game_module::player::name() const
{
    return m_name;
}

const std::array<unsigned short, chess::piece_count>& game_module::player::captured() const
{
    return m_captured;
}

const sf::Time& game_module::player::time() const
{
    return m_time;
}

chess::color game_module::player::color() const
{
    return m_color;
}

bool game_module::player::is_ai() const
{
    return m_ai;
}

bool game_module::player::new_game(uci_process& engine)
{
    /* TODO: reset time */
    std::fill(m_captured.begin(), m_captured.end(), 0);

    if(m_ai && engine.uciok())
    {
        m_name = engine.name();
        engine.newgame();
        engine.newgame();
        if(!engine.try_isready(2000))
        {
            return false;
        }
    }

    return true;
}
