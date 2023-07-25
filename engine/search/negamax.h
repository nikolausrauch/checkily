#pragma once

#include "../eval/base.h"

#include <chess/board.h>
#include <chess/move.h>
#include <chess/move/movegen.h>

#include <limits>

namespace ckly
{

template<evaluation_func EvalFunc>
score negamax(chess::game_board& board, score alpha, score beta, int depth)
{
    if(depth == 0) // TODO: or game over?
    {
        return EvalFunc(board);
    }

    auto mvs = chess::generate_legal_moves(board);
    score eval = std::numeric_limits<score>::min();
    for(auto mv : mvs)
    {
        auto copy_board = board;
        copy_board.make(mv);

        eval = -negamax<EvalFunc>(copy_board, -beta, -alpha, depth - 1);
        if(eval >= beta)
        {
            return beta;
        }

        if(eval > alpha)
        {
            alpha = eval;
        }
    }

    return alpha;
}

template<evaluation_func EvalFunc>
chess::move best_move(chess::game_board &board, int depth)
{
    int alpha = -1e8;
    int beta = 1e8;
    chess::move best = chess::encode_move(0x0, 0x0, 0x0, 0x0, 0x0, 0x0);

    auto mvs = chess::generate_legal_moves(board);
    for(auto mv : mvs)
    {
        auto copy_board = board;
        copy_board.make(mv);

        auto eval = -negamax<EvalFunc>(copy_board, -beta, -alpha, depth - 1);

        if(eval > alpha)
        {
            alpha = eval;
            best = mv;
        }
    }

    return best;
}


}
