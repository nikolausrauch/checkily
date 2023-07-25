#pragma once

#include <chess/board.h>

namespace ckly::iface
{

class cli
{
public:
    void run();

private:
    chess::game_board m_board;
};

}
