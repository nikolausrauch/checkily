#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chess/board.h>
#include <chess/chess_format.h>
#include <chess/chess_string.h>

TEST(format, is_valid_fen)
{
    EXPECT_TRUE(chess::is_valid_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - -"));
    EXPECT_TRUE(chess::is_valid_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));
}

TEST(format, load_fen)
{
    using namespace chess;

    {
        game_board board;
        EXPECT_TRUE(load_fen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq"));

        EXPECT_EQ(board.player_move(), color::white);
        EXPECT_EQ(board.castle_rights(), castle_flags::all_castle);

        EXPECT_EQ(board.piece_on(e1), piece::w_king);
        EXPECT_EQ(board.piece_on(d1), piece::w_queen);

        EXPECT_EQ(board.piece_on(e8), piece::b_king);
        EXPECT_EQ(board.piece_on(d8), piece::b_queen);

        EXPECT_EQ(board.boards(piece::b_pawn), to_bitboard("00000000"
                                                           "11111111"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"));

        EXPECT_EQ(board.boards(piece::w_pawn), to_bitboard("00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "11111111"
                                                           "00000000"));

        EXPECT_EQ(board.boards(piece::b_rook), to_bitboard("10000001"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"));

        EXPECT_EQ(board.boards(piece::w_bishop), to_bitboard("00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00100100"));

        EXPECT_EQ(board.occupancy(color::both), to_bitboard("11111111"
                                                             "11111111"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "11111111"
                                                             "11111111"));
    }

    {
        chess::game_board board;
        EXPECT_TRUE(chess::load_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

        EXPECT_EQ(board.player_move(), color::white);
        EXPECT_EQ(board.castle_rights(), castle_flags::all_castle);

        EXPECT_EQ(board.piece_on(e1), piece::w_king);
        EXPECT_EQ(board.piece_on(f3), piece::w_queen);

        EXPECT_EQ(board.piece_on(e8), piece::b_king);
        EXPECT_EQ(board.piece_on(e7), piece::b_queen);

        EXPECT_EQ(board.boards(piece::b_pawn), to_bitboard("00000000"
                                                           "10110100"
                                                           "00001010"
                                                           "00000000"
                                                           "01000000"
                                                           "00000001"
                                                           "00000000"
                                                           "00000000"));

        EXPECT_EQ(board.boards(piece::w_pawn), to_bitboard("00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00010000"
                                                           "00001000"
                                                           "00000000"
                                                           "11100111"
                                                           "00000000"));

        EXPECT_EQ(board.boards(piece::b_rook), to_bitboard("10000001"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"
                                                           "00000000"));

        EXPECT_EQ(board.boards(piece::w_bishop), to_bitboard("00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00000000"
                                                             "00011000"
                                                             "00000000"));

        EXPECT_EQ(board.occupancy(color::both), to_bitboard("10001001"
                                                             "10111110"
                                                             "11001110"
                                                             "00011000"
                                                             "01001000"
                                                             "00100101"
                                                             "11111111"
                                                             "10001001"));
    }
}

TEST(format, save_fen)
{
    {
        const std::string base_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
        chess::game_board board;
        EXPECT_TRUE(chess::load_fen(board, base_fen));
        EXPECT_EQ(chess::to_fen_string(board), base_fen);
    }

    {
        const std::string base_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
        chess::game_board board;
        EXPECT_TRUE(chess::load_fen(board, base_fen));
        EXPECT_EQ(chess::to_fen_string(board), base_fen);
    }
}
