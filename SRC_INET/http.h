#pragma once
#include <map>

bool LoadSysDll(WORD version);
bool InitHttpSocket(LPCSTR url);
bool ConnectToHttpServer();
bool SendHttpGETRequest(LPCSTR PTFWP, LPCSTR site);
bool SendHttpPOSTRequest(LPCSTR content, LPCSTR path, LPCSTR host);
LPCSTR GetHttpRequestResult();

void CloseHttpSock();
void FreeSysDll();

