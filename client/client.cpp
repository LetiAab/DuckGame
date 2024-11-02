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

void print_first_message(Message& first_message){
    std::cout << "First Message. My Player ID is: " << first_message.player_id << "\n";
    std::cout << "---------------------------------------------" << "\n";
    std::cout << "OPTIONS: " << "\n";
    std::cout << "Create a new match: 5" << "\n";
    std::cout << "Join an existing match: 6" << "\n";
    //por ahora muestro los match ids, esto despues se va a mostrar solo si el jugador
    //hace click en ver partidas existente y vamos a mostrar el nombre de la partida
    printExistingMatches(first_message.existing_matches);
    std::cout << "Once you are in a match, you can start it with: 7" << "\n";
    std::cout << "\n";
}

void Client::handleLobby(uint16_t& id, Queue<Message>& message_queue) {
    while(true){
        Message message = message_queue.pop();

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
    Message first_message = protocol.recive_message();

    //persisto mi id
    uint16_t id = first_message.player_id;

    print_first_message(first_message);

    //inputhandler ---> sender
    //SDL? <--- receiver
    sender = std::make_unique<ClientSender>(protocol);
    input_handler = std::make_unique<InputHandler> (id, sender->get_queue());
    receiver = std::make_unique<ClientReceiver>(protocol);
    sdl_handler = std::make_unique<SDLHandler>();

    //obtengo la queue para procesar los mensajes que me manda el server
    //probablemnete deba mandarsela a SDL
    Queue<Message>& message_queue = receiver->get_queue();

    //inicio los hilos
    sender->start();
    receiver->start();
    input_handler->start();

    //manejo la interaccion con el lobby
    handleLobby(id, message_queue);

    //aca deberia recibir un mensaje especial con el Mapa, y dibujarlo
    sdl_handler->run();

    //recibo los mensajes del juego con las actualizaciones
    while(true){
        Message message = message_queue.pop();

        //procesar el mensaje
        break;

        std::cout << "\n";
    }

    //---------------------------------------------------------------------------------------

    //protocol.close();
    input_handler->join();
    sender->join();
    receiver->join();

    return SUCCESS;
}

