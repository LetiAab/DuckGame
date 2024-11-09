#ifndef STOP_MOVE_COMMAND_H
#define STOP_MOVE_COMMAND_H

#include "../executable.h"
#include <cstdint>

class StopMoveCommand : public Executable {
    private:
        uint16_t player_id;
public:
    StopMoveCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif