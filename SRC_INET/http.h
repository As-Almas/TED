#pragma once

bool LoadSysDll(WORD version);
bool InitHttpSocket(LPCSTR url);
bool ConnectToHttpServer();
bool SendHttpGETRequest(LPCSTR PTFWP, LPCSTR site);
LPCSTR GetHttpRequestResult();

void CloseHttpSock();
void FreeSysDll();

