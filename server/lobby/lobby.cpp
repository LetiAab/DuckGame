#include "lobby.h"
#include "../match.h"

#include "common/lobby_message.h"
#include "common/constants.h"
#include "../executables/executable.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <memory>
#include <algorithm>


Lobby::Lobby(): is_alive(true), lobby_players(), matches(), lobby_queue(), match_counter_ids(1){}

void Lobby::run() {
    try {
        while (is_alive) {
            LobbyCommand cmd = lobby_queue.pop(); //bloqueante
            std::cout << "Lobby: saque un comando de mi queue"  << std::endl;
            if(cmd.type == START_MATCH_CODE) {
                process_start_match_command(cmd);

            } else {
                LobbyMessage msg = process_command(cmd);
                send_message(msg);
            }

            clean_disconnected_players();
            clean_finished_matches();
        }

    } catch (const ClosedQueue& e) {
        std::cout << "Se cerró la queue del lobby\n";
    }
}

void Lobby::clean_finished_matches() {

    for (const auto& match : matches) {
        if (match->is_over()) {
            std::cout << "Lobby: detecto que termino el partido " << match->get_match_id() <<". Voy a eliminarlo"  << std::endl;
            match->stop_match();
        }
    }

    auto it = std::remove_if(matches.begin(), matches.end(),
                             [](const std::unique_ptr<Match>& match) {
                                return match->is_over();
                             });

    matches.erase(it, matches.end());
    //std::cout << "Lobby: borre el partido "  << std::endl;
}

void Lobby::clean_disconnected_players() {
    for (const auto& player : lobby_players) {
        if (player->is_playing()) {
            std::cout << "Lobby: el jugador " << player->get_player_id() <<" esta en partida, voy a eliminarlo"  << std::endl;
        } else if (!player->is_connected()) {
            std::cout << "Lobby: el jugador " << player->get_player_id() <<" se desconecto, voy a  eliminarlo"  << std::endl;
            //si el jugador que se desconecto estaba conectado a una partida sin empezar, lo elimino de la partida
            uint16_t match_id = player->get_match_id();
            if(match_id != 0){
                Match* match = find_match_by_id(match_id);
                match->remove_player();
            }
            
            player->stop();
        }
    }

    lobby_players.remove_if([](const std::shared_ptr<LobbyPlayer>& player) {
        return (!player->is_connected() || player->is_playing());
    });
}

void Lobby::clean_all_players() {
    for (const auto& player : lobby_players) {
        LobbyMessage msg;
        msg.type = EXIT_GAME;
        player->send_lobby_message(msg);
        player->stop();
    }

    lobby_players.clear();
}

void Lobby::clean_all_matches() {
    for (const auto& match : matches) {
        match->stop_match();
    }

    matches.clear();
}


void Lobby::add_player(std::shared_ptr<LobbyPlayer> player) {
    std::cout << "Lobby: recibi un nuevo jugador"  << std::endl;
    LobbyMessage msg = create_lobby_message(player->get_player_id());

    player->send_lobby_message(msg);
    lobby_players.push_back(player);
}

void Lobby::stop() {
    is_alive = false;
    clean_all_players();
    std::cout << "Lobby: elimine todos los jugadores"  << std::endl;
    clean_all_matches();
    std::cout << "Lobby: elimine todos los partidos"  << std::endl;
    lobby_queue.close();
    std::cout << "Lobby: LISTO"  << std::endl;
}

Queue<LobbyCommand>& Lobby::get_lobby_queue() {
    return lobby_queue;
}

void Lobby::send_message(const LobbyMessage& msg) {
    std::shared_ptr<LobbyPlayer> player = find_player_by_id(msg.player_id);
    if (player) {
        player->send_lobby_message(msg);
    }
}

std::shared_ptr<LobbyPlayer> Lobby::find_player_by_id(const uint16_t id) {
    for (auto& player : lobby_players) {
        if (player->get_player_id() == id) {
            return player;
        }
    }
    return nullptr;
}

