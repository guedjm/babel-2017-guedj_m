#pragma once
#include <QString>

class       ICall
{
public:
    virtual ~ICall() {}

    virtual void mute() = 0;
    virtual void silence() = 0;
    virtual bool isHost() const = 0;

    virtual QString const   &getUsernameById(int) const = 0;
    virtual void            sendInput() = 0;
};
