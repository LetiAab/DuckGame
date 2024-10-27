#ifndef LOBBY_PLAYER_H
#define LOBBY_PLAYER_H
#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/lobby_message.h"
#include "common/lobby_command.h"
#include "common/command.h"

#include "server_protocol.h"
#include "lobby_receiver.h"
#include "receiver.h"
#include "sender.h"


class LobbyPlayer {
    
private:
    ServerProtocol protocol;
    const uint16_t player_id;
    LobbyReceiver lobby_receiver;
    bool connected;
    bool playing;
    //Receiver receiver;


public:
    LobbyPlayer(Socket&& socket, const uint16_t id, Queue<LobbyCommand>& lobby_queue);

    void start();

    bool send_lobby_message(const LobbyMessage& msj); 

    //void set_game_queue(Queue<Command>& queue);
    
    void start_playing();

    bool is_connected();

    void stop_playing();

    uint16_t get_player_id();

    void stop();

    //Queue<Message>& get_message_queue();
};

#endif
