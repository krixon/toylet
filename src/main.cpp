#include <iostream>
#include "toylet.h"

int main(int argc, char* argv[])
{
    if (argc > 2) {
        std::cout << "Usage: toylet [file]" << std::endl;
        return 1;
    }

    Toylet toylet;

    if (argc == 2) {
        toylet.file(argv[1]);
    } else {
        toylet.prompt();
    }

    return 0;
}