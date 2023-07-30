#include <cstdlib>

#include "chess_app.h"

int main(int argc, char** argv)
{
    chess_app::config config;

    chess_app app(config);
    app.run();

    return EXIT_SUCCESS;
}
