#include <exception>
#include <iostream>
#include <sstream>

#include "server.h"


int main(int argc, char* argv[]) {
    try {
        const char* servname = nullptr;

        if (argc == 2) {
            servname = argv[1];
        } else {
            std::cerr << "Bad program call. Expected " << argv[0] << " <servname>\n";
            return 1;
        }

        Server server(servname);
        server.run();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
