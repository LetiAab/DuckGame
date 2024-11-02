#ifndef LOBBY_PLAYER_H
#define LOBBY_PLAYER_H
#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/lobby_message.h"
#include "common/lobby_command.h"
#include "common/command.h"
#include "../executables/executable.h"

#include "../server_protocol.h"
#include "lobby_receiver.h"
#include "../player.h"


class LobbyPlayer {
    
private:
    ServerProtocol protocol;
    const uint16_t player_id;
    LobbyReceiver lobby_receiver;
    bool connected;
    bool playing;
    uint16_t match_id;


public:
    LobbyPlayer(Socket&& socket, const uint16_t id, Queue<LobbyCommand>& lobby_queue);

    void start();

    bool send_lobby_message(const LobbyMessage& msj);
    
    std::shared_ptr<Player> start_game(Queue<std::shared_ptr<Executable>>& game_queue);

    bool is_connected();

    uint16_t get_player_id();

    void set_match_id(uint16_t id);

    uint16_t get_match_id();

    void stop();

    //Queue<Message>& get_message_queue();
};

#endif
