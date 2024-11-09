#ifndef STOP_MOVE_UP_COMMAND_H
#define STOP_MOVE_UP_COMMAND_H

#include "../executable.h"
#include <cstdint>

class StopMoveUpCommand : public Executable {
    private:
        uint16_t player_id;
public:
    StopMoveUpCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif