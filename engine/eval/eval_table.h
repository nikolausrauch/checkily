#pragma once

#include "base.h"

namespace ckly
{

template<table_names Table>
struct evaluation_func
{
    /* evaluate board position (material + piece square table) */
    static score evaluate(const chess::game_board& board)
    {
        int side_sign = board.player_move() == chess::white ? 1 : -1;
        int score = 0;

        /* material score */
        for(unsigned int i = chess::w_pawn; i < chess::piece_count; i++)
        {
            auto bb = board.boards(static_cast<chess::piece>(i));
            score += eval_table<Table>::material[i] * bit_count(bb);
        }

        /* positional score */
        for(unsigned int i = chess::w_pawn; i < chess::piece_count; i++)
        {
            auto color = chess::color_of(static_cast<chess::piece>(i));
            auto bb = board.boards(static_cast<chess::piece>(i));
            while(bb)
            {
                auto sq = static_cast<chess::square>(bit_ffs(bb));
                bit_pop(bb, sq);

                sq = (color == chess::white) ? sq : chess::mirror_square(sq);
                score += (color == chess::white) ? eval_table<Table>::position[i % 6][sq] : -eval_table<Table>::position[i % 6][sq];
            }
        }

        return score * side_sign;
    }

    /* mate score penalty */
    static constexpr score mating_score() { return eval_table<Table>::mating_score; }
};

}
