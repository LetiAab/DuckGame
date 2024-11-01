#include "client_inputhandler.h"

#include <iostream>

#include "common/liberror.h"
#include "common/lobby_command.h"


InputHandler::InputHandler(uint16_t id, Queue<std::shared_ptr<Sendable>>& command_queue):
id(id), is_alive(true), command_queue(command_queue) {}

uint16_t getMatchId() {
    std::string match_id_str;
    std::getline(std::cin, match_id_str);

    try {
        unsigned long match_id_long = std::stoul(match_id_str);

        if (match_id_long > std::numeric_limits<uint16_t>::max()) {
            throw std::out_of_range("El número está fuera del rango de uint16_t.");
        }

        return static_cast<uint16_t>(match_id_long);
    } catch (const std::invalid_argument&) {
        std::cerr << "Entrada no válida. Por favor, ingrese un número.\n";
        return 0;
    } catch (const std::out_of_range&) {
        std::cerr << "El número está fuera del rango permitido.\n";
        return 0;
    }
}

void InputHandler::run() {

    uint16_t actual_match_id = 0;

    try {

        while (is_alive) {

            std::string input;
            std::getline(std::cin, input);

            if (input.empty() || input == "Exit") {
                break;
            }

            if (input == "5") {
                auto command = std::make_shared<LobbyCommand>(id, NEW_MATCH_CODE, actual_match_id);

                if (command_queue.try_push(command)){
                    std::cout << "Creando partida..." << "\n";
                }
            }

            if (input == "6"){
                //este es para conectarte
                actual_match_id = getMatchId();//esto bloquea hasta que le pases un match id

                auto command = std::make_shared<LobbyCommand>(id, EXISTING_MATCH_CODE, actual_match_id);
                
                if (command_queue.try_push(command)){
                    std::cout << "Conectando a partida..." << "\n";
                };
            }

            if (input == "7"){
                //iniciar partida a la que estoy conectado
                auto command = std::make_shared<LobbyCommand>(id, START_MATCH_CODE, actual_match_id);

                if (command_queue.try_push(command)){
                    std::cout << "Iniciando partida..." << "\n";
                };
            }

            if (input == "d"){
                //crear comando de mover a la derecha y agregarlo a la queue
            }

        }

    } catch (const ClosedQueue& e) {
        is_alive = false;

    } catch (const LibError& e) {
        is_alive = false;

    } catch (const std::exception& e) {
        is_alive = false;
        std::cerr << "Exeption running the sender: " << e.what() << std::endl;
    }
}

bool InputHandler::is_running() { return is_alive; }

void InputHandler::stop() { is_alive = false; }
