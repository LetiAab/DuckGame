#ifndef STOP_MOVE_RIGHT_COMMAND_H
#define STOP_MOVE_RIGHT_COMMAND_H

#include "../executable.h"
#include <cstdint>

class StopMoveRightCommand : public Executable {
    private:
        uint16_t player_id;
public:
    StopMoveRightCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif
