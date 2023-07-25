#include "uci.h"

#include "engine/iface/iface_string.h"

#include "engine/eval/table_codemonkeyking.h"
#include "engine/eval/table_michniewski.h"
#include "engine/eval/eval_table.h"
#include "engine/search/negamax.h"

#include <chess/chess_format.h>
#include <chess/move/movegen.h>

#include <algorithm>
#include <cassert>
#include <iostream>

namespace ckly::iface
{

void uci::run()
{
    std::cout << "id name checkily\n";
    std::cout << "id author Nikolaus Rauch\n";
    std::cout << "uciok" << std::endl;

    std::string input;
    while(std::getline(std::cin, input))
    {
        auto tokens = tokenize(input);
        if(tokens.empty()) continue;

        if(tokens[0] == "quit")
        {
            break;
        }

        if(tokens[0] == "debug")
        {
            m_debug = tokens[1] == "on" ? true : false;
        }

        if(tokens[0] == "isready")
        {
            std::cout << "readyok" << std::endl;

            //            this is used to synchronize the engine with the GUI. When the GUI has sent a command or
            //            multiple commands that can take some time to complete,
            //            this command can be used to wait for the engine to be ready again or
            //            to ping the engine to find out if it is still alive.
            //            E.g. this should be sent after setting the path to the tablebases as this can take some time.
            //            This command is also required once before the engine is asked to do any search
            //            to wait for the engine to finish initializing.
            //            This command must always be answered with "readyok" and can be sent also when the engine is calculating
            //            in which case the engine should also immediately answer with "readyok" without stopping the search.
        }

        if(tokens[0] == "setoption")
        {
            //            this is sent to the engine when the user wants to change the internal parameters
            //            of the engine. For the "button" type no value is needed.
            //            One string will be sent for each parameter and this will only be sent when the engine is waiting.
            //            The name and value of the option in <id> should not be case sensitive and can inlude spaces.
            //            The substrings "value" and "name" should be avoided in <id> and <x> to allow unambiguous parsing,
            //            for example do not use <name> = "draw value".
            //            Here are some strings for the example below:
            //               "setoption name Nullmove value true\n"
            //              "setoption name Selectivity value 3\n"
            //               "setoption name Style value Risky\n"
            //               "setoption name Clear Hash\n"
            //               "setoption name NalimovPath value c:\chess\tb\4;c:\chess\tb\5\n"
        }

        if(tokens[0] == "ucinewgame")
        {
            chess::load_fen(m_board, chess::fen_startposition);

            //            this is sent to the engine when the next search (started with "position" and "go") will be from
            //            a different game. This can be a new game the engine should play or a new game it should analyse but
            //            also the next position from a testsuite with positions only.
            //            If the GUI hasn't sent a "ucinewgame" before the first "position" command, the engine shouldn't
            //            expect any further ucinewgame commands as the GUI is probably not supporting the ucinewgame command.
            //            So the engine should not rely on this command even though all new GUIs should support it.
            //            As the engine's reaction to "ucinewgame" can take some time the GUI should always send "isready"
            //            after "ucinewgame" to wait for the engine to finish its operation.
        }

        if(tokens[0] == "position")
        {
            if(tokens[1] == "startpos")
            {
                chess::load_fen(m_board, chess::fen_startposition);
            }
            else if(tokens[1] == "fen")
            {
                std::vector<std::string> fen_tokens;
                std::copy_if(tokens.begin() + 2, tokens.end(), std::back_inserter(fen_tokens),
                             [](const std::string& token) { return token != "moves"; });

                chess::load_fen(m_board, fen_tokens);
            }
            else
            {
                std::cerr << "Illegal command!" << std::endl;
            }

            auto index_moves = std::distance(tokens.begin(), std::find(tokens.begin(), tokens.end(), "moves"));
            for(unsigned int i = index_moves + 1; i < tokens.size(); i++)
            {
                auto mv = parse_move(tokens[i], m_board);
                if(mv != chess::illegal_move)
                {
                    m_board.make(mv);
                }
            }
        }

        if(tokens[0] == "go")
        {

            auto mv = ckly::best_move< evaluate<table_names::michniewski> >(m_board, 6);
            std::cout << parse_move(mv) << std::endl;

            //            go
            //                start calculating on the current position set up with the "position" command.
            //                There are a number of commands that can follow this command, all will be sent in the same string.
            //                If one command is not sent its value should be interpreted as it would not influence the search.
            //                * searchmoves <move1> .... <movei>
            //                    restrict search to this moves only
            //                    Example: After "position startpos" and "go infinite searchmoves e2e4 d2d4"
            //                    the engine should only search the two moves e2e4 and d2d4 in the initial position.
            //                * ponder
            //                    start searching in pondering mode.
            //                    Do not exit the search in ponder mode, even if it's mate!
            //                    This means that the last move sent in in the position string is the ponder move.
            //                    The engine can do what it wants to do, but after a "ponderhit" command
            //                    it should execute the suggested move to ponder on. This means that the ponder move sent by
            //                    the GUI can be interpreted as a recommendation about which move to ponder. However, if the
            //                    engine decides to ponder on a different move, it should not display any mainlines as they are
            //                    likely to be misinterpreted by the GUI because the GUI expects the engine to ponder
            //                   on the suggested move.
            //                * wtime <x>
            //                    white has x msec left on the clock
            //                * btime <x>
            //                    black has x msec left on the clock
            //                * winc <x>

            //                    white increment per move in mseconds if x > 0
            //                * binc <x>
            //                    black increment per move in mseconds if x > 0
            //                * movestogo <x>
            //                  there are x moves to the next time control,
            //                    this will only be sent if x > 0,
            //                    if you don't get this and get the wtime and btime it's sudden death
            //                * depth <x>
            //                    search x plies only.
            //                * nodes <x>
            //                   search x nodes only,
            //                * mate <x>
            //                    search for a mate in x moves
            //                * movetime <x>
            //                    search exactly x mseconds
            //                * infinite
            //                    search until the "stop" command. Do not exit the search without being told so in this mode!
        }
        if(tokens[0] == "stop")
        {
            //                stop calculating as soon as possible,
            //                don't forget the "bestmove" and possibly the "ponder" token when finishing the search
        }

        if(tokens[0] == "ponderhit")
        {
            //                the user has played the expected move. This will be sent if the engine was told to ponder on the same move
            //                the user has played. The engine should continue searching but switch from pondering to normal search.
        }
    }
}

}
