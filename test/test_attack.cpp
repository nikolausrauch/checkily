#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chess/board.h>
#include <chess/move/attack.h>
#include <chess/chess_string.h>

TEST(attack, pawn_on_the_fly)
{
    using namespace chess;

    /*************** white tests ****************/
    bitboard attack = otf::pawn_attack(white, d2);
    EXPECT_EQ(attack, bit(c3) | bit(e3));

    attack = otf::pawn_attack(white, e5);
    EXPECT_EQ(attack, bit(f6) | bit(d6));

    attack = otf::pawn_attack(white, f8);
    EXPECT_EQ(attack, 0ull);

    attack = otf::pawn_attack(white, a4);
    EXPECT_EQ(attack, bit(b5));

    attack = otf::pawn_attack(white, h3);
    EXPECT_EQ(attack, bit(g4));

    /*************** black tests ****************/
    attack = otf::pawn_attack(black, d2);
    EXPECT_EQ(attack, bit(c1) | bit(e1));

    attack = otf::pawn_attack(black, e5);
    EXPECT_EQ(attack, bit(f4) | bit(d4));

    attack = otf::pawn_attack(black, f1);
    EXPECT_EQ(attack, 0ull);

    attack = otf::pawn_attack(black, a4);
    EXPECT_EQ(attack, bit(b3));

    attack = otf::pawn_attack(black, h3);
    EXPECT_EQ(attack, bit(g2));
}

TEST(attack, pawn_table)
{
    using namespace chess;

    /*************** white tests ****************/
    bitboard attack = pawn_attack(white, d2);
    EXPECT_EQ(attack, bit(c3) | bit(e3));

    attack = pawn_attack(white, e5);
    EXPECT_EQ(attack, bit(f6) | bit(d6));

    attack = pawn_attack(white, f8);
    EXPECT_EQ(attack, 0ull);

    attack = pawn_attack(white, a4);
    EXPECT_EQ(attack, bit(b5));

    attack = pawn_attack(white, h3);
    EXPECT_EQ(attack, bit(g4));

    /*************** black tests ****************/
    attack = pawn_attack(black, d2);
    EXPECT_EQ(attack, bit(c1) | bit(e1));

    attack = pawn_attack(black, e5);
    EXPECT_EQ(attack, bit(f4) | bit(d4));

    attack = pawn_attack(black, f1);
    EXPECT_EQ(attack, 0ull);

    attack = pawn_attack(black, a4);
    EXPECT_EQ(attack, bit(b3));

    attack = pawn_attack(black, h3);
    EXPECT_EQ(attack, bit(g2));
}


TEST(attack, knight_on_the_fly)
{
    using namespace chess;

    bitboard attack = otf::knight_attack(f3);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00001010"
                  "00010001"
                  "00000000"
                  "00010001"
                  "00001010"));

    attack = otf::knight_attack(a4);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "01000000"
                  "00100000"
                  "00000000"
                  "00100000"
                  "01000000"
                  "00000000"));


    attack = otf::knight_attack(g6);
    EXPECT_EQ(attack, to_bitboard(
                  "00000101"
                  "00001000"
                  "00000000"
                  "00001000"
                  "00000101"
                  "00000000"
                  "00000000"
                  "00000000"));

    attack = otf::knight_attack(h1);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000010"
                  "00000100"
                  "00000000"));

    attack = otf::knight_attack(e2);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00010100"
                  "00100010"
                  "00000000"
                  "00100010"));
}

TEST(attack, knight_table)
{
    using namespace chess;

    bitboard attack = knight_attack(f3);
    EXPECT_EQ(attack, otf::knight_attack(f3));

    attack = knight_attack(a4);
    EXPECT_EQ(attack, otf::knight_attack(a4));

    attack = knight_attack(g6);
    EXPECT_EQ(attack, otf::knight_attack(g6));

    attack = knight_attack(h1);
    EXPECT_EQ(attack, otf::knight_attack(h1));

    attack = knight_attack(e2);
    EXPECT_EQ(attack, otf::knight_attack(e2));
}


TEST(attack, king_on_the_fly)
{
    using namespace chess;

    bitboard attack = otf::king_attack(e1);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00011100"
                  "00010100"));

    attack = otf::king_attack(f4);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00001110"
                  "00001010"
                  "00001110"
                  "00000000"
                  "00000000"));

    attack = otf::king_attack(a8);
    EXPECT_EQ(attack, to_bitboard(
                  "01000000"
                  "11000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"));

    attack = otf::king_attack(h1);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000011"
                  "00000010"));
}

