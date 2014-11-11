#include "Callee.h"

Callee::Callee()
{
    this->_ip = "";
    this->_username = "";
    this->_sock = 0;
}

Callee::Callee(Callee const &cpy)
{
    this->_ip = cpy._ip;
    this->_username = cpy._username;
    this->_sock = cpy._sock;
}

Callee::Callee(QString const &ip, QString const &username)
{
    this->_ip = ip;
    this->_username = username;
    this->_sock = 0;
}

Callee::~Callee()
{
}

QString const        &Callee::getIp() const
{
    return (this->_ip);
}

QString const        &Callee::getUsername() const
{
    return (this->_username);
}

QTcpSocket           *Callee::getSock()
{
    return (this->_sock);
}

void                  Callee::setSock(QTcpSocket *sock)
{
    this->_sock = sock;
}

