#pragma once
#include "pch.h"

extern "C" {
	__declspec(dllexport) bool Create(HWND main);
	__declspec(dllexport) const char* Open(HWND main);
	__declspec(dllexport) bool OpenProjact(HWND main);
	__declspec(dllexport) bool CreateProjact(HWND main);
	__declspec(dllexport) void LogErrorMessage(int errorID);

	__declspec(dllexport) bool CreateFileFromPath(LPCWSTR path);

	__declspec(dllexport) void GetFolderPath(char*arr);
	__declspec(dllexport) void GetFilePath(char* arr);
}
