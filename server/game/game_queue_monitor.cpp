#include "game_queue_monitor.h"

#include <algorithm>
#include <iostream>

#include "common/constants.h"
#include "common/message.h"

GameQueueMonitor::GameQueueMonitor() {}

bool GameQueueMonitor::add_queue(Queue<Message>* new_queue) {
    std::unique_lock<std::mutex> lck(mtx);
    queues.push_back(new_queue);
    return true;
}

bool GameQueueMonitor::send_message(const Message& message) {
    std::unique_lock<std::mutex> lck(mtx);

    for (auto& q: queues) {
        q->try_push(message);
    }

    return true;
}

void GameQueueMonitor::remove_queue(Queue<Message>* queue) {
    std::unique_lock<std::mutex> lck(mtx);

    auto it = std::find(queues.begin(), queues.end(), queue);
    if (it != queues.end()) {
        (*it)->close();
        queues.erase(it);
    }
}

void GameQueueMonitor::remove_all_queues() {
    std::unique_lock<std::mutex> lck(mtx);
    for (auto& queue: queues) {
        queue->close();
    }

    queues.clear();
}
