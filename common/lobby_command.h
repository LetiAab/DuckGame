#include "sendable.h"
#include "client_protocol.h"

#ifndef LOBBY_COMMAND_H
#define LOBBY_COMMAND_H

#include <cstdint>
#include <string>


class LobbyCommand : public Sendable {
private:
    uint16_t player_id;
    uint8_t type;
    uint16_t match_id;

    friend class ClientProtocol;
    friend class Protocol;
    
    //refactor: estas dos no deberian ser amigas
    friend class Lobby;
    friend class LobbyReceiver;


public:
    LobbyCommand(uint16_t player_id, uint8_t type, uint16_t match_id);

    void send_myself(ClientProtocol& protocol) override;

};

#endif
