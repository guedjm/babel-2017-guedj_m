#pragma once

#include <QTcpServer>
#include <QUdpSocket>
#include <QObject>
#include "ICall.h"
#include "Callee.h"
#include "eCalleeStatus.h"
#include "ISoundManager.h"
#include "SoundMixer.h"
#include "SoundContainer.h"

class ClientWorker;

class HostedCall : public QObject, public ICall
{
    Q_OBJECT
public:
    explicit    HostedCall(ClientWorker &, QObject *parent = 0);
    virtual     ~HostedCall();

    virtual void    mute();
    virtual void    silence();
    virtual bool    isHost() const;
    virtual QString const   &getUsernameById(int) const;
    virtual void    sendInput();

    void            start();
    unsigned short  getTcpPort() const;
    unsigned short  getUdpPort() const;

    QMap<int, Callee> const &getCallees() const;
    void                    addCallee(QString const &username, QString const &ip);
    void                    onReceiveSendTextMsg(int, QString const &);

signals:

public slots:
    void        receiveUdp();

private slots:
    void        receiveNewTcpConnection();       
    void        onAcceptError(QAbstractSocket::SocketError);
    void        onClientDisconnected();
    void        onSocketError(QAbstractSocket::SocketError);

    void        onReceiveUdpPacket();

private:
    void        broadcastTcpMsg(int, std::string const &);

    QTcpServer          _tcpServer;
    QUdpSocket          _udpServer;
    QList<Callee>       _allowedUser;
    QMap<int, Callee>   _connectedUsers;
    ClientWorker        &_worker;

    ISoundManager       *_sManager;
    SoundContainer      _container;
};
