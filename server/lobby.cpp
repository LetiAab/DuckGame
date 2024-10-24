#include "lobby.h"
#include "match.h"

#include "common/lobby_message.h"
#include "common/constants.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>


Lobby::Lobby(): is_alive(true), players(), matches(), lobby_queue(), match_counter_ids(1){}

void Lobby::run() {
    try {
        while (is_alive) {
            LobbyCommand cmd = lobby_queue.pop(); // OJO: bloqueante, quizás tengamos que usar try_pop + sleep
            LobbyMessage msg = process_command(cmd);

            std::shared_ptr<Player> player = find_player_by_id(msg.player_id);
            if (player) {
                player->send_lobby_message(msg);
            }
        }
    } catch (const ClosedQueue& e) {
        std::cout << "Se cerró la queue del lobby\n";
    }
}

void Lobby::add_player(std::shared_ptr<Player> player) {
    LobbyMessage msg = create_lobby_message(player->get_player_id());

    player->send_lobby_message(msg);
    players.push_back(player);
}

void Lobby::stop() {
    is_alive = false;
    lobby_queue.close();
    // liberar memoria, etc
}

Queue<LobbyCommand>& Lobby::get_lobby_queue() {
    return lobby_queue;
}


std::shared_ptr<Player> Lobby::find_player_by_id(const uint16_t id) {
    for (auto& player : players) {
        if (player->get_player_id() == id) {
            return player;
        }
    }
    return nullptr;
}

std::shared_ptr<Match> Lobby::find_match_by_id(const uint16_t id) {
    for (auto& match : matches) {
        if (match->get_match_id() == id) {
            return match;
        }
    }
    return nullptr;
}

void Lobby::get_all_match_ids(std::vector<uint16_t>& match_ids) {
    for (auto& match : matches) {
        if (match->is_match_avaiable()) {
            match_ids.push_back(match->get_match_id());
        }
    }
}

LobbyMessage Lobby::create_lobby_message(uint16_t player_id) {
    LobbyMessage msg;
    msg.player_id = player_id;
    msg.type = FIRST_LOBBY_MESSAGE;
    msg.len_matches = matches.size();
    get_all_match_ids(msg.existing_matches);
    msg.current_match_id = 0;

    return msg;
}

LobbyMessage Lobby::create_lobby_response(uint16_t player_id, uint8_t type, uint16_t current_match) {
    LobbyMessage msg;
    msg.player_id = player_id;
    msg.type = type;
    msg.len_matches = 0;
    msg.current_match_id = current_match;

    if (type == LOBBY_COMMAND_FAIL) {
        msg.len_matches = matches.size();
        get_all_match_ids(msg.existing_matches);
        msg.current_match_id = 0;
    }

    return msg;
}


LobbyMessage Lobby::process_command(const LobbyCommand& cmd) {
    std::shared_ptr<Player> player = find_player_by_id(cmd.player_id);
    uint8_t type = cmd.type;
    uint16_t match_id = 0;

    switch (type) {
        case NEW_MATCH_CODE: {
            std::shared_ptr<Match> new_match = std::make_shared<Match>(match_counter_ids);
            matches.push_back(new_match);
            new_match->add_player(player);
            match_counter_ids += 1;
            match_id = new_match->get_match_id();

            std::cout << "El jugador creó una nueva partida\n";
            break;
        }
        case EXISTING_MATCH_CODE: {
            std::shared_ptr<Match> match = find_match_by_id(cmd.match_id);

            if (match->add_player(player)) {
                match_id = cmd.match_id;
                std::cout << "El jugador se conectó a la partida existente\n";
            } else {
                std::cout << "El jugador NO pudo conectarse a la partida\n";
                type = LOBBY_COMMAND_FAIL;
            }
            break;
        }
        default: {
            std::shared_ptr<Match> match = find_match_by_id(cmd.match_id);

            if (match->start_match()) {
                //OJO ACA
                //Se esta iniciando la partida Y DESPUES se le avisa SOLO al jugador que la inicio
                //ver como manejamos esto...
                match_id = match->get_match_id();
                std::cout << "La partida se inició\n";
            } else {
                std::cout << "No se puede iniciar esta partida aún\n";
                type = LOBBY_COMMAND_FAIL;
            }
            break;
        }
    }

    return create_lobby_response(cmd.player_id, type, match_id);
}


