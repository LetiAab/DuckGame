#ifndef STOP_MOVE_LEFT_COMMAND_H
#define STOP_MOVE_LEFT_COMMAND_H

#include "../executable.h"
#include <cstdint>

class StopMoveLeftCommand : public Executable {
    private:
        uint16_t player_id;
public:
    StopMoveLeftCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif
