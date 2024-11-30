#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H

#include "../executable.h"
#include <cstdint>

class ShootCommand : public Executable {
    private:
        uint16_t player_id;
public:
    ShootCommand(uint16_t player_id);

    void execute(Game& game) override;

};

#endif
