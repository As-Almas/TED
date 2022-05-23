#include "pch.h"
#include "reader.h"

typedef string(*GETVALUE)(const char* blockName, LPWSTR pathToFile);

string GetJsonValue(const char* blockName, LPWSTR pathToFile)
{
	try {
		HMODULE jsonLib = LoadLibrary(L"components/JSON.dll");
		if (!jsonLib) {
			return NULL;
		}
		GETVALUE GetValue = (GETVALUE)GetProcAddress(jsonLib, "GetValue");
		if (!*GetValue) {
			FreeLibrary(jsonLib);
			return NULL;
		}

		string res = GetValue(blockName, pathToFile);
		FreeLibrary(jsonLib);
		return res;
	}
	catch (...) {
		MessageBox(NULL, L"ERR: cannot read json file!", L"JSON ERROR", MB_ICONERROR);
		return NULL;
	}
}