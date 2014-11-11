#pragma once

enum eErrorCode
{
	Success = 0,
	InvalidUsername = 1,
	InvalidPassword = 2,
	UnavailableUsername = 3,
	UnknownUser = 4,
	UserAlreadyConnected = 5,
	AlreadyFriend = 6,
	FriendsMaxedOut = 7,
	UserOffline = 8,
	RequestPending = 9,
	AuthentificationTimeOut = 10
};