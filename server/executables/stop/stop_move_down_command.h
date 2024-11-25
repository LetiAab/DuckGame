#ifndef STOP_MOVE_DOWN_COMMAND_H
#define STOP_MOVE_DOWN_COMMAND_H
#include "../executable.h"
#include <cstdint>
class StopMoveDownCommand : public Executable {
private:
    uint16_t player_id;
public:
    StopMoveDownCommand(uint16_t player_id);
    void execute(Game& game) override;
};
#endif //STOP_MOVE_DOWN_COMMAND_H