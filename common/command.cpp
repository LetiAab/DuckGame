#include "sendable.h"
#include "client/client_protocol.h"

#include <cstdint>
#include <string>


class Command : public Sendable {
private:
    uint16_t player_id;
    uint8_t type;


public:
    Command(uint16_t player_id, uint8_t type)
        : player_id(player_id), type(type) {}

    void send_myself(ClientProtocol& protocol) override {
        //pongo esta linea para que compile
        //despues hay que cambiarla
        protocol.recive_lobby_message();
    }

};

