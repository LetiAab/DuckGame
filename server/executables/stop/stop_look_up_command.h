#ifndef STOP_LOOK_UP_COMMAND_H
#define STOP_LOOK_UP_COMMAND_H

#include "../executable.h"
#include <cstdint>

class StopLookUpCommand : public Executable {
private:
    uint16_t player_id;
public:
    StopLookUpCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif
