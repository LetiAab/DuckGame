#include "server.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "../common/socket.h"
#include "acceptor.h"

Server::Server(const char* servname): servname(servname) {}

void Server::run() {
    try {
        bool running = true;
        Socket socket(servname);
        Acceptor acceptor(socket);

        acceptor.start();

        std::string command = "";
        while (running) {
            std::getline(std::cin, command);
            if (command == "q") {
                running = false;
            }
        }

        socket.shutdown(2);
        socket.close();

        acceptor.stop();
        acceptor.join();

    } catch (const std::exception& e) {
        std::cerr << "Exception in Server::run(): " << e.what() << std::endl;
    }
}
