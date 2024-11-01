// Interfaz Sendable

#ifndef SENDABLE_H
#define SENDABLE_H

class ClientProtocol;

class Sendable {
public:
    virtual ~Sendable() = default;
    virtual void send_myself(ClientProtocol& protocol) = 0;
};

#endif
