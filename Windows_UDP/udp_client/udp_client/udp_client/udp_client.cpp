// udp_client.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://www.youtube.com/watch?v=uIanSvWou1M&list=WL&index=3&t=0s

#include <iostream>
#include <ws2tcpip.h>
#include <string>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#pragma comment (lib, "ws2_32.lib")

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string find_my_ip_address()
{
    //std::string address;
    //std::system("ipconfig");
    std::string result = exec("ipconfig");

    std::cout << "HERE HERE HERE" << std::endl;
    //std::string s = "scott>=tiger>=mushroom";
    std::string s = result;
    std::string delimiter = "\n";
    size_t pos = 0;
    int count = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        //std::cout << count << std::endl;
        ++count;
        //std::cout << token << std::endl;
        if ((token.find("IPv4 Address") != std::string::npos)
            && (token.find("192.168.1.") != std::string::npos))
        {
            std::string address = token.replace(token.begin(), token.begin() + 39, "");
            //std::string address2 = address.replace(address.end()-1, address.end(), "");
            //std::cout << count << std::endl;
            std::cout << address << std::endl;
            return address;
        }
        s.erase(0, pos + delimiter.length());
    }
    //std::cout << "HERE HERE HERE" << std::endl;
    //std::cout << result << std::endl;
    //return address;
}

int main(int argc, char* argv[])
{
    find_my_ip_address();

    /*
    WSADATA data;
    WORD version = MAKEWORD(2, 2);
    int wsOK = WSAStartup(version, &data);
    if (wsOK != 0)
    {
        std::cout << "Error: startup failed" << wsOK << std::endl;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(1234);

    inet_pton(AF_INET, "192.168.1.2", &server.sin_addr);

    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    std::string s(argv[1]);
    int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    if (sendOk == SOCKET_ERROR)
    {
        std::cout << "Error: send failed" << WSAGetLastError() << std::endl;
    }

    closesocket(out);
    WSACleanup();
    */
}
