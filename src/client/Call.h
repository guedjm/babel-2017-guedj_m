#ifndef CALL_H
#define CALL_H

#include <QMap>
#include <QList>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include "Callee.h"

class Call : public QObject
{
    Q_OBJECT
public:
    explicit Call(QObject *parent = 0);
    ~Call();

    unsigned short      getTcpServerPort() const;
    unsigned short      getUdpServerPort() const;

    unsigned short      getRemoteTCPServerPort() const;
    unsigned short      getRemoteUDPServerPort() const;

    QMap<int, Callee>   const &getCallees() const;

    QTcpServer          &getTcpServer();
    QUdpSocket          &getUdpServer();

    QTcpSocket          &getTcpSocket();
    QUdpSocket          &getUdpSocket();

    bool                isActive() const;
    bool                isHost() const;

    void                host(QString const &, QString const &);
    void                joinCall(QString const &, unsigned short);

signals:
    //HOST
    void            tcpServerReady(unsigned short);
    void            udpServerReady(unsigned short);
    void            newCallee(Callee &);

    //CLIENT

public slots:

private slots:
    void            onNewTcpConnection();
    void            onAcceptError(QAbstractSocket::SocketError);
    void            onReceiveTCPPacket();
    void            onClientDisconnected();
    void            onSocketError(QAbstractSocket::SocketError);


private:
    void        startHosting();
    void        broadcastTCPPacket();

    bool        _active;
    bool        _host;

    //HOST
    unsigned short      _tcpServerPort;
    QTcpServer          _tcpServer;
    unsigned short      _udpServerPort;
    QUdpSocket          _udpServer;
    QList<Callee>       _allowedUser;
    QMap<int, Callee>   _connectedUsers;
    //SoundMixer

    //CLIENT
    QString             _remoteServerIp;
    unsigned short      _remoteTCPServerPort;
    QTcpSocket          _tcpSocket;
    unsigned short      _remoteUDPServerPort;
    QUdpSocket          _udpSocket;
    QList<QString>      _callees;

    //BOTH
    //SoundContainer
    //SoundManager
};

#endif // CALL_H
