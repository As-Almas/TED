#pragma once
extern "C" {
	__declspec(dllexport) bool FolderExists(LPWSTR path);
	__declspec(dllexport) bool CreateFolder(LPCWSTR path);
};

