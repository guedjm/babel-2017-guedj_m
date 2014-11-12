#include <QEventLoop>
#include <QString>
#include <iostream>
#include <string>
#include <sstream>
#include "PacketHelper.h"
#include "Serializer.hpp"
#include "ClientWorker.h"
#include "eFriendRequestType.h"
#include "eFriendStatus.h"
#include "mainwindow.h"
#include "TCPPacketHeader.h"
#include "HostedCall.h"
#include "RemoteCall.h"

ClientWorker::ClientWorker(ClientData &data, QObject *):
    _data(data),
    _serverSocket(this)
{
    this->_call = 0;
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<QString>("QString");
    connect(&this->_serverSocket, SIGNAL(connected()), this, SLOT(onConnectionSuccess()));
    connect(&this->_serverSocket, SIGNAL(readyRead()), this, SLOT(onReadSomething()));
    connect(&this->_serverSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(&this->_serverSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

ClientWorker::~ClientWorker()
{

}

void        ClientWorker::connectToServer()
{
    QString     ip;
    unsigned short  port;

    this->_data.lockServer();
    ip = this->_data.getServerIp();
    port = this->_data.getServerPort();
    this->_data.unlockServer();
    this->_serverSocket.connectToHost(ip, port);
}

void        ClientWorker::onReadSomething()
{
    int                     totalSize;
    int                     currentSize;
    std::istringstream      stream;
    struct TCPPacketHeader  packetHeader;
    QTcpSocket              *client = (QTcpSocket*)(sender());
    QMap<eCommandId, void (ClientWorker::*)(std::istringstream &, int)> _funcs;

    _funcs[RequestAuth] = &ClientWorker::receiveAuthRequest;
    _funcs[SendFriendsList] = &ClientWorker::receiveFriendList;
    _funcs[FriendStatusUpdate] = &ClientWorker::receiveFriendStatusUpdate;
    _funcs[FriendRequest] = &ClientWorker::receiveFriendRequest;
    _funcs[AddToCall] = &ClientWorker::receiveCallRequest;
    _funcs[CallDropped] = &ClientWorker::receiveCallDropped;
    _funcs[Host] = &ClientWorker::receiveHostRequest;
    _funcs[ConnectToPeer] = &ClientWorker::receiveConnectToPeer;
    _funcs[OK] = &ClientWorker::receiveOk;
    _funcs[KO] = &ClientWorker::receiveKO;
    _funcs[Welcome] = &ClientWorker::receiveWelcome;
    _funcs[ParticipantStatusUpdate] = &ClientWorker::receiveParticipantStatusUpdate;
    _funcs[UDPReady] = &ClientWorker::receiveUdpReady;
    _funcs[SendText] = &ClientWorker::receiveSendText;

    currentSize = 0;
    totalSize = client->read(this->_buff, BUFFER_SIZE);
    stream.str(std::string(this->_buff, totalSize));
    while ((totalSize - currentSize) >= sizeof(packetHeader) && currentSize < totalSize)
    {
        if (PacketHelper::readTcpHeader(stream, packetHeader)
                && (unsigned)(totalSize - currentSize) >= (sizeof(packetHeader) + packetHeader.payloadSize))
            (this->*_funcs[(eCommandId)packetHeader.commandId])(stream, packetHeader.payloadSize);
        currentSize += (sizeof(packetHeader) + packetHeader.payloadSize);
    }
}

void        ClientWorker::onConnectionSuccess()
{
    std::cout << "Connected" << std::endl;
}

void        ClientWorker::onDisconnected()
{
    std::cout << "Disconnected" << std::endl;
}

void        ClientWorker::onSocketError(QAbstractSocket::SocketError err)
{
    if (err == 2 || err == 5)
        emit this->connectionFailed();
}



/*--------------------------- REQUESTS -------------------------*/

void        ClientWorker::onSignInRequest()
{
    this->connectToServer();
    this->_data.lockServer();
    this->_data.setAuthType(SIGNIN);
    this->_data.unlockServer();
}

void        ClientWorker::onSignUpRequest()
{
    this->connectToServer();
    this->_data.lockServer();
    this->_data.setAuthType(SIGNUP);
    this->_data.unlockServer();
}

void        ClientWorker::addToCallRequest(const QString &dest)
{
    std::string                 request;
    std::list<std::string>      calleesName;

    if(this->_call != 0 && !this->_call->isHost())
        return;
    else if (this->_call != 0)
    {
        QMap<int, Callee> const     &callees = ((HostedCall*)(this->_call))->getCallees();
        for (QMap<int, Callee>::const_iterator it = callees.begin(); it != callees.end();
             ++it)
            calleesName.push_back(it->getUsername().toStdString());

    }
    PacketHelper::writeTcpHeader(request, AddToCall);
    request = request.append(Serializer::serialize<std::string>(dest.toStdString()));
    request = request.append(Serializer::serialize<std::list<std::string>>(calleesName));
    PacketHelper::writeTcpHeaderSize(request);
    this->_serverSocket.write(request.c_str(), request.length());
}

void		ClientWorker::addFriendRequest(QString const &dest)
{
    this->_data.lockServer();
    QString const &username = this->_data.getUser().getName();
    this->_data.unlockServer();

    std::string addFriend;

    PacketHelper::writeTcpHeader(addFriend, FriendRequest);
    addFriend = addFriend.append(Serializer::serialize<char>(1));
    addFriend = addFriend.append(Serializer::serialize<std::string>(username.toStdString()));
    addFriend = addFriend.append(Serializer::serialize<std::string>(dest.toStdString()));
    PacketHelper::writeTcpHeaderSize(addFriend);
    this->_serverSocket.write(addFriend.c_str(), addFriend.length());

}

void		ClientWorker::deleteFriendRequest(QString const &dest)
{
    this->_data.lockServer();
    QString const &username = this->_data.getUser().getName();
    this->_data.unlockServer();

    std::string delFriend;

    PacketHelper::writeTcpHeader(delFriend, FriendRequest);
    delFriend = delFriend.append(Serializer::serialize<char>(0));
    delFriend = delFriend.append(Serializer::serialize<std::string>(username.toStdString()));
    delFriend = delFriend.append(Serializer::serialize<std::string>(dest.toStdString()));
    PacketHelper::writeTcpHeaderSize(delFriend);
    this->_serverSocket.write(delFriend.c_str(), delFriend.length());

}

void		ClientWorker::friendValidationRequest(QString const &dest, bool answer)
{
    this->_data.lockServer();
    QString const &username = this->_data.getUser().getName();
    this->_data.unlockServer();

    std::string friendValidation;
    PacketHelper::writeTcpHeader(friendValidation, FriendValidation);
    if (answer == true)
        friendValidation = friendValidation.append(Serializer::serialize<char>(1));
    else
        friendValidation = friendValidation.append(Serializer::serialize<char>(0));
    friendValidation = friendValidation.append(Serializer::serialize<std::string>(username.toStdString()));
    friendValidation = friendValidation.append(Serializer::serialize<std::string>(dest.toStdString()));
    PacketHelper::writeTcpHeaderSize(friendValidation);
    this->_serverSocket.write(friendValidation.c_str(), friendValidation.length());
}

void		ClientWorker::answerCallRequest(QString const &dest, bool answer)
{
    std::string answerCall;

    PacketHelper::writeTcpHeader(answerCall, AnswerCall);
    if (answer == true)
        answerCall = answerCall.append(Serializer::serialize<char>(1));
    else
        answerCall = answerCall.append(Serializer::serialize<char>(0));
    answerCall = answerCall.append(Serializer::serialize<std::string>(dest.toStdString()));
    PacketHelper::writeTcpHeaderSize(answerCall);
    this->_serverSocket.write(answerCall.c_str(), answerCall.length());
}

/*--------------------------------------------------------------*/


/*-------------------------------- RECEIVE -------------------------*/

void        ClientWorker::receiveAuthRequest(std::istringstream &, int)
{
	std::string auth;

    PacketHelper::writeTcpHeader(auth, SignIn);
	auth = auth.append(Serializer::serialize<std::string>(this->_data.getUser().getName().toStdString()));
	auth = auth.append(Serializer::serialize<std::string>(this->_data.getUser().getPassword().toStdString()));
    auth[3] = (this->_data.getAuthType() == SIGNIN) ? SignIn : SignUp;
    PacketHelper::writeTcpHeaderSize(auth);
    this->_serverSocket.write(auth.c_str(), auth.length());
}

void        ClientWorker::receiveFriendList(std::istringstream &buff, int)
{
    std::list<std::pair<std::string, char>>    friendList = Serializer::deserialize<std::list<std::pair<std::string, char>>>(buff);

    this->_data.lockUser();
    User usr = this->_data.getUser();
    for (std::list<std::pair<std::string, char>>::iterator it = friendList.begin();
         it != friendList.end(); ++it)
            usr.addFriend(it->first.c_str(), (eFriendStatus)it->second);
    this->_data.unlockUser();
    emit this->onConnectionSuccess();
}

void        ClientWorker::receiveFriendStatusUpdate(std::istringstream &buff, int)
{
    eFriendStatus status = (eFriendStatus)Serializer::deserialize<char>(buff);
    std::string username = Serializer::deserialize<std::string>(buff);
    this->_data.lockUser();
    User usr = this->_data.getUser();
    usr.friendSetStatus(username.c_str(), status);
    this->_data.unlockUser();
    emit this->friendStatusUpdate();
}

void        ClientWorker::receiveFriendRequest(std::istringstream &buff, int)
{
    eFriendRequestType type = (eFriendRequestType)Serializer::deserialize<char>(buff);
    std::string asker = Serializer::deserialize<std::string>(buff);
    std::string me = Serializer::deserialize<std::string>(buff);
    this->_data.lockUser();
    User usr = this->_data.getUser();
    usr.addFriend(asker.c_str(), FriendRequestPending);
    this->_data.unlockUser();
    emit this->friendRequest(asker.c_str());
}

void        ClientWorker::receiveHostRequest(std::istringstream &buff, int)
{
    std::string hostReady;
    HostedCall  *host;
    std::string ip  = Serializer::deserialize<std::string>(buff);
    std::string username = Serializer::deserialize<std::string>(buff);

    PacketHelper::writeTcpHeader(hostReady, HostReady);
    if (this->_call == 0)
    {
        host = new HostedCall(*this);
        if (host == 0)
                throw std::runtime_error("Cannot alocate new call");
        host->start();
        host->addCallee(username.c_str(), ip.c_str());
        this->_call = host;
    }
    else
        host = (HostedCall*)(this->_call);
    hostReady = Serializer::serialize<int>(host->getTcpPort());
    hostReady = Serializer::serialize<std::string>(username);
    PacketHelper::writeTcpHeaderSize(hostReady);
    this->_serverSocket.write(hostReady.c_str(), hostReady.length());
}

void        ClientWorker::receiveConnectToPeer(std::istringstream &buff, int)
{
    RemoteCall      *remote;
    int             port = Serializer::deserialize<int>(buff);
    std::string     ip = Serializer::deserialize<std::string>(buff);
    std::string     username = Serializer::deserialize<std::string>(buff);

    if (this->_call == 0)
    {
        remote = new RemoteCall(*this);
        remote->start(ip.c_str(), port);
        this->_call = remote;
    }
}

void		ClientWorker::receiveCallRequest(std::istringstream &buff, int)
{
	std::string				username = Serializer::deserialize<std::string>(buff);
	std::list<std::string>	_list = Serializer::deserialize<std::list<std::string>>(buff);

    if (this->_call != 0)
        this->answerCallRequest(username.c_str(), false);
    else
        emit this->callRequest(username.c_str(), _list);
}

void		ClientWorker::receiveCallDropped(std::istringstream &buff, int)
{
	std::string username = Serializer::deserialize<std::string>(buff);

    emit this->callDropped(username.c_str());
}

void        ClientWorker::receiveOk(std::istringstream &buff, int)
{
    int cmdId = Serializer::deserialize<int>(buff);
    std::cout << "Command " << cmdId << " succed" << std::endl;
}

void        ClientWorker::receiveKO(std::istringstream &buff, int)
{
    int errorCode = Serializer::deserialize<int>(buff);
    emit this->commandFailed((eErrorCode)errorCode);
}

void        ClientWorker::receiveWelcome(std::istringstream &buff, int)
{
    RemoteCall  *call;
    std::list<std::pair<std::string, char>> names = Serializer::deserialize<std::list<std::pair<std::string, char>>>(buff);
    int id = Serializer::deserialize<int>(buff);

    if (this->_call != 0)
    {
        call = (RemoteCall*)this->_call;
        for (std::list<std::pair<std::string, char>>::iterator it = names.begin();
             it != names.end(); ++it)
        {
            call->addCallee(it->first.c_str(), it->second);
            emit this->addInCall(it->first.c_str());
        }
        call->setMyId(id);
    }
}

void        ClientWorker::receiveParticipantStatusUpdate(std::istringstream &buff, int)
{
    RemoteCall  *call;
    char        status = Serializer::deserialize<char>(buff);
    int         id = Serializer::deserialize<char>(buff);
    std::string username = Serializer::deserialize<std::string>(buff);

    if (status == Join && this->_call != 0)
    {
        call = (RemoteCall*)this->_call;
        call->addCallee(username.c_str(), id);
        emit this->addInCall(username.c_str());
    }
    else if (this->_call != 0)
    {
        call = (RemoteCall*)this->_call;
        call->removeCallee(id);
        emit this->leftCall(username.c_str());
    }
}

void        ClientWorker::receiveUdpReady(std::istringstream &buff, int)
{
    int     port = Serializer::deserialize<int>(buff);

    if (this->_call != 0 && !this->_call->isHost())
        ((RemoteCall*)(this->_call))->onReceiveUDPReady(port);
}

void        ClientWorker::receiveSendText(std::istringstream &buff, int)
{
    int         id = Serializer::deserialize<int>(buff);
    std::string msg = Serializer::deserialize<std::string>(buff);

    if (this->_call != 0)
    {
        if (this->_call->isHost())
            ((HostedCall*)(this->_call))->onReceiveSendTextMsg(id, msg.c_str());
        QString usr = this->_call->getUsernameById(id);
        emit this->sendText(usr, msg.c_str());
    }
}

/*-------------------------------------------------------------------*/
