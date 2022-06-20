#include "pch.h"
#include "http.h"

#include <string>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

WSADATA SysDllData;
SOCKET servSock;
SOCKADDR_IN sockAddr;
string result;

bool LoadSysDll(WORD version)
{
    if (WSAStartup(version, &SysDllData) != 0) {
        return false;
    }
    return true;
}

bool InitHttpSocket(LPCSTR site)
{
    
    servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock < 0)
        return false;
    hostent* host = gethostbyname(site);

    if (!host)
        return false;

    sockAddr.sin_port = htons(80);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    return true;
}

bool ConnectToHttpServer()
{
    if (connect(servSock, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != 0)
        return false;
    return true;
}

bool SendHttpGETRequest(LPCSTR PTFWP, LPCSTR site)
{
    string get = "GET "s + PTFWP + " HTTP/1.1\r\nHost: " + site + "\r\nConnection: close\r\n\r\n";

    if (send(servSock, get.c_str(), get.size(), 0) <= 0) {
        result = "Error\0";
        return false;
    }
    
    char* resHeaders = new char[310];

    if (recv(servSock, resHeaders, 310, 0) < 0) {
        result = "Error!\0";
        return false;
    }
    result = resHeaders;

    result = result.substr(result.find("Content-Length:") + 15);
    result = result.substr(0, result.find_first_of("\r\n"));

    delete resHeaders;
    resHeaders = new char[100];

    int readOfBytes = 0;
    int AllSize = 0;
    result = "";

    while ((readOfBytes = recv(servSock, resHeaders, 100, 0)) > 0) {
        result += resHeaders;

    }
    if ((readOfBytes < 0) && result.empty())
        return false;
    return true;
}

LPCSTR GetHttpRequestResult()
{
    return result.c_str();
}

void CloseHttpSock()
{
    closesocket(servSock);
}

void FreeSysDll()
{
    WSACleanup();
}
