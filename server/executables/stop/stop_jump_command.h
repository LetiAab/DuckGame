#ifndef STOP_JUMP_COMMAND_H
#define STOP_JUMP_COMMAND_H

#include "../executable.h"
#include <cstdint>

class StopJumpCommand : public Executable {
    private:
        uint16_t player_id;
public:
    StopJumpCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif