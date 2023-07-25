#include "cli.h"

#include "engine/iface/iface_string.h"

#include <chess/chess_format.h>
#include <chess/chess_string.h>
#include <chess/move/movegen.h>

// DEBUG TODO: relative paths?
#include "engine/eval/table_codemonkeyking.h"
#include "engine/eval/table_michniewski.h"
#include "engine/eval/eval_table.h"
#include "engine/search/negamax.h"

#include <cassert>
#include <iostream>

void ckly::iface::cli::run()
{
    chess::load_fen(m_board, chess::fen_startposition);

    std::cout << "---- [checkily cli] ----\n";
    std::cout << chess::to_string(m_board) << std::endl;

    std::string input;
    while(std::getline(std::cin, input))
    {
        auto tokens = tokenize(input);
        if(tokens.empty()) continue;

        if(tokens[0] == "quit")
        {
            break;
        }

        if(tokens[0] == "go")
        {
            auto mv = ckly::best_move< evaluate<table_names::michniewski> >(m_board, 6);
            std::cout << "checkily plays: " << parse_move(mv) << std::endl;

            m_board.make(mv);
            std::cout << chess::to_string(m_board) << std::endl;
        }

        if(tokens[0] == "move")
        {
            auto mv = parse_move(tokens[1], m_board);
            if(mv != chess::illegal_move)
            {
                m_board.make(mv);
                std::cout << chess::to_string(m_board) << std::endl;
            }
            else
            {
                std::cout << "Not a valid move!" << std::endl;
            }
        }
    }
}
