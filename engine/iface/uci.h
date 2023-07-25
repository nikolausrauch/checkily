#pragma once

#include <chess/board.h>

namespace ckly::iface
{

class uci
{
public:
    void run();

private:
    chess::game_board m_board;
    bool m_debug{false};
};

}
