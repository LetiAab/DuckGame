#ifndef DROP_WEAPON_COMMAND_H
#define DROP_WEAPON_COMMAND_H

#include "../executable.h"
#include <cstdint>

class DropWeaponCommand : public Executable {
private:
    uint16_t player_id;  

public:
    DropWeaponCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif // DROP_WEAPON_COMMAND_H