TEST(attack, king_table)
{
    using namespace chess;

    bitboard attack = king_attack(e1);
    EXPECT_EQ(attack, otf::king_attack(e1));

    attack = king_attack(f4);
    EXPECT_EQ(attack, otf::king_attack(f4));

    attack = king_attack(a8);
    EXPECT_EQ(attack, otf::king_attack(a8));

    attack = king_attack(h1);
    EXPECT_EQ(attack, otf::king_attack(h1));
}


TEST(attack, bishop_on_the_fly)
{
    using namespace chess;

    /******* no blocking attack masks *******/
    bitboard attack = otf::bishop_attack(d4, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "00000001"
                  "10000010"
                  "01000100"
                  "00101000"
                  "00000000"
                  "00101000"
                  "01000100"
                  "10000010"));

    attack = otf::bishop_attack(h1, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "10000000"
                  "01000000"
                  "00100000"
                  "00010000"
                  "00001000"
                  "00000100"
                  "00000010"
                  "00000000"));

    attack = otf::bishop_attack(a8, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "01000000"
                  "00100000"
                  "00010000"
                  "00001000"
                  "00000100"
                  "00000010"
                  "00000001"));

    attack = otf::bishop_attack(h8, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000010"
                  "00000100"
                  "00001000"
                  "00010000"
                  "00100000"
                  "01000000"
                  "10000000"));

    attack = otf::bishop_attack(a4, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "00001000"
                  "00010000"
                  "00100000"
                  "01000000"
                  "00000000"
                  "01000000"
                  "00100000"
                  "00010000"));

    /******* blocking attack masks *******/
    bitboard block = to_bitboard(
                "00000000"
                "00000000"
                "00000100"
                "00100000"
                "00000000"
                "00000110"
                "01000000"
                "00000000");

    attack = otf::bishop_attack(d4, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000100"
                  "00101000"
                  "00000000"
                  "00101000"
                  "01000100"
                  "00000010"));

    attack = otf::bishop_attack(h1, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000100"
                  "00000010"
                  "00000000"));

    attack = otf::bishop_attack(a8, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "01000000"
                  "00100000"
                  "00010000"
                  "00001000"
                  "00000100"
                  "00000000"
                  "00000000"));

    attack = otf::bishop_attack(h8, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000010"
                  "00000100"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"));

    attack = otf::bishop_attack(a4, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00001000"
                  "00010000"
                  "00100000"
                  "01000000"
                  "00000000"
                  "01000000"
                  "00100000"
                  "00010000"));
}

TEST(attack, bishop_table)
{
    using namespace chess;

    /******* no blocking attack masks *******/
    bitboard attack = bishop_attack(d4, 0ull);
    EXPECT_EQ(attack, otf::bishop_attack(d4, 0ull));

    attack = bishop_attack(h1, 0ull);
    EXPECT_EQ(attack, otf::bishop_attack(h1, 0ull));

    attack = bishop_attack(a8, 0ull);
    EXPECT_EQ(attack, otf::bishop_attack(a8, 0ull));

    attack = bishop_attack(h8, 0ull);
    EXPECT_EQ(attack, otf::bishop_attack(h8, 0ull));

    attack = bishop_attack(a4, 0ull);
    EXPECT_EQ(attack, otf::bishop_attack(a4, 0ull));

    /******* blocking attack masks *******/
    bitboard block = to_bitboard(
                "00000000"
                "00000000"
                "00000100"
                "00100000"
                "00000000"
                "00000110"
                "01000000"
                "00000000");

    attack = bishop_attack(d4, block);
    EXPECT_EQ(attack, otf::bishop_attack(d4, block));

    attack = bishop_attack(h1, block);
    EXPECT_EQ(attack, otf::bishop_attack(h1, block));

    attack = bishop_attack(a8, block);
    EXPECT_EQ(attack, otf::bishop_attack(a8, block));

    attack = bishop_attack(h8, block);
    EXPECT_EQ(attack, otf::bishop_attack(h8, block));

    attack = bishop_attack(a4, block);
    EXPECT_EQ(attack, otf::bishop_attack(a4, block));
}


