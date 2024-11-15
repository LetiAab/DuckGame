#ifndef TAKE_ITEM_COMMAND_H
#define TAKE_ITEM_COMMAND_H

#include "../executable.h"
#include <cstdint>

class TakeItemCommand : public Executable {
private:
    uint16_t player_id;
public:
    TakeItemCommand(uint16_t player_id);

    void execute(Game& game) override;
};

#endif
