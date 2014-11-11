#pragma once

enum    eCommandId
{
	RequestAuth = 1,
	SignUp = 2,
	SignIn = 3,
	SendFriendsList = 4,
	FriendStatusUpdate = 5,
	FriendRequest = 6,
	FriendValidation = 7,
	AddToCall = 8,
	AnswerCall = 9,
	CallDropped = 10,
	Host = 11,
	HostReady = 12,
	ConnectToPeer = 13,
	OK = 14,
	KO = 15,
	Welcome = 16,
	ParticipantStatusUpdate = 17,
	UDPReady = 18,
	SendText = 19
};
