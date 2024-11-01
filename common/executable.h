// Interfaz Executable
#ifndef EXECUTABLE_H
#define EXECUTABLE_H

class Executable {
public:
    virtual ~Executable() = default;
    //seguramente reciba un game y un vector de Messages
    virtual void execute() = 0;
};

#endif
