#include "client.h"

#include "common/command.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/constants.h"

Client::Client(const std::string& hostname, const std::string& port):
        protocol(Socket(hostname.c_str(), port.c_str())){}

void printExistingMatches(const std::vector<uint16_t>& existing_matches) {
    std::cout << "=> Ids de partidas disponibles: ";
    if (existing_matches.empty()) {
        std::cout << "No existing matches.";
    } else {
        for (size_t i = 0; i < existing_matches.size(); ++i) {
            std::cout << existing_matches[i];
            if (i < existing_matches.size() - 1) {
                std::cout << ", ";
            }
        }
    }
    std::cout << "\n";
}

void printMap(const std::vector<std::vector<char>>& map) {
    for (const auto& row : map) {
        for (char cell : row) {
            std::cout << cell << " "; // Imprime cada celda
        }
        std::cout << std::endl; // Nueva línea después de cada fila
    }
}

void print_first_message(Message& first_message){
    std::cout << "First Message. My Player ID is: " << first_message.player_id << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "OPTIONS: " << std::endl;
    std::cout << "Create a new match: 5" << std::endl;
    std::cout << "Join an existing match: 6" << std::endl;
    //por ahora muestro los match ids, esto despues se va a mostrar solo si el jugador
    //hace click en ver partidas existente y vamos a mostrar el nombre de la partida
    printExistingMatches(first_message.existing_matches);
    std::cout << "Once you are in a match, you can start it with: 7" << std::endl;
    std::cout << "If you want to close the client, you can do it with 'Exit'" << std::endl;
    std::cout << std::endl;
}

void Client::handleLobby(uint16_t& id, Queue<Message>& message_queue) {
    while(true){
        Message message = message_queue.pop();

        if (message.type == LOBBY_EXIT_CODE){
            std::cout << "Comando para salir..." << "\n";
            break;
        }

        if (message.type == NEW_MATCH_CODE){
            std::cout << "Partida creada con id: " << static_cast<int>(message.current_match_id) << "\n";
            printExistingMatches(message.existing_matches);
        }

        if (message.type == EXISTING_MATCH_CODE){
            std::cout << "Conectado a partida con id: " << static_cast<int>(message.current_match_id) << "\n";
        }

        if (message.type == LOBBY_COMMAND_FAIL){
            std::cout << "Ups! parece que no puedes realizar esa accion" << "\n";
        }

        if (message.type == START_MATCH_CODE){
            std::cout << "Partida iniciada con id: " << static_cast<int>(message.current_match_id) << "\n";

            //esto no es muy lindo pero de momento funciona
            auto cmd = Command(id, LOBBY_STOP_CODE);

            if (protocol.send_command(cmd)){
                std::cout << "Me desconecte del lobby. Ahora voy a comunicarme con el juego" << "\n";
                break;
            };
        }
        std::cout << "\n";
    }
}

int Client::start(){
    // primer mensaje de la conexion para saber mi id
    std::cout << "Estoy por recibir el primer mensaje" << "\n";
    Message first_message = protocol.receive_message();

    //persisto mi id
    uint16_t id = first_message.player_id;
    print_first_message(first_message);

    //inputhandler ---> sender
    //SDL? <--- receiver
    sender = std::make_unique<ClientSender>(protocol);
    input_handler = std::make_unique<InputHandler> (id, sender->get_queue());
    receiver = std::make_unique<ClientReceiver>(protocol);

    //obtengo la queue para procesar los mensajes que me manda el server
    //probablemente deba mandarsela a SDL
    Queue<Message>& message_queue = receiver->get_queue();

    //inicio los hilos
    sender->start();
    receiver->start();
    input_handler->start();

    //manejo la interaccion con el lobby
    handleLobby(id, message_queue);

    //aca deberia recibir un mensaje especial con el Mapa, y dibujarlo
    std::cout << "Inicializacion del mapa" << std::endl;
    Message message = message_queue.pop();
    printMap(message.map);

    sdl_handler = std::make_unique<SDLHandler>();
    sdl_handler->run(message.map, sender->get_queue(), id, message_queue);

    //---------------------------------------------------------
    //hacer un cierre mas prolijo

    std::cout << "Cerrando socket..." << std::endl;
    protocol.shutdown();
    std::cout << "Cerrando input handler..." << std::endl;
    input_handler->stop();
    input_handler->join();
    std::cout << "Cerrando sender..." << std::endl;
    sender->stop();
    sender->join();
    std::cout << "Cerrando receiver..." << std::endl;
    receiver->stop();
    receiver->join();
    std::cout << "Cliente cerrado..." << std::endl;

    return SUCCESS;
}

