#ifndef MOVE_LEFT_COMMAND_H
#define MOVE_LEFT_COMMAND_H

#include "executable.h"
#include <cstdint>

class MoveLeftCommand : public Executable {
    private:
        uint16_t player_id;
public:
    MoveLeftCommand(uint16_t player_id);

    void execute() override;
};

#endif