TEST(attack, rook_on_the_fly)
{
    using namespace chess;

    /******* no blocking attack masks *******/
    bitboard attack = otf::rook_attack(d4, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "00010000"
                  "00010000"
                  "00010000"
                  "00010000"
                  "11101111"
                  "00010000"
                  "00010000"
                  "00010000"));

    attack = otf::rook_attack(h1, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "00000001"
                  "00000001"
                  "00000001"
                  "00000001"
                  "00000001"
                  "00000001"
                  "00000001"
                  "11111110"));

    attack = otf::rook_attack(b7, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "01000000"
                  "10111111"
                  "01000000"
                  "01000000"
                  "01000000"
                  "01000000"
                  "01000000"
                  "01000000"));

    /******* blocking attack masks *******/
    bitboard block = to_bitboard(
                "00000000"
                "00000000"
                "00010100"
                "00100000"
                "00000000"
                "00000111"
                "01000000"
                "00000000");

    attack = otf::rook_attack(d4, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00010000"
                  "00010000"
                  "11101111"
                  "00010000"
                  "00010000"
                  "00010000"));

    attack = otf::rook_attack(h1, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000001"
                  "00000001"
                  "11111110"));

    attack = otf::rook_attack(b7, block);
    EXPECT_EQ(attack, to_bitboard(
                  "01000000"
                  "10111111"
                  "01000000"
                  "01000000"
                  "01000000"
                  "01000000"
                  "01000000"
                  "00000000"));
}

TEST(attack, rook_table)
{
    using namespace chess;

    /******* no blocking attack masks *******/
    bitboard attack = rook_attack(d4, 0ull);
    EXPECT_EQ(attack, otf::rook_attack(d4, 0ull));

    attack = rook_attack(h1, 0ull);
    EXPECT_EQ(attack, otf::rook_attack(h1, 0ull));

    attack = rook_attack(b7, 0ull);
    EXPECT_EQ(attack, otf::rook_attack(b7, 0ull));

    /******* blocking attack masks *******/
    bitboard block = to_bitboard(
                "00000000"
                "00000000"
                "00010100"
                "00100000"
                "00000000"
                "00000111"
                "01000000"
                "00000000");

    attack = rook_attack(d4, block);
    EXPECT_EQ(attack, otf::rook_attack(d4, block));

    attack = rook_attack(h1, block);
    EXPECT_EQ(attack, otf::rook_attack(h1, block));

    attack = rook_attack(b7, block);
    EXPECT_EQ(attack, otf::rook_attack(b7, block));
}


TEST(attack, queen_on_the_fly)
{
    using namespace chess;

    /******* no blocking attack masks *******/
    bitboard attack = otf::queen_attack(d4, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "00010001"
                  "10010010"
                  "01010100"
                  "00111000"
                  "11101111"
                  "00111000"
                  "01010100"
                  "10010010"));

    attack = otf::queen_attack(h1, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "10000001"
                  "01000001"
                  "00100001"
                  "00010001"
                  "00001001"
                  "00000101"
                  "00000011"
                  "11111110"));

    attack = otf::queen_attack(b7, 0ull);
    EXPECT_EQ(attack, to_bitboard(
                  "11100000"
                  "10111111"
                  "11100000"
                  "01010000"
                  "01001000"
                  "01000100"
                  "01000010"
                  "01000001"));

    /******* blocking attack masks *******/
    bitboard block = to_bitboard(
                "00000000"
                "00000000"
                "00010100"
                "00100000"
                "00000000"
                "00000111"
                "01000000"
                "00000000");

    attack = otf::queen_attack(d4, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00010100"
                  "00111000"
                  "11101111"
                  "00111000"
                  "01010100"
                  "00010010"));

    attack = otf::queen_attack(h1, block);
    EXPECT_EQ(attack, to_bitboard(
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000000"
                  "00000101"
                  "00000011"
                  "11111110"));

    attack = otf::queen_attack(b7, block);
    EXPECT_EQ(attack, to_bitboard(
                  "11100000"
                  "10111111"
                  "11100000"
                  "01010000"
                  "01001000"
                  "01000100"
                  "01000000"
                  "00000000"));
}

TEST(attack, queen_table)
{
    using namespace chess;

    /******* no blocking attack masks *******/
    bitboard attack = queen_attack(d4, 0ull);
    EXPECT_EQ(attack, otf::queen_attack(d4, 0ull));

    attack = queen_attack(h1, 0ull);
    EXPECT_EQ(attack, otf::queen_attack(h1, 0ull));

    attack = queen_attack(b7, 0ull);
    EXPECT_EQ(attack, otf::queen_attack(b7, 0ull));

    /******* blocking attack masks *******/
    bitboard block = to_bitboard(
                "00000000"
                "00000000"
                "00010100"
                "00100000"
                "00000000"
                "00000111"
                "01000000"
                "00000000");

    attack = queen_attack(d4, block);
    EXPECT_EQ(attack, otf::queen_attack(d4, block));

    attack = queen_attack(h1, block);
    EXPECT_EQ(attack, otf::queen_attack(h1, block));

    attack = queen_attack(b7, block);
    EXPECT_EQ(attack, otf::queen_attack(b7, block));
}
