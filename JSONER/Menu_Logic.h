#pragma once

#include "Window.h"

DWORD WINAPI ShowProjectWindow(const char* path);


namespace Logic {
	void Init(Window window);
	void Menu(int id, int action);
	void UnInit();
	void setPath(std::wstring path);	
	LPCWSTR getPath();
}