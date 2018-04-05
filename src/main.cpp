#include <iostream>
#include "toylet.h"

int main(int argc, char* argv[])
{
    if (argc > 2) {
        std::cout << "Usage: toylet [file]" << std::endl;
        return 1;
    }

    Toylet klox;

    if (argc == 2) {
        klox.file(argv[1]);
    } else {
        klox.prompt();
    }

    return 0;
}