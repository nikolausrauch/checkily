#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chess/board.h>
#include <chess/chess_format.h>
#include <chess/move/movegen.h>

TEST(board, check)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r3k2r/p1Npqpb1/bnp1pnp1/3PN3/1p2P3/5Q1p/PPPBBPPP/R3K2R b KQkq"));
    EXPECT_TRUE(board.check());

    auto move_list = generate_legal_moves(board);
    EXPECT_EQ(move_list.size(), 2);
}

TEST(board, check_mate)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r3kq1r/p1N5/bn1p1Qpb/4N3/1p2P3/7p/PPPBBPPP/R3K2R b KQkq"));
    EXPECT_TRUE(board.check());
    EXPECT_TRUE(board.check_mate());

    auto move_list = generate_legal_moves(board);
    EXPECT_EQ(move_list.size(), 0);
}

TEST(board, make_move)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

    board.make(encode_move(e2, a6, w_bishop, b_bishop, not_a_piece, no_move_flag));
    EXPECT_EQ(to_fen_string(board), "r3k2r/p1ppqpb1/Bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R3K2R b KQkq -");

    board.make(encode_move(b4, c3, b_pawn, w_knight, not_a_piece, no_move_flag));
    EXPECT_EQ(to_fen_string(board), "r3k2r/p1ppqpb1/Bn2pnp1/3PN3/4P3/2p2Q1p/PPPB1PPP/R3K2R w KQkq -");

    board.make(encode_move(f3, c3, w_queen, b_pawn, not_a_piece, no_move_flag));
    EXPECT_EQ(to_fen_string(board), "r3k2r/p1ppqpb1/Bn2pnp1/3PN3/4P3/2Q4p/PPPB1PPP/R3K2R b KQkq -");
}

TEST(board, unmake_move)
{
    using namespace chess;

    game_board board_test;
    EXPECT_TRUE(load_fen(board_test, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

    move m = encode_move(e2, a6, w_bishop, b_bishop, not_a_piece, no_move_flag);
    game_board copy_board = board_test;
    board_test.make(m);
    board_test.unmake(m, copy_board.en_passant(), copy_board.castle_rights());
    EXPECT_EQ(board_test, copy_board);
    board_test.make(m);

    m = encode_move(b4, c3, b_pawn, w_knight, not_a_piece, no_move_flag);
    copy_board = board_test;
    board_test.make(m);
    board_test.unmake(m, copy_board.en_passant(), copy_board.castle_rights());
    EXPECT_EQ(board_test, copy_board);
    board_test.make(m);

    m = encode_move(f3, c3, w_queen, b_pawn, not_a_piece, no_move_flag);
    copy_board = board_test;
    board_test.make(m);
    board_test.unmake(m, copy_board.en_passant(), copy_board.castle_rights());
    EXPECT_EQ(board_test, copy_board);
    board_test.make(m);

    m = encode_move(e8, g8, b_king, not_a_piece, not_a_piece, castling);
    copy_board = board_test;
    board_test.make(m);
    board_test.unmake(m, copy_board.en_passant(), copy_board.castle_rights());
    EXPECT_EQ(board_test, copy_board);
    board_test.make(m);

    m = encode_move(e1, g1, w_king, not_a_piece, not_a_piece, castling);
    copy_board = board_test;
    board_test.make(m);
    board_test.unmake(m, copy_board.en_passant(), copy_board.castle_rights());
    EXPECT_EQ(board_test, copy_board);
}

TEST(board, make_unmake_all)
{
    using namespace chess;

    game_board board_test;
    EXPECT_TRUE(load_fen(board_test, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

    auto move_list = generate_legal_moves(board_test);
    for(auto m : move_list)
    {
        auto copy_board = board_test;
        board_test.make(m);
        board_test.unmake(m, copy_board.en_passant(), copy_board.castle_rights());
        EXPECT_EQ(board_test, copy_board);
    }
}


void rec_move(chess::game_board& board, int depth)
{
    if(depth == 0) { return; }

    auto mv_list = chess::generate_legal_moves(board);
    for(auto m : mv_list)
    {
        auto ep = board.en_passant();
        auto cr = board.castle_rights();

        board.make(m);

        rec_move(board, depth - 1);

        board.unmake(m, ep, cr);
    }
}

TEST(board, make_unmake_recursive)
{
    using namespace chess;

    game_board board_test;
    EXPECT_TRUE(load_fen(board_test, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

    auto copy_board = board_test;
    rec_move(board_test, 4);
    EXPECT_EQ(board_test, copy_board);
}
