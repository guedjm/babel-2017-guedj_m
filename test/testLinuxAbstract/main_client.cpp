#include <exception>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "../../src/common/IUDPClient.h"
#include "../../src/common/LinuxUDPClient.h"

int main()
{
    std::string ip = "127.0.0.1";
    unsigned short port = 4242;
    IUDPClient  *client;
    int rcvSize;
    int sendSize;
    char buff[200];

    std::cout << "Launching server ..." << std::endl;
    system("./testServer&");
    sleep(2);
    client = new LinuxUDPClient();
    std::cout << "CLIENT : Sendind data ..." << std::endl;
    sleep(1);
    std::cout << "CLIENT : Sending HELLO ..." << std::endl;
    sendSize = client->sendTo("HELLO", 6, ip, port);
    rcvSize = client->recvFrom(buff, 200, ip, port);
    if (sendSize != rcvSize)
        std::cout << "SEND DIFFER" << std::endl;

    sleep(1);
    std::cout << "CLIENT : Sending HOW ARE U ? ..." << std::endl;
    sendSize = client->sendTo("HOW ARE U ?", 12, ip, port);
    rcvSize = client->recvFrom(buff, 200, ip, port);
    if (sendSize != rcvSize)
        std::cout << "SEND DIFFER" << std::endl;

    sleep(1);
    std::cout << "CLIENT : Sending LOL ..." << std::endl;
    sendSize = client->sendTo("LOL", 4, ip, port);
    rcvSize = client->recvFrom(buff, 200, ip, port);
    if (sendSize != rcvSize)
        std::cout << "SEND DIFFER" << std::endl;

    sleep(1);
    sendSize = client->sendTo("STOP", 5, ip, port);
    std::cout << "CLIENT : Sending STOP ... (" << sendSize << ")" << std::endl;
    rcvSize = client->recvFrom(buff, 200, ip, port);
    if (sendSize != rcvSize)
        std::cout << "SEND DIFFER" << std::endl;
    client->closeSock();
}
