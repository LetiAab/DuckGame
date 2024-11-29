#ifndef LOOK_UP_COMMAND_H
#define LOOK_UP_COMMAND_H

#include "../executable.h"
#include <cstdint>

class LookUpCommand : public Executable {
private:
    uint16_t player_id;
public:
    LookUpCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif
