
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <iostream>


#include "common/client_protocol.h"
#include "common/socket.h"
#include "common/message.h"
#include "common/command.h"
#include "common/constants.h"

#include "client_sender.h"
#include "client_receiver.h"
#include "client_inputhandler.h"

class Client {
private:
    ClientProtocol protocol;
    std::unique_ptr<ClientSender> sender;
    std::unique_ptr<ClientReceiver> receiver;
    std::unique_ptr<InputHandler> input_handler;


public:
    Client(const std::string& hostname, const std::string& port);
    int start();

};

#endif
