#include "movegen.h"
#include "chess/move/attack.h"
#include <algorithm>

namespace chess
{

template<color Player, color Opponent = opponent(Player)>
bool is_attacked(const game_board& board, square sq)
{
    /************ leaping pieces *************/
    constexpr piece pawn_atk = color_piece(Opponent, pawn);
    if(pawn_attack(Player, sq) & board.boards(pawn_atk)) return true;

    constexpr piece knight_atk = color_piece(Opponent, knight);
    if(knight_attack(sq) & board.boards(knight_atk)) return true;

    constexpr piece king_atk = color_piece(Opponent, king);
    if(king_attack(sq) & board.boards(king_atk)) return true;


    /************ sliding pieces *************/
    bitboard occupancy = board.occupancy(color::both);

    constexpr piece bishop_atk = color_piece(Opponent, bishop);
    if(bishop_attack(sq, occupancy) & board.boards(bishop_atk)) return true;

    constexpr piece rook_atk = color_piece(Opponent, rook);
    if(rook_attack(sq, occupancy) & board.boards(rook_atk)) return true;

    constexpr piece queen_atk = color_piece(Opponent, queen);
    if(queen_attack(sq, occupancy) & board.boards(queen_atk)) return true;

    return false;
}

template<color Player, color Opponent = opponent(Player)>
void generate_pawn_moves(const game_board &board, move_list& mv_list)
{
    constexpr piece move_piece = color_piece(Player, piece::pawn);
    constexpr bitboard double_rank = Player == white ? rank_2 : rank_7;
    constexpr bitboard promotion_rank = Player == white ? rank_8 : rank_1;
    constexpr int move_offset = Player == white ? -8 : 8;

    bitboard pawn_board = board.boards(move_piece);
    bitboard occupancy = board.occupancy(color::both);

    while(pawn_board)
    {
        auto sq = static_cast<square>(bit_ffs(pawn_board));
        bit_pop(pawn_board, sq);

        /********************* quiet moves *********************/
        auto target_sq = sq + move_offset;
        bitboard target_mask = bit(target_sq);
        if(~occupancy & target_mask)
        {
            /********************* promotion *********************/
            if(target_mask & promotion_rank)
            {
                mv_list.push_back(encode_move(sq, target_sq, move_piece, not_a_piece, color_piece(Player, queen), no_move_flag));
                mv_list.push_back(encode_move(sq, target_sq, move_piece, not_a_piece, color_piece(Player, rook), no_move_flag));
                mv_list.push_back(encode_move(sq, target_sq, move_piece, not_a_piece, color_piece(Player, bishop), no_move_flag));
                mv_list.push_back(encode_move(sq, target_sq, move_piece, not_a_piece, color_piece(Player, knight), no_move_flag));
            }
            else
            {
                mv_list.push_back(encode_move(sq, target_sq, move_piece, not_a_piece, not_a_piece, no_move_flag));

                if( (bit(sq) & double_rank) && !(occupancy & bit(target_sq + move_offset)))
                {
                    mv_list.push_back(encode_move(sq, target_sq + move_offset, move_piece, not_a_piece, not_a_piece, double_push));
                }
            }
        }

        /********************* en passant attack *********************/
        bitboard attack = pawn_attack(Player, sq);
        if(auto en_passant_attack = attack & bit(board.en_passant()))
        {
            mv_list.push_back(encode_move(sq, board.en_passant(), move_piece, color_piece(Opponent, piece::pawn), not_a_piece, en_passant));
        }


        /********************* attack *********************/
        attack &= board.occupancy(Opponent);
        while(attack)
        {
            auto attack_sq = static_cast<square>(bit_ffs(attack));
            auto captured = board.piece_on(attack_sq);
            bit_pop(attack, attack_sq);

            /********************* promotion *********************/
            if(bit(attack_sq) & promotion_rank)
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, captured, color_piece(Player, queen), no_move_flag));
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, captured, color_piece(Player, rook), no_move_flag));
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, captured, color_piece(Player, bishop), no_move_flag));
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, captured, color_piece(Player, knight), no_move_flag));
            }
            else
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, captured, not_a_piece, no_move_flag));
            }
        }
    }
}

