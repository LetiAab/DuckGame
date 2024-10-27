#ifndef LOBBY_H
#define LOBBY_H
#include <exception>
#include <iostream>
#include <list>
#include <sstream>

#include "common/queue.h"
#include "common/lobby_command.h"
#include "common/lobby_message.h"
#include "match.h"
#include "lobby_player.h"
#include "common/thread.h"


class Lobby : public Thread {
private:
    bool is_alive;
    std::list<std::shared_ptr<LobbyPlayer>> lobby_players;
    std::list<std::shared_ptr<Match>> matches;
    Queue<LobbyCommand> lobby_queue;
    uint16_t match_counter_ids;

    LobbyMessage process_command(const LobbyCommand& cmd);
    void process_start_match_command(const LobbyCommand& cmd);
    void send_message(const LobbyMessage& msg);
    std::shared_ptr<LobbyPlayer> find_player_by_id(const uint16_t id);
    std::shared_ptr<Match> find_match_by_id(const uint16_t id);
    void get_all_match_ids(std::vector<uint16_t>& match_ids);
    LobbyMessage create_lobby_message(uint16_t player_id);
    LobbyMessage create_lobby_response(uint16_t player_id, uint8_t type, uint16_t current_match);
    void notify_players(std::shared_ptr<Match> match, LobbyMessage msg);

public:
    explicit Lobby();

    void run() override;

    void add_player(std::shared_ptr<LobbyPlayer> player);
    Queue<LobbyCommand>& get_lobby_queue();

    void stop() override;

    Lobby(const Lobby&) = delete;
    Lobby& operator=(const Lobby&) = delete;

    Lobby(Lobby&&) = default;
    Lobby& operator=(Lobby&&) = default;
};


#endif
