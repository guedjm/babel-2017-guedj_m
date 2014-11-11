#include <QEventLoop>
#include <QString>
#include <iostream>
#include <string>
#include <sstream>
#include "Serializer.hpp"
#include "ClientWorker.h"
#include "eFriendRequestType.h"
#include "eFriendStatus.h"
#include "mainwindow.h"
#include "TCPPacketHeader.h"

ClientWorker::ClientWorker(ClientData &data, QObject *):
    _data(data),
    _call(this),
    _serverSocket(this)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<QString>("QString");
    connect(&this->_serverSocket, SIGNAL(connected()), this, SLOT(onConnectionSuccess()));
    connect(&this->_serverSocket, SIGNAL(readyRead()), this, SLOT(onReadSomething()));
    connect(&this->_serverSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(&this->_serverSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    connect(&this->_call.getTcpSocket(), SIGNAL(connected()), this, SLOT(onConnectedToRemoteHost()));
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
    QMap<int, Callee> const     &callees = this->_call.getCallees();
    for (QMap<int, Callee>::const_iterator it = callees.begin(); it != callees.end();
         ++it)
        calleesName.push_back(it->getUsername().toStdString());
    this->writeHeader(request, AddToCall);
    request = request.append(Serializer::serialize<std::string>(dest.toStdString()));
    request = request.append(Serializer::serialize<std::list<std::string>>(calleesName));
    request = request.replace(4, 4, Serializer::serialize<int>(request.length() - sizeof(struct TCPPacketHeader)));
    this->_serverSocket.write(request.c_str(), request.length());
}

void        ClientWorker::onReadSomething()
{
    int                     totalSize;
    int                     currentSize;
    std::istringstream      stream;
    struct TCPPacketHeader  packetHeader;
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

    currentSize = 0;
    totalSize = this->_serverSocket.read(this->_buff, BUFFER_SIZE);
    stream.str(std::string(this->_buff, totalSize));
    while ((totalSize - currentSize) > sizeof(packetHeader) && currentSize < totalSize)
    {
        if (this->isHeaderValid(stream, packetHeader)
                && (totalSize - currentSize) > (sizeof(packetHeader) + packetHeader.payloadSize))
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

void		ClientWorker::addFriendRequest(QString const &dest)
{
	this->_data.lockServer();
	QString const &username = this->_data.getUser().getName();
	this->_data.unlockServer();

	std::string addFriend;

	this->writeHeader(addFriend, FriendRequest);
	addFriend = addFriend.append(Serializer::serialize<char>(1));
	addFriend = addFriend.append(Serializer::serialize<std::string>(username.toStdString()));
	addFriend = addFriend.append(Serializer::serialize<std::string>(dest.toStdString()));
	addFriend = addFriend.replace(4, 4, Serializer::serialize<int>(addFriend.length() - sizeof(struct TCPPacketHeader)));
	this->_serverSocket.write(addFriend.c_str(), addFriend.length());

}

void		ClientWorker::deleteFriendRequest(QString const &dest)
{
	this->_data.lockServer();
	QString const &username = this->_data.getUser().getName();
	this->_data.unlockServer();

	std::string delFriend;

	this->writeHeader(delFriend, FriendRequest);
	delFriend = delFriend.append(Serializer::serialize<char>(0));
	delFriend = delFriend.append(Serializer::serialize<std::string>(username.toStdString()));
	delFriend = delFriend.append(Serializer::serialize<std::string>(dest.toStdString()));
	delFriend = delFriend.replace(4, 4, Serializer::serialize<int>(delFriend.length() - sizeof(struct TCPPacketHeader)));
	this->_serverSocket.write(delFriend.c_str(), delFriend.length());

}

void		ClientWorker::friendValidationRequest(QString const &dest, bool answer)
{
	this->_data.lockServer();
	QString const &username = this->_data.getUser().getName();
	this->_data.unlockServer();

	std::string friendValidation;
	this->writeHeader(friendValidation, FriendValidation);
	if (answer == true)
		friendValidation = friendValidation.append(Serializer::serialize<char>(1));
	else
		friendValidation = friendValidation.append(Serializer::serialize<char>(0));
	friendValidation = friendValidation.append(Serializer::serialize<std::string>(username.toStdString()));
	friendValidation = friendValidation.append(Serializer::serialize<std::string>(dest.toStdString()));
	friendValidation = friendValidation.replace(4, 4, Serializer::serialize<int>(friendValidation.length() - sizeof(struct TCPPacketHeader)));
	this->_serverSocket.write(friendValidation.c_str(), friendValidation.length());
}

void		ClientWorker::answerCallRequest(QString const &dest, bool answer)
{
	std::string answerCall;
	this->writeHeader(answerCall, AnswerCall);
	if (answer == true)
		answerCall = answerCall.append(Serializer::serialize<char>(1));
	else
		answerCall = answerCall.append(Serializer::serialize<char>(0));
	answerCall = answerCall.append(Serializer::serialize<std::string>(dest.toStdString()));
	answerCall = answerCall.replace(4, 4, Serializer::serialize<int>(answerCall.length() - sizeof(struct TCPPacketHeader)));
	this->_serverSocket.write(answerCall.c_str(), answerCall.length());
}

bool        ClientWorker::isHeaderValid(std::istringstream &st, struct TCPPacketHeader &header)
{
    header.magicA = Serializer::deserialize<char>(st);
    header.magicB = Serializer::deserialize<char>(st);
    header.magicC = Serializer::deserialize<char>(st);
    header.commandId = Serializer::deserialize<char>(st);
    header.payloadSize = Serializer::deserialize<int>(st);

    if (header.magicA == TCP_MAGIC_A && header.magicB == TCP_MAGIC_B && header.magicC == TCP_MAGIC_C
            && header.commandId > 0 && header.commandId < 16)
        return (true);
    return (false);
}

void        ClientWorker::writeHeader(std::string &dest, eCommandId cmd)
{
    dest = Serializer::serialize<char>(TCP_MAGIC_A);
    dest = dest.append(Serializer::serialize<char>(TCP_MAGIC_B));
    dest = dest.append(Serializer::serialize<char>(TCP_MAGIC_C));
    dest = dest.append(Serializer::serialize<char>(cmd));
    dest = dest.append(Serializer::serialize<int>(0));
}

void        ClientWorker::receiveAuthRequest(std::istringstream &, int)
{
	std::string auth;

	this->writeHeader(auth, SignIn);
	auth = auth.append(Serializer::serialize<std::string>(this->_data.getUser().getName().toStdString()));
	auth = auth.append(Serializer::serialize<std::string>(this->_data.getUser().getPassword().toStdString()));
	auth = auth.replace(4, 4, Serializer::serialize<int>(auth.length() - sizeof(struct TCPPacketHeader)));
	auth[3] = (this->_data.getAuthType() == SIGNIN) ? SignIn : SignUp;
	this->_serverSocket.write(auth.c_str(), auth.length());
}

void        ClientWorker::receiveFriendList(std::istringstream &buff, int)
{
    std::list<std::pair<std::string, char>>    _friendList = Serializer::deserialize<std::list<std::pair<std::string, char>>>(buff);

    this->_data.lockUser();
    User usr = this->_data.getUser();
    for (std::list<std::pair<std::string, char>>::iterator it = _friendList.begin();
         it != _friendList.end(); ++it)
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
    std::string ip  = Serializer::deserialize<std::string>(buff);
    std::string username = Serializer::deserialize<std::string>(buff);

    this->_call.host(ip.c_str(), username.c_str());
    this->writeHeader(hostReady, HostReady);
    hostReady = Serializer::serialize<int>(this->_call.getTcpServerPort());
    hostReady = Serializer::serialize<std::string>(username);
    hostReady = hostReady.replace(4, 4, Serializer::serialize<int>(hostReady.length() - sizeof(struct TCPPacketHeader)));
    this->_serverSocket.write(hostReady.c_str(), hostReady.length());
}

void        ClientWorker::receiveConnectToPeer(std::istringstream &buff, int)
{
    int             port = Serializer::deserialize<int>(buff);
    std::string     ip = Serializer::deserialize<std::string>(buff);
    std::string     username = Serializer::deserialize<std::string>(buff);

    this->_call.joinCall(ip.c_str(), (unsigned short)port);
}

void		ClientWorker::receiveCallRequest(std::istringstream &buff, int)
{
	std::string				username = Serializer::deserialize<std::string>(buff);
	std::list<std::string>	_list = Serializer::deserialize<std::list<std::string>>(buff);

    //IF CALL ACTIVE -> RESPOND NO
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
}

void        ClientWorker::receiveKO(std::istringstream &buff, int)
{
    int errorCode = Serializer::deserialize<int>(buff);
    emit this->commandFailed((eErrorCode)errorCode);
}

void        ClientWorker::onConnectedToRemoteHost()
{
    //TODO
}
