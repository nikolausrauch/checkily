#include "chess_format.h"
#include "chess/chess_string.h"

#include <regex>
#include <unordered_map>

namespace chess
{

namespace detail
{

static const std::unordered_map<char, piece> map_piece
{
    {'P', piece::w_pawn},
    {'N', piece::w_knight},
    {'B', piece::w_bishop},
    {'R', piece::w_rook},
    {'Q', piece::w_queen},
    {'K', piece::w_king},

    {'p', piece::b_pawn},
    {'n', piece::b_knight},
    {'b', piece::b_bishop},
    {'r', piece::b_rook},
    {'q', piece::b_queen},
    {'k', piece::b_king}
};

std::vector<std::string> tokenize(const std::string &t_fen)
{
    const std::regex fenRegex("\\s+");

    std::sregex_token_iterator iter(t_fen.begin(), t_fen.end(), fenRegex, -1);
    std::sregex_token_iterator end;

    std::vector<std::string> tokens;
    while (iter != end)
    {
        tokens.emplace_back(*iter);
        ++iter;
    }

    return tokens;
}

}

bool is_valid_fen(const std::string& str)
{
    auto tokens = detail::tokenize(str);
    return is_valid_fen(tokens);
}

bool is_valid_fen(const std::vector<std::string>& str_tokens)
{
    if(str_tokens.empty()) { return false; }

    static const std::regex placement_regex(R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8})");

    return std::regex_match(str_tokens[0], placement_regex);
}

bool load_fen(game_board& board, const std::string& str)
{
    auto tokens = detail::tokenize(str);
    return load_fen(board, tokens);
}

bool load_fen(game_board &board, const std::vector<std::string> &tokens)
{
    board = game_board();

    if(!is_valid_fen(tokens)) { return false; }

    /********** piece positions (set bitboards) **********/
    size_t row = 0;
    size_t col = 0;
    for(auto i = 0u; i < tokens[0].size(); i++)
    {
        char c = tokens[0][i];
        if (c == '/') { row++; col = 0; }
        else if (std::isdigit(c)) { col += c - '0'; }
        else if (std::isalpha(c))
        {
            auto piece_ = detail::map_piece.at(c);
            bit_set(board.m_bitboards[piece_], row*8 + col);
            bit_set(board.m_occupancies[color_of(piece_)], row*8 + col);
            bit_set(board.m_occupancies[color::both], row*8 + col);
            ++col;
        }
    }


    /********** player to move **********/
    board.m_player_move = (tokens[1][0] == 'w') ? color::white : color::black;

    /********** castling rights **********/
    if(tokens.size() >= 3 && tokens[2][0] != '-')
    {
        for(auto i = 0u; i < tokens[2].size(); i++)
        {
            char c = tokens[2][i];
            switch (c)
            {
            case 'K': board.m_castle_rights |= castle_flags::w_king_side; break;
            case 'Q': board.m_castle_rights |= castle_flags::w_queen_side; break;
            case 'k': board.m_castle_rights |= castle_flags::b_king_side; break;
            case 'q': board.m_castle_rights |= castle_flags::b_queen_side; break;
            default: break;
            }
        }
    }

    /********** en passant square **********/
    if(tokens.size() >= 4 && (std::isalpha(tokens[3][0]) && std::isdigit(tokens[3][1])))
    {
        auto file_ = tokens[3][0] - 'a';
        auto rank_ = tokens[3][1] - '1';
        if(0 > file_ || file_ > 7 || 0 > rank_ || rank_ > 7) {  return false; }

        board.m_en_passant_square = static_cast<square>( (7-rank_)*8 + file_);
    }

    return true;
}

std::string to_fen_string(const game_board& board)
{
    std::stringstream ss;

    auto board_array = chess::to_char_array(board);

    int empty_count = 0;
    for(int r = 0; r < 8; r++)
    {
        for(int f = 0; f < 8; f++)
        {
            square sq = static_cast<square>(r*8 + f);
            if(board_array[sq] == '.') { empty_count++; }
            else
            {
                if(empty_count > 0) { ss << std::to_string(empty_count); empty_count = 0;  }
                ss << board_array[sq];
            }
        }

        if(empty_count > 0) { ss << std::to_string(empty_count); empty_count = 0;  }
        if(r != 7) { ss << "/"; }
    }

    ss << " " << ( board.player_move() == color::white  ? 'w' : 'b' );

    if(board.castle_rights() != castle_flags::no_castle)
    {
        ss << " "
        << ((board.castle_rights() & castle_flags::w_king_side)  ? "K" : "")
        << ((board.castle_rights() & castle_flags::w_queen_side) ? "Q" : "")
        << ((board.castle_rights() & castle_flags::b_king_side)  ? "k" : "")
        << ((board.castle_rights() & castle_flags::b_queen_side) ? "q" : "");
    }
    else
    {
        ss << " -";
    }

    if(board.en_passant() != not_a_square)
    {
        ss << " " << square_names[board.en_passant()];
    }
    else
    {
        ss << " -";
    }

    return ss.str();
}

}
