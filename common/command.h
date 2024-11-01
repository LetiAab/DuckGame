#include "sendable.h"
#include "client/client_protocol.h"

#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>
#include <string>

class Command : public Sendable {
private:
    uint16_t player_id;
    uint8_t type;


public:
    Command(uint16_t player_id, uint8_t type);

    void send_myself(ClientProtocol& protocol) override;

};

#endif
