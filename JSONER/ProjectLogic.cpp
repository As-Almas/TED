#include "ProjectLogic.h"
#include "resource.h"
#include <iostream>

#define CONSOLE_PROJ "0"
#define GRAPHIC_PROJ "1"
#define EMPTY_PROJEC "2"

using namespace std;
typedef string(*GETVALUE)(const char* blockName, LPWSTR pathToFile);
typedef bool(*INIT_EDITOR)(HWND MainWnd);
typedef void(*START_FUNC)(const char* linkToMainXML);
typedef bool(*EDITOR_INIT)(HWND, LPCWSTR);

DWORD WINAPI StartCompiler(LPVOID params)
{
	Project_Logic* cmpClass = (Project_Logic*)params;
	wstring mFile(cmpClass->StartFile.begin(), cmpClass->StartFile.end());
	wstring startLine = L"compiler.exe \"";
	startLine += wstring(cmpClass->pathToFolder.begin(), cmpClass->pathToFolder.end())+ L"\\" + mFile + L"\"";
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	si.wShowWindow = SW_SHOW;

	BOOL res = CreateProcess(
		NULL,
		(LPWSTR)startLine.c_str(),
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);
	if (!res) {
		MessageBox(cmpClass->wnd.GetWindow(), L"ERROR: cannot run compiler!", L"compiler error!", MB_ICONERROR);
		return -1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	return 0;
}

Project_Logic::Project_Logic(Window main, LPCSTR path) : wnd(main), pathToFolder(path)
{
}

bool Project_Logic::Init()
{
	if(string(pathToFolder).empty())
		return false;
	HMODULE lib = LoadLibrary(L"Filer.dll");
	if (!lib) {
		return false;
	}

	GETVALUE getJsonVal = (GETVALUE)GetProcAddress(lib, "GetJsonValue");
	if (!*getJsonVal) {
		FreeLibrary(lib);
		return false;
	}

	string pathToFile = pathToFolder;
	pathToFile += "\\";
	pathToFile += "config.json";

	wstring x(pathToFile.begin(), pathToFile.end());

	string mainObj = getJsonVal("main", (LPWSTR)x.c_str());
	this->StartFile = mainObj.c_str();
	string title = getJsonVal("name", (LPWSTR)x.c_str());
	FreeLibrary(lib);
	if (mainObj.empty() || title.empty()) {
		MessageBox(wnd.GetWindow(), L"ERR: project main file path or name of project not found  in config file!", L"ERROR #8", MB_ICONERROR);
		return false;
	}

	title = "user project - " + title;
	SetWindowTextA(wnd.GetWindow(), title.c_str());


	wnd.DestroyAllItems();
	wnd.MenuItemEnable(IDM_CREATE_FILE);
	wnd.MenuItemEnable(IDM_FILE_OPENFILE);
	wnd.MenuItemEnable(ID_OPENHOMEWINDOW);
	wnd.MenuItemDisable(IDM_CREATE_PROJACT);
	wnd.MenuItemDisable(IDM_OPEN_PROJACT);
	wnd.Show();
	this->prLib = LoadLibrary(L"components/Editor.dll");
	if (!this->prLib)
	{
		MessageBox(wnd.GetWindow(), L"ERROR cannot open editor!", L"ERROR #10", MB_ICONERROR);
		this->isOpen = false;
	}
	else {
		INIT_EDITOR init = (INIT_EDITOR)GetProcAddress(this->prLib, "InitEditor");
		if (!*init)
			return false;
		init(wnd.GetWindow());
	}
	this->isOpen = true;
	return this->isOpen;
}

void Project_Logic::Start()
{
	HANDLE hThread = CreateThread(
		0,
		0,
		StartCompiler,
		(LPVOID)this,
		0,
		0
	);
	if (!hThread) {
		MessageBox(this->wnd.GetWindow(), L"ERROR: sys error", L"ERROR #???", MB_ICONERROR);
		return;
	}
	CloseHandle(hThread);
}

Project_Logic::~Project_Logic()
{
	wnd.Hide();
	wnd.DestroyAllItems();
	wnd.MenuItemDisable(IDM_CREATE_FILE);
	wnd.MenuItemDisable(IDM_FILE_OPENFILE);
	SetWindowText(wnd.GetWindow(), L"JSONER");
	FreeLibrary(prLib);
	wnd.Show();
	pathToFolder.clear();
}
