#include "acceptor.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "common/socket.h"


Acceptor::Acceptor(Socket& skt):
        skt_listener(skt), is_alive(true), lobby(), ids_counter(1){}

void Acceptor::run() {
    try {
        lobby.start();

        while (is_alive) {
            Socket peer = skt_listener.accept();

            std::cout << "Acceptor: se conecto un nuevo cliente"  << std::endl;

            std::shared_ptr<LobbyPlayer> new_player = std::make_unique<LobbyPlayer>(std::move(peer), ids_counter, lobby.get_lobby_queue());
            new_player->start();

            lobby.add_player(new_player);
            ids_counter = ids_counter + 1;
        }

    } catch (const std::exception& e) {
        is_alive = false;
    }
}

void Acceptor::stop() {
    is_alive = false;
    std::cout << "Acceptor: deteniendo el lobby..."  << std::endl;
    lobby.stop();
    std::cout << "Acceptor: joineando el lobby..."  << std::endl;
    lobby.join();
    std::cout << "Acceptor: todo listo"  << std::endl;
}