template<color Player, color Opponent = opponent(Player)>
void generate_king_moves(const game_board& board, move_list& mv_list)
{
    constexpr bitboard queen_side_mask = Player == white ? (bit(d1) | bit(c1) |  bit(b1)) : (bit(d8) | bit(c8) |  bit(b8));
    constexpr bitboard king_side_mask = Player == white ? (bit(f1) | bit(g1)) : (bit(f8) | bit(g8));

    constexpr std::array<square, 3> queen_side_square = Player == white ?
                std::array<square, 3>{e1, d1, c1} : std::array<square, 3>{e8, d8, c8};

    constexpr std::array<square, 3> king_side_square = Player == white ?
                std::array<square, 3>{e1, f1, g1} : std::array<square, 3>{e8, f8, g8};

    bitboard occupancy = board.occupancy(color::both);
    constexpr piece move_piece = color_piece(Player, king);

    constexpr flags castle_state_q = Player == white ? w_queen_side : b_queen_side;
    bool queen_side = board.castle_rights() & castle_state_q;
    if(queen_side && !(occupancy & queen_side_mask)
            && !is_attacked<Player>(board, queen_side_square[0])
            && !is_attacked<Player>(board, queen_side_square[1])
            && !is_attacked<Player>(board, queen_side_square[2])
    )
    {
        mv_list.push_back(encode_move(queen_side_square[0], queen_side_square[2], move_piece, not_a_piece, not_a_piece, castling));
    }

    constexpr flags castle_state_k = Player == white ? w_king_side : b_king_side;
    bool king_side = board.castle_rights() & castle_state_k;
    if(king_side && !(occupancy & king_side_mask)
            && !is_attacked<Player>(board, king_side_square[0])
            && !is_attacked<Player>(board, king_side_square[1])
            && !is_attacked<Player>(board, king_side_square[2])
    )
    {
        mv_list.push_back(encode_move(king_side_square[0], king_side_square[2], move_piece, not_a_piece, not_a_piece, castling));
    }


    bitboard bb_board = board.boards(move_piece);
    bitboard occup_player = board.occupancy(Player);
    bitboard occup_opponent = board.occupancy(Opponent);

    while(bb_board)
    {
        auto sq = static_cast<square>(bit_ffs(bb_board));
        bit_pop(bb_board, sq);

        bitboard attack = king_attack(sq) & ~occup_player;
        while(attack)
        {
            auto attack_sq = static_cast<square>(bit_ffs(attack));
            if(attack & (occup_opponent & bit(attack_sq)))
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, board.piece_on(attack_sq), not_a_piece, no_move_flag));
            }
            else
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, not_a_piece, not_a_piece, no_move_flag));
            }

            bit_pop(attack, attack_sq);
        }
    }
}

template<color Player, color Opponent = opponent(Player)>
void generate_knight_moves(const game_board& board, move_list& mv_list)
{
    constexpr piece move_piece = color_piece(Player, piece::knight);

    bitboard bb_board = board.boards(move_piece);
    bitboard occup_player = board.occupancy(Player);
    bitboard occup_opponent = board.occupancy(Opponent);

    while(bb_board)
    {
        auto sq = static_cast<square>(bit_ffs(bb_board));
        bit_pop(bb_board, sq);

        bitboard attack = knight_attack(sq) & ~occup_player;
        while(attack)
        {
            auto attack_sq = static_cast<square>(bit_ffs(attack));

            if(attack & (occup_opponent & bit(attack_sq)))
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, board.piece_on(attack_sq), not_a_piece, no_move_flag));
            }
            else
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, not_a_piece, not_a_piece, no_move_flag));
            }

            bit_pop(attack, attack_sq);
        }
    }
}

typedef bitboard(*attack_func)(const square, const bitboard);
template<color Player, piece Piece, attack_func AttackFunc, color Opponent = opponent(Player)>
void generate_slider_moves(const game_board& board, move_list& mv_list)
{
    constexpr piece move_piece = color_piece(Player, Piece);
    bitboard bb_board = board.boards(move_piece);
    bitboard occupancy = board.occupancy(color::both);
    bitboard occup_player = board.occupancy(Player);
    bitboard occup_opponent = board.occupancy(Opponent);

    while(bb_board)
    {
        auto sq = static_cast<square>(bit_ffs(bb_board));
        bit_pop(bb_board, sq);

        bitboard attack = AttackFunc(sq, occupancy) & ~occup_player;

        while(attack)
        {
            auto attack_sq = static_cast<square>(bit_ffs(attack));

            if(attack & (occup_opponent & bit(attack_sq)))
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, board.piece_on(attack_sq), not_a_piece, no_move_flag));
            }
            else
            {
                mv_list.push_back(encode_move(sq, attack_sq, move_piece, not_a_piece, not_a_piece, no_move_flag));
            }

            bit_pop(attack, attack_sq);
        }
    }
}

