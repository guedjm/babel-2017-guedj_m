#include <sstream>
#include "PacketHelper.h"
#include "RemoteCall.h"
#include "ClientWorker.h"
#include "UDPPacketHeader.h"
#include "PortAudioSoundManager.h"

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
    this->_sManager = new PortAudioSoundManager(this->_container, this);
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

void            RemoteCall::sendInput()
{
    int             size;
    unsigned char   buff[BUFFER_SIZE];

    size = this->_container.getInput().cpyEncriptedSound(buff, BUFFER_SIZE);
    this->_udpClient.writeDatagram((char*)buff, QHostAddress(this->_ip), this->_udpPort);
}

void            RemoteCall::start(const QString &ip, unsigned short port)
{
    this->_ip = ip;
    this->_tcpClient.connectToHost(ip, port);
}

void            RemoteCall::setMyId(unsigned int id)
{
    this->_myId = id;
}

void            RemoteCall::addCallee(const QString &name, unsigned int id)
{
    this->_connectedUsers[id] = name;
}

void            RemoteCall::removeCallee(unsigned int id)
{
    this->_connectedUsers.remove(id);
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
    long int                size;
    QHostAddress            from;
    unsigned short          port;
    long int                current_size;
    char                    buff[BUFFER_SIZE];
    struct UDPPacketHeader  header;

    current_size = 0;
    size = this->_udpClient.readDatagram(buff, BUFFER_SIZE, &from, &port);
    std::istringstream st(buff, size);
    while ((size - current_size) > sizeof(struct UDPPacketHeader))
    {
        if (PacketHelper::readUdpHeader(st, header) && (size >= current_size +  (sizeof(struct UDPPacketHeader) + header.payloadSize)))
            this->_container.pushEncriptedOutputSound(header.senderID, (unsigned char*)buff, header.payloadSize);
        current_size += (sizeof(struct UDPPacketHeader) + header.payloadSize);
    }
}



