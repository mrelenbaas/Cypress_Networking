// udp_server.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://www.youtube.com/watch?v=uIanSvWou1M&list=WL&index=3&t=0s

#include <iostream>
#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

int main()
{
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOK = WSAStartup(version, &data);
    if (wsOK != 0)
    {
        std::cout << "Error: startup failed" << wsOK << std::endl;
    }

    SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverHint;
    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(1234);

    if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
    {
        std::cout << "Error: failed to bind socket" << std::endl;
        return 0;
    }

    sockaddr_in client;
    int clientLength = sizeof(client);
    ZeroMemory(&client, clientLength);


    char buf[1024];

    while (true)
    {
        ZeroMemory(buf, 1024);

        int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
        if (bytesIn == SOCKET_ERROR)
        {
            std::cout << "Error: receiving from client" << WSAGetLastError() << std::endl;
            continue;
        }

        char clientIp[256];
        ZeroMemory(clientIp, 256);

        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        std::cout << clientIp << " : " << buf << std::endl;
    }

    closesocket(in);
    WSACleanup();

    std::cout << "Complete" << std::endl;
}