Match* Lobby::find_match_by_id(const uint16_t id) {
    for (auto& match : matches) {
        if (match->get_match_id() == id) {
            return match.get();
        }
    }
    return nullptr;
}

void Lobby::get_all_match_ids(std::vector<uint16_t>& match_ids) {
    for (auto& match : matches) {
        if (match->is_match_available()) {
            match_ids.push_back(match->get_match_id());
            //std::cout << match->get_match_id() << std::endl;
        }
    }
}

LobbyMessage Lobby::create_lobby_message(uint16_t player_id) {
    LobbyMessage msg;
    msg.player_id = player_id;
    msg.type = FIRST_LOBBY_MESSAGE;
    get_all_match_ids(msg.existing_matches);
    msg.len_matches = msg.existing_matches.size();
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
        get_all_match_ids(msg.existing_matches);
        msg.len_matches = msg.existing_matches.size();
        msg.current_match_id = 0;
    } else if (type == LIST_MATCH_AVAILABLE) {
        get_all_match_ids(msg.existing_matches);
        msg.len_matches = msg.existing_matches.size();
    }

    return msg;
}

void Lobby::notify_players(Match* match, LobbyMessage msg){
    uint16_t match_id = match->get_match_id();
    Queue<std::shared_ptr<Executable>>& queue = match->get_game_queue();

    for (auto& lobby_player : lobby_players) {

        if (lobby_player->get_match_id() == match_id) {
            //aviso a los jugadores que la partida va a empezar para que se detenga el hilo recibidor
            if(lobby_player->send_lobby_message(msg)){
                std::shared_ptr<Player> player = lobby_player->start_game(queue);
                match->add_player(player);
            }

        }
    }
    
}

void Lobby::process_start_match_command(const LobbyCommand& cmd) {


    std::shared_ptr<LobbyPlayer> player = find_player_by_id(cmd.player_id);
    Match* match = find_match_by_id(cmd.match_id);

    if (match and match->is_able_to_start()) {

        LobbyMessage msg = create_lobby_response(0, START_MATCH_CODE, cmd.match_id);

        notify_players(match, msg);
        match->start_match();

        std::cout << "La partida se inició\n";

    } else {
        LobbyMessage msg = create_lobby_response(cmd.player_id, LOBBY_COMMAND_FAIL, cmd.match_id);
        send_message(msg);
        std::cout << "No se puede iniciar esta partida aún\n";

    }
}


LobbyMessage Lobby::process_command(const LobbyCommand& cmd) {
    std::shared_ptr<LobbyPlayer> lobby_player = find_player_by_id(cmd.player_id);
    uint8_t type = cmd.type;
    uint16_t match_id = 0;

    switch (type) {
        case NEW_MATCH_CODE: {
            std::unique_ptr<Match> new_match = std::make_unique<Match>(match_counter_ids);
            //new_match->can_add_player();

            match_id = new_match->get_match_id();
            //lobby_player->set_match_id(match_id);

            matches.push_back(std::move(new_match));
            match_counter_ids += 1;

            std::cout << "El jugador creó una nueva partida\n";
            break;
        }
        case EXISTING_MATCH_CODE: {
            Match* match = find_match_by_id(cmd.match_id);
            uint16_t old_match = lobby_player->get_match_id();
            //verifico que se pueda conectar a dicha partida y que no haya estado conectado a otra antes
            if (match->can_add_player() && (old_match == 0)) {

                match_id = cmd.match_id;
                lobby_player->set_match_id(match_id);

                std::cout << "El jugador se conectó a la partida existente\n";
            } else {
                std::cout << "El jugador NO pudo conectarse a la partida\n";
                type = LOBBY_COMMAND_FAIL;
            }
            break;
        }
        case LIST_MATCH_AVAILABLE: {
            std::cout << "El jugador quiere ver las partidas disponibles\n";
            break;
        }
    }
    
    return create_lobby_response(cmd.player_id, type, match_id);
}


