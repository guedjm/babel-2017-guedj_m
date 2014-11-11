#include <exception>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "../../src/common/IUDPServer.h"
#include "../../src/common/LinuxUDPServer.h"

int main()
{
    std::string ip = "";
    unsigned short port = 4242;
    IUDPServer *server;
    int size;
    int send_size;
    char buff[200];

    server = new LinuxUDPServer();
    std::cout << "Starting UDP server ... " << std::endl;
    if (server->start(ip, port) == -1)
    {
        std::cout << "Failed to bind UDP server" << std::endl;
        throw std::exception();
    }

    std::cout << "UDP Socket binded on port " << port << std::endl << "Waiting for message ... " << std::endl;

    while (1)
    {
        size = server->recvFrom(buff, 200, ip, port);
        std::cout << "SERVER : Receive " << size << " bytes from " << ip << " on port " << port << " : 0x" << std::hex;
        for (int i = 0; i < size; ++i)
            std::cout << (int)buff[i] << " ";
        sleep(1);
        std::cout << std::endl << "SERVER : Responding ... " << std::endl;
        send_size = server->sendTo(buff, size, ip, port);
        std::cout << "SERVER : " << std::dec << send_size << " bytes sended" << std::endl;
        if (size != send_size)
            std::cout << "SEND DIFFER" << std::endl;
        if (strncmp("STOP", buff, 4) == 0)
            break;
    }
    server->stop();
}
