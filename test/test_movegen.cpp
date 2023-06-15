#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <chess/board.h>
#include <chess/chess_format.h>
#include <chess/move/movegen.h>

TEST(move_generation, pseudo_legal_all)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

    auto move_list = generate_pseudo_legal_moves(board);

    /************ selected piece moves ***********/
    auto it = std::find(move_list.begin(), move_list.end(),
                        encode_move(a2, a4, w_pawn, not_a_piece, not_a_piece, double_push));
    EXPECT_TRUE(it != move_list.end());

    it = std::find(move_list.begin(), move_list.end(),
                   encode_move(g2, h3, w_pawn, b_pawn, not_a_piece, no_move_flag));
    EXPECT_TRUE(it != move_list.end());

    it = std::find(move_list.begin(), move_list.end(),
                   encode_move(e5, f7, w_knight, b_pawn, not_a_piece, no_move_flag));
    EXPECT_TRUE(it != move_list.end());

    it = std::find(move_list.begin(), move_list.end(),
                   encode_move(f3, f6, w_queen, b_knight, not_a_piece, no_move_flag));
    EXPECT_TRUE(it != move_list.end());


    /************ selected piece move count ***********/
    auto count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_castling; });
    EXPECT_EQ(count, 2);

    count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_double; });
    EXPECT_EQ(count, 2);

    count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_piece == w_king; });
    EXPECT_EQ(count, 4);

    count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_piece == w_queen; });
    EXPECT_EQ(count, 9);

    count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_capture != not_a_piece; });
    EXPECT_EQ(count, 8);
}

TEST(move_generation, legal_all)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r3k2r/p1Npqpb1/bnp1pnp1/3PN3/1p2P3/5Q1p/PPPBBPPP/R3K2R b KQkq"));

    auto move_list = generate_pseudo_legal_moves(board);
}

TEST(move_generation, promotion)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r2k3r/p1p1qPb1/bn1p1np1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQ"));

    auto move_list = generate_legal_moves(board);
    auto count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_promote != not_a_piece; });
    EXPECT_EQ(count, 4);
}

TEST(move_generation, castling)
{
    using namespace chess;

    {
        game_board board;
        EXPECT_TRUE(load_fen(board, "r2k3r/p1Npqpb1/bnp1pnp1/3PN3/1p2P3/5Q1p/PPPB1PPP/R2BK2R w KQ"));

        auto move_list = generate_legal_moves(board);
        auto count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_castling; });
        EXPECT_EQ(count, 0);
    }

    {
        game_board board;
        EXPECT_TRUE(load_fen(board, "r2k4/p1Npqpb1/b3pn2/2pnN1BB/1p2P3/7p/PPP2PPP/R3K2R w KQ"));

        auto move_list = generate_legal_moves(board);
        auto count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_castling; });
        EXPECT_EQ(count, 1);
    }
}

TEST(move_generation, double_push)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r2k1B1r/p5b1/bnp2np1/1N2p3/4P3/5Q2/PPP1BPpP/R3K2R w KQkq"));

    auto move_list = generate_legal_moves(board);
    auto count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_double; });
    EXPECT_EQ(count, 4);
}

TEST(move_generation, en_passant)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3"));

    auto move_list = generate_legal_moves(board);
    auto count = std::count_if(move_list.begin(), move_list.end(), [](move m) { return m.m_to == a3 && m.m_en_passant; });
    EXPECT_EQ(count, 1);
}



/**************************** PERFT ROUTINE ****************************/
enum perft_counter
{
    perft_all,
    perft_capture,
    perft_en_passant,
    perft_castles,
    perft_promotion,
    perft_counter
};

using perft_result = std::array<unsigned int, perft_counter>;

void perft(unsigned int depth, chess::game_board& board, perft_result& result,
           chess::move mv = chess::encode_move(0, 0, chess::not_a_piece, chess::not_a_piece, chess::not_a_piece, chess::no_move_flag))
{
    if(depth == 0)
    {
        result[perft_all]++;
        result[perft_capture] += mv.m_capture != chess::not_a_piece ? 1 : 0;
        result[perft_promotion] += mv.m_promote != chess::not_a_piece ? 1 : 0;
        result[perft_castles] += mv.m_castling ? 1 : 0;
        result[perft_en_passant] += mv.m_en_passant ? 1 : 0;
        return;
    }

    auto mv_list = chess::generate_legal_moves(board);
    for(auto m : mv_list)
    {
        auto ep = board.en_passant();
        auto cr = board.castle_rights();

        board.make(m);

        perft(depth - 1, board, result, m);

        board.unmake(m, ep, cr);
    }
}

TEST(move_generation, perft_startpos)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq"));

    std::array< perft_result, 6 > result =
    {{
        {      1,     0,   0, 0, 0},
        {     20,     0,   0, 0, 0},
        {    400,     0,   0, 0, 0},
        {   8902,    34,   0, 0, 0},
        { 197281,  1576,   0, 0, 0},
        {4865609, 82719, 258, 0, 0 }
    }};

    for(auto depth = 0u; depth < result.size(); depth++)
    {
        perft_result counter{0, 0, 0, 0, 0};
        perft(depth, board, counter);

        EXPECT_EQ(counter, result[depth]);
    }
}

TEST(move_generation, perft_kiwipete)
{
    using namespace chess;

    game_board board;
    EXPECT_TRUE(load_fen(board, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "));

    std::array< perft_result, 5 > result =
    {{
        {         1,       0,      0,       0,     0},
        {        48,       8,      0,       2,     0},
        {      2039,     351,      1,      91,     0},
        {     97862,    17102,    45,    3162,     0},
        {   4085603,   757163,  1929,  128013, 15172},
        //{ 193690690, 35043416, 73365, 4993637,  8392}
    }};

    for(auto depth = 0u; depth < result.size(); depth++)
    {
        perft_result counter{0, 0, 0, 0, 0};
        perft(depth, board, counter);

        EXPECT_EQ(counter, result[depth]);
    }
}
