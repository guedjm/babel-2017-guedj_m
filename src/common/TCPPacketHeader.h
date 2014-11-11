#pragma once

struct		TCPPacketHeader
{
	char	magicA;
	char	magicB;
	char	magicC;
	char	commandId;
	int		payloadSize;
};
