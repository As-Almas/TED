#include "pch.h"
#include "http.h"

#include <string>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

WSADATA SysDllData;
SOCKET servSock;
SOCKADDR_IN sockAddr;
string result;
int contentLength = 0;

bool CheckHTTPHeaders(string headers) {
    if (headers.empty())
        return false;
    
    if (headers.find("Content-Length:") == string::npos)
        return false;

    string oneHead = headers.substr(0, headers.find_first_of("\r\n"));
    headers = headers.substr(oneHead.size()+1);
    if (oneHead != "HTTP/1.1 200 OK")
        return false;
    try
    {
        headers = headers.substr(headers.find("Content-Length:")+15);
        oneHead = headers.substr(0, headers.find("\r\n"));
        contentLength = stoi(oneHead);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

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
    string get = "GET "s + PTFWP + " HTTP/1.1\r\nHost: " + site + "\r\nReferer: TED - user app\r\nAccept: text/html\r\nConnection: close\r\n\r\n";

    if (send(servSock, get.c_str(), get.size(), 0) <= 0) {
        result = "Error\0";
        return false;
    }
    
    char* res = new char[1024];
    int AllReadBytes = 0;
    if ((AllReadBytes = recv(servSock, res, 1024, 0)) < 0) {
        result = "Internet error\0";
        return false;
    }

    result = res;
    string headers = result.substr(0, result.find("\r\n\r\n"));
    result = result.substr(result.find("\r\n\r\n"));
    

    if (!CheckHTTPHeaders(headers)) {
        result = "SERVER SEND ERROR HEADER!\0";
        return false;
    }
    
    if (AllReadBytes < contentLength) {
        delete res;
        res = new char[contentLength];
        int ReadBytes;
        if ((ReadBytes = recv(servSock, res, contentLength, 0)) < 0) {
            result = "ERROR CONTENT!\0";
            return false;
        }
        AllReadBytes += ReadBytes;
        if (AllReadBytes < contentLength) {
            return false;
        }
        res[ReadBytes] = '\0';
        result += res;
        delete res;
    }

    return true;
}

bool SendHttpPOSTRequest(LPCSTR content, LPCSTR path, LPCSTR host)
{
    char* buf = new char[11];
    _itoa_s(strlen(content), buf, 10, 10);
    string sendHeaders =
        "POST "s + path + " HTTP/1.1\r\n"
        "Host: " + host + "\r\n"
        "Referer: TED - user app\r\n"
        "Accept: text/html, text/plain\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: " +  buf +
        "\r\n\r\n" + content;
    if (send(servSock, sendHeaders.c_str(), sendHeaders.size(), 0) <= 0) {
        result = "Error\0";
        return false;
    }

    char* res = new char[1024];
    int AllReadBytes = 0;
    if ((AllReadBytes = recv(servSock, res, 1024, 0)) < 0) {
        result = "Internet error\0";
        return false;
    }

    result = res;
    string headers = result.substr(0, result.find("\r\n\r\n"));
    result = result.substr(result.find("\r\n\r\n")+4);


    if (!CheckHTTPHeaders(headers)) {
        result = "SERVER SEND ERROR HEADER!\0";
        return false;
    }
    
    AllReadBytes = AllReadBytes -headers.size()-4;
    if (AllReadBytes < contentLength) {
        delete res;
        res = new char[contentLength];
        int ReadBytes;
        if ((ReadBytes = recv(servSock, res, contentLength, 0)) < 0) {
            result = "ERROR CONTENT!\0";
            return false;
        }
        AllReadBytes += ReadBytes;
        if (AllReadBytes < contentLength) {
            return false;
        }
        res[ReadBytes] = '\0';
        result += res;
        delete res;
    }
    result[AllReadBytes] = '\0';

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
