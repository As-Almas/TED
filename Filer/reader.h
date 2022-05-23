#pragma once

#include <iostream>

using namespace std;

extern "C" {
	__declspec(dllexport) string GetJsonValue(const char* blockName, LPWSTR pathToFile);
};

