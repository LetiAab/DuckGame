#ifndef LEAVE_GUN_COMMAND_H
#define LEAVE_GUN_COMMAND_H

#include "../executable.h"
#include <cstdint>

class LeaveGunCommand : public Executable {
    private:
        uint16_t player_id;
public:
    LeaveGunCommand(uint16_t player_id);

    //void execute(Game& game) override;
};

#endif
