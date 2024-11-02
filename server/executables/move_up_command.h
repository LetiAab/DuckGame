#ifndef MOVE_UP_COMMAND_H
#define MOVE_UP_COMMAND_H

#include "executable.h"
#include <cstdint>

class MoveUpCommand : public Executable {
    private:
        uint16_t player_id;
public:
    MoveUpCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif
