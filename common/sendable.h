// Interfaz Sendable
#include "client/client_protocol.h"

#ifndef SENDABLE_H
#define SENDABLE_H

class Sendable {
public:
    virtual ~Sendable() = default;
    virtual void send_myself(ClientProtocol& protocol) = 0;
};

#endif
