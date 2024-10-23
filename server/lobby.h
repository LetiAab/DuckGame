#ifndef LOBBY_H
#define LOBBY_H
#include <exception>
#include <iostream>
#include <list>
#include <sstream>

#include "common/queue.h"
#include "common/lobby_command.h"
#include "match.h"
#include "player.h"
#include "common/thread.h"


class Lobby: public Thread {
private:
    bool is_alive;
    std::list<Player*> players;
    std::list<Match*> matches;
    Queue<LobbyCommand> lobby_queue;
    int match_counter_ids;

    void send_first_message(Player* player);
    void process_command(const LobbyCommand& cmd);
    Player* find_player_by_id(const int id);
    Match* find_match_by_id(const int id);

public:
    explicit Lobby();

    void run() override;

    void add_player(Player* player);

    Queue<LobbyCommand>& get_lobby_queue(); 

    void stop() override;

    Lobby(const Lobby&) = delete;
    Lobby& operator=(const Lobby&) = delete;

    Lobby(Lobby&&) = default;
    Lobby& operator=(Lobby&&) = default;
};

#endif
