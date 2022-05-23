#pragma once
#include <Windows.h>
#include <string>
class UI;
struct tagUI_PARAMS {
	float x, y;
	float width, height;
	std::wstring text;
};

typedef tagUI_PARAMS UI_PARAMS;
