#pragma once
#include "Window.h"
#include <string>


class Project_Logic {
private:
	Window wnd;
	std::string pathToFolder;
	HMODULE prLib;
	bool isOpen = false;
	std::string StartFile;
	friend DWORD WINAPI StartCompiler(LPVOID params);
public:
	Project_Logic(Window main, LPCSTR path);
	bool Init();
	void Start();
	~Project_Logic();
};
