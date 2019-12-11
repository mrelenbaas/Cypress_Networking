// udp_client.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://www.youtube.com/watch?v=uIanSvWou1M&list=WL&index=3&t=0s

#include <iostream>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOK = WSAStartup(version, &data);
    if (wsOK != 0)
    {
        std::cout << "Error: startup failed" << wsOK << std::endl;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    std::string s(argv[1]);
    int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    if (sendOk == SOCKET_ERROR)
    {
        std::cout << "Error: send failed" << WSAGetLastError() << std::endl;
    }

    closesocket(out);
    WSACleanup();
}
