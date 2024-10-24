#ifndef LOBBY_MESSAGE_H
#define LOBBY_MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

struct LobbyMessage {
    uint16_t player_id;
    uint8_t type;
    uint16_t len_matches;
    std::vector<uint16_t> existing_matches;
    uint8_t current_match_id;
};


#endif
