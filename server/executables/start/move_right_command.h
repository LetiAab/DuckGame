#ifndef MOVE_RIGHT_COMMAND_H
#define MOVE_RIGHT_COMMAND_H

#include "../executable.h"
#include <cstdint>

class MoveRightCommand : public Executable {
    private:
        uint16_t player_id;
public:
    MoveRightCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif
