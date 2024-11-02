#ifndef MOVE_DOWN_COMMAND_H
#define MOVE_DOWN_COMMAND_H

#include "executable.h"
#include <cstdint>

class MoveDownCommand : public Executable {
    private:
        uint16_t player_id;
public:
    MoveDownCommand(uint16_t player_id);

    void execute() override;
};

#endif
