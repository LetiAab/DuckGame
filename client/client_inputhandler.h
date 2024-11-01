#include <cstdint>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "common/lobby_command.h"
#include "common/sendable.h"


#include "common/client_protocol.h"
#include "common/constants.h"


#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

class InputHandler: public Thread {
private:
    uint16_t id;
    bool is_alive;
    Queue<std::shared_ptr<Sendable>>& command_queue;


public:
    explicit InputHandler(uint16_t id, Queue<std::shared_ptr<Sendable>>& command_queue);

    void run() override;

    bool is_running();

    void stop() override;

};

#endif
