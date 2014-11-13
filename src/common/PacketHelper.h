#pragma once

#include <sstream>
#include "eCommandId.h"
#include "TCPPacketHeader.h"
#include "UDPPacketHeader.h"

class PacketHelper
{
public:
    static bool readTcpHeader(std::istringstream &, struct TCPPacketHeader &);
    static void writeTcpHeader(std::string &, eCommandId);
    static void writeTcpHeaderSize(std::string &);

    static bool readUdpHeader(std::istringstream &, struct UDPPacketHeader &);
};
