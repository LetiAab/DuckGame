
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "client_protocol.h"
#include "client_sender.h"
#include "client_receiver.h"
#include "client_inputhandler.h"
#include "sdl_handler.h"

class SDLHandler;

class Client {
private:
    ClientProtocol protocol;
    std::unique_ptr<ClientSender> sender;
    std::unique_ptr<ClientReceiver> receiver;
    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<SDLHandler> sdl_handler;
    uint16_t lobby_id;
    uint16_t duck_id;
    
    void handleLobby(uint16_t& id, Queue<Message>& message_queue);
public:
    Client(const std::string& hostname, const std::string& port);
    int start();

};

#endif
