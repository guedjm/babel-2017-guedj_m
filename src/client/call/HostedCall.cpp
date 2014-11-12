#include "HostedCall.h"
#include "Serializer.hpp"
#include "PacketHelper.h"
#include "TCPPacketHeader.h"
#include "ClientWorker.h"

HostedCall::HostedCall(ClientWorker &worker, QObject *parent) :
    QObject(parent),
    _worker(worker)
{
    connect(&this->_tcpServer, SIGNAL(newConnection()), this, SLOT(receiveNewTcpConnection()));
    connect(&this->_tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onAcceptError(QAbstractSocket::SocketError)));

    connect(&this->_udpServer, SIGNAL(readyRead()), this, SLOT(onReceiveUdpPacket()));
}

HostedCall::~HostedCall()
{

}

void            HostedCall::mute()
{
}

void            HostedCall::silence()
{
}

bool            HostedCall::isHost() const
{
    return (true);
}

QString const   &HostedCall::getUsernameById(int id) const
{
    QMap<int, Callee>::const_iterator it = this->_connectedUsers.find(id);
    return (it->getUsername());
}

void            HostedCall::start()
{
    if (!this->_tcpServer.listen())
        throw std::runtime_error("Cannot bind TCP server");
    if (!this->_udpServer.bind())
        throw std::runtime_error("Cannot bind UDP server");
}

unsigned short  HostedCall::getTcpPort() const
{
    return (this->_tcpServer.serverPort());
}

unsigned short  HostedCall::getUdpPort() const
{
    return (this->_udpServer.localPort());
}

QMap<int, Callee> const &HostedCall::getCallees() const
{
    return (this->_connectedUsers);
}

void            HostedCall::addCallee(const QString &username, const QString &ip)
{
    this->_allowedUser.push_back(Callee(ip, username));
}

void            HostedCall::onReceiveSendTextMsg(int id, const QString &msg)
{
    std::string             packet;

    PacketHelper::writeTcpHeader(packet, SendText);
    packet = packet.append(Serializer::serialize<std::string>(msg.toStdString()));
    PacketHelper::writeTcpHeaderSize(packet);
    this->broadcastTcpMsg(id, packet.c_str());
}

void            HostedCall::receiveNewTcpConnection()
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
                connect(sock, SIGNAL(readyRead()), &this->_worker, SLOT(onReadSomething()));
                connect(sock, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
                connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
                //Send welcome
                //Send ParticipantStatusUpdate
                this->_connectedUsers[this->_connectedUsers.count()] = Callee("test", "lol");
                this->_allowedUser.erase(it);
                break;
            }
        }
        if (!ok)
            sock->close();
    }
}

void            HostedCall::onAcceptError(QAbstractSocket::SocketError)
{

}

void            HostedCall::onClientDisconnected()
{

}

void            HostedCall::onSocketError(QAbstractSocket::SocketError)
{

}

void            HostedCall::onReceiveUdpPacket()
{

}

void            HostedCall::broadcastTcpMsg(int sender, const std::string &msg)
{
    for (QMap<int, Callee>::iterator it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); ++it)
    {
        if (it.key() != sender)
            it->getSock()->write(msg.c_str(), msg.length());
    }
}
