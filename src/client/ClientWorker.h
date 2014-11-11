#pragma once

#include <QTcpSocket>
#include <QThread.h>
#include <QString>
#include "ClientData.h"
#include "eErrorCode.h"
#include "constant.h"
#include "Call.h"
#include "eCommandId.h"

class MainWindow;

class ClientWorker : public QObject
{
	Q_OBJECT

public:
    ClientWorker(ClientData &, QObject *parent = 0);
	~ClientWorker();

signals:
    void    commandFailed(eErrorCode);
    void    connectedToServer();
    void    connectionFailed();

    void    friendStatusUpdate();

    void    friendRequest(QString const &);
    void    friendRequestSended(QString const &);

    void    callRequest(QString const &, std::list<std::string> const &);
    void    callDropped(QString const &);

public slots:
    void    onReadSomething();

    void    onSignInRequest();
    void    onSignUpRequest();

    void    onConnectionSuccess();
    void    onDisconnected();
    void    onSocketError(QAbstractSocket::SocketError);
	void	addFriendRequest(QString const &);
	void	deleteFriendRequest(QString const &);
	void	friendValidationRequest(QString const &, bool);
	void	answerCallRequest(QString const &, bool);

    void    addToCallRequest(QString const &);

    void    onConnectedToRemoteHost();

private:
    void     connectToServer();

    bool    isHeaderValid(std::istringstream &, struct TCPPacketHeader &);
    void    writeHeader(std::string &, eCommandId);

    void    receiveAuthRequest(std::istringstream &, int);
    void    receiveFriendList(std::istringstream &, int);
    void    receiveFriendStatusUpdate(std::istringstream &, int);
    void    receiveFriendRequest(std::istringstream &, int);
    void	receiveCallRequest(std::istringstream &, int);
    void	receiveCallDropped(std::istringstream &, int);
    void    receiveHostRequest(std::istringstream &, int);
    void    receiveConnectToPeer(std::istringstream&, int);
    void    receiveOk(std::istringstream &, int);
    void    receiveKO(std::istringstream &, int);
  

    char                _buff[BUFFER_SIZE];
	ClientData			&_data;
    QTcpSocket          _serverSocket;
    Call                _call;
};
