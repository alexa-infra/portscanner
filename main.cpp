#include <iostream>

#include "app.h"

int main(int argc, char* argv[]) {
    {
        if (argc != 2) {
            std::cout << "Use format: " <<
                "pscan <Filename>" << std::endl <<
                "where Filename in given format" << std::endl;
            return 0;
        }
        ext::Application app;
        app.run(argv[1]);
    }
    return 0;
}