move_list generate_pseudo_legal_moves(const game_board& board)
{
    move_list moves;

    if(board.player_move() == color::white)
    {
        generate_pawn_moves<color::white>(board, moves);
        generate_king_moves<color::white>(board, moves);
        generate_knight_moves<color::white>(board, moves);
        generate_slider_moves<color::white, piece::bishop, bishop_attack>(board, moves);
        generate_slider_moves<color::white, piece::rook, rook_attack>(board, moves);
        generate_slider_moves<color::white, piece::queen, queen_attack>(board, moves);
    }
    else
    {
        generate_pawn_moves<color::black>(board, moves);
        generate_king_moves<color::black>(board, moves);
        generate_knight_moves<color::black>(board, moves);
        generate_slider_moves<color::black, piece::bishop, bishop_attack>(board, moves);
        generate_slider_moves<color::black, piece::rook, rook_attack>(board, moves);
        generate_slider_moves<color::black, piece::queen, queen_attack>(board, moves);
    }

    return moves;
}

move_list generate_pseudo_legal_moves(const game_board& board, const piece man, const square sq)
{
    move_list moves;
    auto color = color_of(man);
    if(color == color::white)
    {
        if(man == piece::w_pawn) generate_pawn_moves<color::white>(board, moves);
        if(man == piece::w_king) generate_king_moves<color::white>(board, moves);
        if(man == piece::w_knight) generate_knight_moves<color::white>(board, moves);
        if(man == piece::w_bishop) generate_slider_moves<color::white, piece::bishop, bishop_attack>(board, moves);
        if(man == piece::w_rook) generate_slider_moves<color::white, piece::rook, rook_attack>(board, moves);
        if(man == piece::w_queen) generate_slider_moves<color::white, piece::queen, queen_attack>(board, moves);
    }
    else
    {
        if(man == piece::b_pawn) generate_pawn_moves<color::black>(board, moves);
        if(man == piece::b_king) generate_king_moves<color::black>(board, moves);
        if(man == piece::b_knight) generate_knight_moves<color::black>(board, moves);
        if(man == piece::b_bishop) generate_slider_moves<color::black, piece::bishop, bishop_attack>(board, moves);
        if(man == piece::b_rook) generate_slider_moves<color::black, piece::rook, rook_attack>(board, moves);
        if(man == piece::b_queen) generate_slider_moves<color::black, piece::queen, queen_attack>(board, moves);
    }

    moves.erase(std::remove_if(moves.begin(), moves.end(), [sq](auto move)
    {
        return move.m_from != sq;
    }), moves.end());

    return moves;
}

move_list generate_legal_moves(const game_board& board)
{
    auto copy_board = board;

    auto moves = generate_pseudo_legal_moves(board);

    moves.erase(std::remove_if(moves.begin(), moves.end(), [&copy_board](auto m)
    {
        auto en_passant = copy_board.en_passant();
        auto castle_right = copy_board.castle_rights();

        copy_board.make(m);
        auto player_attack = copy_board.player_move();
        auto king_sq = static_cast<square>( bit_ffs(copy_board.boards(color_piece(opponent(player_attack), piece::king))) );
        bool check = player_attack == color::white ?
                    is_attacked<black>(copy_board, king_sq)
                  : is_attacked<white>(copy_board, king_sq);

        copy_board.unmake(m, en_passant, castle_right);

        return check;
    }), moves.end());

    return moves;
}

move_list generate_legal_moves(const game_board &board, const piece man, const square sq)
{
    auto copy_board = board;

    auto moves = generate_pseudo_legal_moves(board, man, sq);

    moves.erase(std::remove_if(moves.begin(), moves.end(), [&copy_board](auto m)
    {
        auto en_passant = copy_board.en_passant();
        auto castle_right = copy_board.castle_rights();

        copy_board.make(m);
        auto player_attack = copy_board.player_move();
        auto king_sq = static_cast<square>( bit_ffs(copy_board.boards(color_piece(opponent(player_attack), piece::king))) );
        bool check = is_attacked(copy_board, opponent(player_attack), king_sq);

        copy_board.unmake(m, en_passant, castle_right);

        return check;
    }), moves.end());

    return moves;
}

bool is_attacked(const game_board& board, color defender, const square sq)
{
    return defender == color::white ?
                is_attacked<white>(board, sq)
              : is_attacked<black>(board, sq);
}

}
