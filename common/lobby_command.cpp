#include "lobby_command.h"

#include <cstdint>
#include <string>


LobbyCommand::LobbyCommand(uint16_t player_id, uint8_t type, uint16_t match_id)
    : player_id(player_id), type(type), match_id(match_id) { }


void LobbyCommand::send_myself(ClientProtocol& protocol){
    protocol.send_lobby_command(*this);
}


