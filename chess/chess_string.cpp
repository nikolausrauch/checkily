#include "chess_string.h"

#include <sstream>

namespace chess
{

std::string to_string(const chess::bitboard bboard)
{
    std::stringstream ss;

    for(int r = 0; r < 8; r++)
    {
        ss << " " << 8 - r << " ";

        for(int f = 0; f < 8; f++)
        {
            ss << " " << (bit_get(bboard, (r*8 + f)) != 0ull ? 1 : 0);
        }

        ss << "\n";
    }

    ss << "\n    a b c d e f g h\n";

    return ss.str();
}

std::string to_string(const game_board& board)
{
    std::stringstream ss;

    auto board_array = to_char_array(board);
    for(int r = 0; r < 8; r++)
    {
        ss << " " << 8 - r << " ";
        for(int f = 0; f < 8; f++)
        {
            square sq = static_cast<square>(r*8 + f);
            ss << " " << board_array[sq];
        }
        ss << "\n";
    }

    ss << "\n    a b c d e f g h";
    ss << "\n---------[state]---------";
    ss << "\n  player     = " << player_names[board.player_move()];
    ss << "\n  en passant = " << square_names[board.en_passant()];
    ss << "\n  castle     = "
       << ((board.castle_rights() & castle_flags::w_king_side)  ? "K" : "-")
       << ((board.castle_rights() & castle_flags::w_queen_side) ? "Q" : "-")
       << ((board.castle_rights() & castle_flags::b_king_side)  ? "k" : "-")
       << ((board.castle_rights() & castle_flags::b_queen_side) ? "q" : "-");
    ss << "\n-------------------------";

    return ss.str();
}

std::array<char, 64> to_char_array(const game_board& board)
{
    std::array<char, 64> board_array;
    const auto& boards = board.boards();

    for(int r = 0; r < 8; r++)
    {
        for(int f = 0; f < 8; f++)
        {
            square sq = static_cast<square>(r*8 + f);

            int found_ = piece::not_a_piece;
            for(auto i = 0u; i < piece::piece_count; i++)
            {
                if(bit_get(boards[i], sq))
                {
                    found_ = i;
                    break;
                }
            }

            board_array[sq] = piece_names[found_];
        }
    }

    return board_array;
}

std::string to_string(const move mv)
{
    std::stringstream ss;
    ss  << square_names[mv.m_from] << square_names[mv.m_to]
        << ", piece: "       << piece_names[mv.m_piece]
        << ", capture: "     << piece_names[mv.m_capture]
        << ", promote: "     << piece_names[mv.m_promote]
        << ", double push: " << (mv.m_double ? "yes" : " - ")
        << ", en passant: "  << (mv.m_en_passant ? "yes" : " - ")
        << ", castling: "    << (mv.m_castling ? "yes" : " - ");

    return ss.str();
}

std::string to_string(const move_list& mv_list)
{
    std::stringstream ss;
    ss << "move    piece    capture    promote    double    enpassant    castle\n";
    ss << "--------------------------------------------------------------------\n";
    for(auto move : mv_list)
    {
        ss << square_names[move.m_from] << square_names[move.m_to] << "      ";
        ss << piece_names[move.m_piece]   << "         ";
        ss << piece_names[move.m_capture] << "          ";
        ss << piece_names[move.m_promote] << "        ";
        ss << (move.m_double     ? "yes" : " - ")  << "         ";
        ss << (move.m_en_passant ? "yes" : " - ")  << "        ";
        ss << (move.m_castling   ? "yes" : " - ")  << "\n";
    }

    ss << "\nTotal: " << mv_list.size();

    return ss.str();
}

std::string to_notation_string(const move mv, const bool check, const bool checkmate)
{
    auto man = color_discard(mv.m_piece);

    std::stringstream ss;
    if(mv.m_castling)
    {
        ss << ( (file_square(mv.m_to) == 2) ? "O-O-O" : "O-O");
        return ss.str();
    }

    if(man != piece::pawn) { ss << piece_names[man]; }
    if(mv.m_capture != not_a_piece)
    {
        if(man == piece::pawn) { ss << file_names[file_square(mv.m_from)]; }
        ss << "x";
    }
    ss << square_names[mv.m_to];

    if(checkmate) { ss << "#"; }
    else if(check) { ss << "+"; }

    return ss.str();
}

}
