#pragma once

#include <QList>
#include <QUdpSocket>
#include <QObject>
#include "ICall.h"
#include "Callee.h"
#include "ISoundManager.h"
#include "SoundContainer.h"

class ClientWorker;

class RemoteCall : public QObject, public ICall
{
    Q_OBJECT
public:
    explicit RemoteCall(ClientWorker &, QObject *parent = 0);
    virtual  ~RemoteCall();

    virtual void            mute();
    virtual void            silence();
    virtual bool            isHost() const;
    virtual QString const   &getUsernameById(int) const;
    virtual void            sendInput();

    void            start(QString const &, unsigned short);
    void            setMyId(unsigned int);

    void            addCallee(QString const &, unsigned int);
    void            removeCallee(unsigned int);
    void            onReceiveUDPReady(unsigned short);

signals:

private slots:
    void            connectedToHost();
    void            receiveUdp();

public slots:

private:
    int                 _myId;
    QString             _ip;
    unsigned short      _udpPort;
    bool                _tcpConnected;
    bool                _udpConnected;
    QTcpSocket          _tcpClient;
    QUdpSocket          _udpClient;
    QMap<int, QString>  _connectedUsers;
    ClientWorker        &_worker;

    ISoundManager       *_sManager;
    SoundContainer      _container;
};

