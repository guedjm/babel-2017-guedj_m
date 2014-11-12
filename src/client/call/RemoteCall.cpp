#include "RemoteCall.h"
#include "ClientWorker.h"

RemoteCall::RemoteCall(ClientWorker &worker, QObject *parent) :
    QObject(parent),
    _worker(worker)
{
    this->_tcpConnected = false;
    this->_udpConnected = false;
    connect(&this->_tcpClient, SIGNAL(connected()), this, SLOT(connectedToHost()));
    connect(&this->_tcpClient, SIGNAL(readyRead()), &this->_worker, SLOT(onReadSomething()));
    //CONNECTION FAILED
    //DISCONNECTED

    connect(&this->_udpClient, SIGNAL(readyRead()), this, SLOT(receiveUdp()));
}

RemoteCall::~RemoteCall()
{
}

void        RemoteCall::mute()
{
}

void        RemoteCall::silence()
{
}

bool        RemoteCall::isHost() const
{
    return (false);
}

QString const   &RemoteCall::getUsernameById(int id) const
{
    QMap<int, QString>::const_iterator it = this->_connectedUsers.find(id);
    return (*it);
}

void            RemoteCall::start(const QString &ip, unsigned short port)
{
    this->_ip = ip;
    this->_tcpClient.connectToHost(ip, port);
}

void            RemoteCall::onReceiveWelcome(const QMap<int, QString> &callees, unsigned int id)
{
    this->_myId = id;
    this->_connectedUsers = callees;
}

void            RemoteCall::onReceiveUDPReady(unsigned short port)
{
    this->_udpPort = port;
    this->_udpClient.bind(DEFAULT_CLIENT_UDP_PORT);
}

void            RemoteCall::connectedToHost()
{
    this->_tcpConnected = true;
}

void            RemoteCall::receiveUdp()
{

}



