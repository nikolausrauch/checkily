#include <engine/iface/iface_string.h>
#include <engine/iface/cli.h>
#include <engine/iface/uci.h>

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    std::string input;
    while(std::getline(std::cin, input))
    {
        auto tokens = ckly::iface::tokenize(input);
        if(tokens.empty()) continue;

        if(tokens[0] == "uci")
        {
            ckly::iface::uci interface;
            interface.run();
            break;
        }

        if(tokens[0] == "cli")
        {
            ckly::iface::cli interface;
            interface.run();
            break;
        }
    }

    return EXIT_SUCCESS;
}
