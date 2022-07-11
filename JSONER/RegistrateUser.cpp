#include "RegistrateUser.h"

typedef bool (*LOAD_DLL)(WORD v),
(*CREATE_SOCK)(LPCSTR url),
(*CONNECT_TO_SERV)(),
(*SEND_REQUEST)(LPCSTR content, LPCSTR path, LPCSTR host);
typedef LPCSTR (*GET_REQ_RESULT)();

void OnSubmitClick(UI* submitEl, int action)
{
        HMODULE SRC_LIB = LoadLibrary(L"SRC_INET");
        if (!SRC_LIB)
        {
            MessageBox(NULL, L"Cannot init request to server! 'SRC_INET.dll' is not found!", L"REQUEST ERROR!", MB_ICONERROR);
            return;
        }
        LOAD_DLL LoadSysDll = (LOAD_DLL)GetProcAddress(SRC_LIB, "LoadSysDll");
        CREATE_SOCK InitSocket = (CREATE_SOCK)GetProcAddress(SRC_LIB, "InitHttpSocket");
        CONNECT_TO_SERV ConnectToHttpServer = (CONNECT_TO_SERV)GetProcAddress(SRC_LIB, "ConnectToHttpServer");
        SEND_REQUEST SendHttpGET = (SEND_REQUEST)GetProcAddress(SRC_LIB, "SendHttpPOSTRequest");
        GET_REQ_RESULT GetResult = (GET_REQ_RESULT)GetProcAddress(SRC_LIB, "GetHttpRequestResult");


        if (!LoadSysDll(MAKEWORD(2, 2))) {
            MessageBox(GetParent(submitEl->GetUIWindow()), L"ERROR: cannot init request!", L"ERROR", MB_ICONERROR);
            FreeLibrary(SRC_LIB); 
            return;
        }
        if (!InitSocket("www.TableEncryptedData.com")) {
            MessageBox(GetParent(submitEl->GetUIWindow()), L"ERROR: cannot connect to server!", L"ERROR", MB_ICONERROR);
            GetProcAddress(SRC_LIB, "FreeSysDll")();
            FreeLibrary(SRC_LIB);
            return;
        }
        if (!ConnectToHttpServer())
        {
            MessageBox(GetParent(submitEl->GetUIWindow()), L"ERROR: cannot connect to server!", L"ERROR", MB_ICONERROR);
            GetProcAddress(SRC_LIB, "CloseHttpSock")();
            GetProcAddress(SRC_LIB, "FreeSysDll")();
            FreeLibrary(SRC_LIB);
            return;
        }
        if (!SendHttpGET("login=As_Almas&password=123456fsdspps&code=TED_CODE_mp3dfjvfdcnjudndxcsukcdunjcjn/sdju", "/application_reg.php", "www.TableEncryptedData.com")) {
            MessageBox(GetParent(submitEl->GetUIWindow()), L"ERROR: cannot send request to server!", L"ERROR", MB_ICONERROR);
        }
        LPCSTR res = GetResult();
        GetProcAddress(SRC_LIB, "CloseHttpSock")();
        GetProcAddress(SRC_LIB, "FreeSysDll")();
        std::cout << res;
        FreeLibrary(SRC_LIB);
}
