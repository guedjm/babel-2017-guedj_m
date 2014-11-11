#include "Call.h"
#include <stdexcept>
#include <iostream>

Call::Call(QObject *parent) :
    QObject(parent)
{
    this->_active = false;
    this->_host = false;

    connect(&this->_tcpServer, SIGNAL(newConnection()), this, SLOT(onNewTcpConnection()));
    connect(&this->_tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onAcceptError(QAbstractSocket::SocketError)));
}

Call::~Call()
{
}

unsigned short      Call::getTcpServerPort() const
{
    return (this->_tcpServerPort);
}

unsigned short      Call::getUdpServerPort() const
{
    return (this->_udpServerPort);
}

unsigned short      Call::getRemoteTCPServerPort() const
{
    return (this->_remoteTCPServerPort);
}

unsigned short      Call::getRemoteUDPServerPort() const
{
    return (this->_remoteUDPServerPort);
}

QTcpServer          &Call::getTcpServer()
{
    return (this->_tcpServer);
}

QUdpSocket          &Call::getUdpServer()
{
    return (this->_udpServer);
}

QTcpSocket          &Call::getTcpSocket()
{
    return (this->_tcpSocket);
}

QUdpSocket          &Call::getUdpSocket()
{
    return (this->_udpSocket);
}

QMap<int, Callee> const &Call::getCallees() const
{
    return (this->_connectedUsers);
}

void                Call::onNewTcpConnection()
{
    bool            ok = false;
    QTcpSocket      *sock = this->_tcpServer.nextPendingConnection();
    if (sock != 0)
    {
        QString ip = sock->peerAddress().toString();
        QList<Callee>::iterator it = this->_allowedUser.begin();
        for (; it != this->_allowedUser.end(); ++it)
        {
            if (it->getIp() == ip)
            {
                ok = true;
                it->setSock(sock);
                connect(sock, SIGNAL(readyRead()), this, SLOT(onReceiveTCPPacket()));
                connect(sock, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
                connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
                this->_connectedUsers[this->_connectedUsers.count()] = Callee("test", "lol");
                //this->broadcastTCPPacket();
                this->_allowedUser.erase(it);
                break;
            }
        }
        if (!ok)
            sock->close();
    }
}

void                Call::onAcceptError(QAbstractSocket::SocketError)
{
    throw std::runtime_error("Accecpt Error");
}

void                Call::onReceiveTCPPacket()
{
}

void                Call::onClientDisconnected()
{
}

void                Call::onSocketError(QAbstractSocket::SocketError)
{
}


bool                Call::isActive() const
{
    return (this->_active);
}

bool                Call::isHost() const
{
    return (this->_host);
}

void                Call::host(const QString &ip, const QString &username)
{
    if (!this->_active)
        this->startHosting();
    this->_allowedUser.push_back(Callee(ip, username));
}

void                Call::joinCall(const QString &ip, unsigned short port)
{
    this->_active = true;
    this->_host = false;
    this->_remoteServerIp = ip;
    this->_tcpServerPort = port;
    this->_tcpSocket.connectToHost(ip, port);
}

void                Call::startHosting()
{
    if (!this->_tcpServer.listen())
        throw std::runtime_error("Cannot bind server");
    this->_tcpServerPort = this->_tcpServer.serverPort();
    std::cout << "Call : TcpServer started on port " << this->_tcpServerPort << std::endl;

    //UDP
    this->_active = true;
    this->_host = true;
}
