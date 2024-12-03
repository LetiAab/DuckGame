#include <cstdint>
#include <list>
#include <mutex>
#include <string>

#include "common/message.h"
#include "common/queue.h"

#ifndef GAME_QUEUE_MONITOR_H
#define GAME_QUEUE_MONITOR_H

class GameQueueMonitor {
private:
    std::mutex mtx;
    std::list<Queue<Message>*> queues;


public:
    GameQueueMonitor();

    bool add_queue(Queue<Message>* queue);

    bool send_message(const Message& message);

    void remove_queue(Queue<Message>* queue);

    void remove_all_queues();
    
    void broadcast(const Message& message);

};

#endif
