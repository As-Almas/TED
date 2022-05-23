#include "UI.h"

#pragma comment(lib, "comctl32.lib")

void UI::DrawUIText(DRAWITEMSTRUCT* dc, COLORREF color, int fSize)
{
	SetTextColor(dc->hDC, color);
	if (!font) {
		printf("\nSome fonts is not loaded!");
	}
	else {
		SelectObject(dc->hDC, font);
	}

	int length = GetWindowTextLength(this->wnd);
	DrawText(
		dc->hDC,
		this->parametrs.text.c_str(),
		length,
		&dc->rcItem,
		DT_SINGLELINE | DT_LEFT | this->textStyles
	);
	if (font != NULL) {
		SelectObject(dc->hDC, NULL);
	}
}

void UI::DrawDef(DRAWITEMSTRUCT* dc)
{
	//SetCursor(0);
	SetTextColor(dc->hDC, this->defText);

	HBRUSH brush = CreateSolidBrush(this->defBack);
	FillRect(dc->hDC, &dc->rcItem, brush);
	SetBkMode(dc->hDC, TRANSPARENT);

	SelectObject(dc->hDC, brush);

	DrawUIText(dc, this->defText, fontSize);
	SelectObject(dc->hDC, NULL);
	DrawBorder(dc);

	DeleteObject(brush);
}

void UI::DrawFocus(DRAWITEMSTRUCT* dc)
{
	SetCursor(cursor);
	SetTextColor(dc->hDC, this->FocusText);

	HBRUSH brush = CreateSolidBrush(this->FocusBack);
	FillRect(dc->hDC, &dc->rcItem, brush);
	SetBkMode(dc->hDC, TRANSPARENT);

	SelectObject(dc->hDC, brush);

	

	DrawUIText(dc, this->FocusText, fontSize);
	SelectObject(dc->hDC, NULL);
	DrawBorder(dc);

	DeleteObject(brush);
}

void UI::DrawBorder(DRAWITEMSTRUCT* dc)
{
	HPEN BorderPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	SelectObject(dc->hDC, BorderPen);

	if (this->border.top) {
		MoveToEx(dc->hDC, dc->rcItem.left, dc->rcItem.top, NULL);
		LineTo(dc->hDC, dc->rcItem.right, dc->rcItem.top);
	}
	if (this->border.bottom) {
		MoveToEx(dc->hDC, dc->rcItem.left, dc->rcItem.bottom, NULL);
		LineTo(dc->hDC, dc->rcItem.right, dc->rcItem.bottom);
	}
	if (this->border.left) {
		MoveToEx(dc->hDC, dc->rcItem.left, dc->rcItem.top, NULL);
		LineTo(dc->hDC, dc->rcItem.left, dc->rcItem.bottom);
	}
	if (this->border.right) {
		MoveToEx(dc->hDC, dc->rcItem.right, dc->rcItem.top, NULL);
		LineTo(dc->hDC, dc->rcItem.right, dc->rcItem.bottom);
	}

	SelectObject(dc->hDC, NULL);
	DeleteObject(BorderPen);
}

UI::UI(HWND parent, int id) : parentWnd(parent), id(id)
{
}

bool UI::Create(LPCWSTR elType, UI_PARAMS params)
{
	this->parametrs = params;
	this->type = elType;
	RECT rc;
	GetClientRect(this->parentWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int elX = parametrs.x * width;
	int elY = parametrs.y * height;
	int elWidth = parametrs.width * width;
	int elHeight = parametrs.height * height;

	if (!this->LoadFont())
		MessageBox(this->parentWnd, L"WARN: some fonts is not loaded!", L"WARN", MB_ICONWARNING);

	this->wnd = CreateWindow(
		elType,
		params.text.c_str(),
		WS_CHILD | WS_VISIBLE | this->anyStyles,
		elX,elY,
		elWidth, elHeight,
		this->parentWnd, 
		(HMENU)this->id,
		NULL,
		NULL
		);
	UpdateWindow(this->parentWnd);
	if (!this->wnd)
		this->id = ERROR_UI;
	return this->wnd !=NULL;
}

bool UI::Create()
{
	RECT rc;
	GetClientRect(this->parentWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int elX = parametrs.x * width;
	int elY = parametrs.y * height;
	int elWidth = parametrs.width * width;
	int elHeight = parametrs.height * height;

	if (!this->LoadFont())
		MessageBox(this->parentWnd, L"WARN: some fonts is not loaded!", L"WARN", MB_ICONWARNING);
	this->wnd = CreateWindow(
		this->type,
		this->parametrs.text.c_str(),
		WS_CHILD | WS_VISIBLE | this->anyStyles,
		elX, elY,
		elWidth, elHeight,
		this->parentWnd,
		(HMENU)this->id,
		NULL,
		NULL
	);
	UpdateWindow(this->parentWnd);
	if (!this->wnd)
		this->id = -13893;
	return this->wnd != NULL;
}

bool UI::LoadFont()
{
	this->font = CreateFont(
		this->fontSize,
		0,
		0,
		0,
		this->weight,
		this->isItalic,
		this->isUnderLine,
		this->isStrikeOut,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		this->fontName
	);
	if (!font) {
		printf("\nSome fonts is not loaded!");
		return false;
	}
	return true;
}

void UI::UnLoadFont()
{
	DeleteObject(font);
	font = NULL;
}


void UI::Resize()
{
	RECT rc;
	GetClientRect(this->parentWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int elX = parametrs.x * width;
	int elY = parametrs.y * height;
	int elWidth = parametrs.width * width;
	int elHeight = parametrs.height * height;


	
	SetWindowPos(this->wnd,
		HWND_TOP,
		elX, elY,
		((elWidth > maxWidth && maxWidth != 0) ? maxWidth : elWidth),
		((elHeight > maxHeight && maxHeight != 0) ? maxHeight : elHeight),
		SWP_ASYNCWINDOWPOS
	);


	UpdateWindow(this->parentWnd);
}

void UI::Resize(UI_PARAMS params)
{
	RECT rc;
	GetClientRect(this->parentWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int elX = params.x * width;
	int elY = params.y * height;
	int elWidth = params.width * width;
	int elHeight = params.height * height;

	SetWindowPos(this->wnd, HWND_TOP, elX, elY, elWidth, elHeight, SWP_ASYNCWINDOWPOS);
	UpdateWindow(this->parentWnd);
}

void UI::Click()
{
	if (*this->onClick)
		onClick(this);
}

void UI::SetText(LPCWSTR text)
{
	this->parametrs.text = text;
	SetWindowText(this->wnd, text);
	UpdateWindow(this->parentWnd);
}

void UI::SetParams(LPCWSTR elType, UI_PARAMS params)
{
	this->type = elType;
	this->parametrs = params;
}

void UI::SetMaxSize(int width, int height)
{
	this->maxWidth = width;
	this->maxHeight = height;
}

void UI::SetFont(LPCWSTR fontName, int size)
{
	this->fontName = fontName;
	this->fontSize = size;	
}

void UI::AddTextStyle(DWORD userStyle)
{
	this->textStyles |= userStyle;
}

void UI::AddStyle(DWORD userStyle)
{
	this->anyStyles |= userStyle;
}

void UI::SetUserCursor(LPWSTR id)
{
	cursor = LoadCursor(NULL, id);
}

void UI::DrawTopBorder()
{
	this->border.top = true;
}

void UI::DrawLeftBorder()
{
	this->border.left = true;
}

void UI::DrawRightBorder()
{
	this->border.right = true;
}

void UI::DrawBottomBorder()
{
	this->border.bottom = true;
}

void UI::SetDefBackground(int r, int g, int b)
{
	this->defBack = RGB(r, g, b);
}

void UI::SetFocusBackground(int r, int g, int b)
{
	this->FocusBack = RGB(r, g, b);
}

void UI::SetDefTextColor(int r, int g, int b)
{
	this->defText = RGB(r, g, b);
}

void UI::SetFocusTextColor(int r, int g, int b)
{
	this->FocusText = RGB(r, g, b);
}

void UI::SetOnClick(void(*func)(UI*))
{
	this->onClick = func;
}

void UI::SetOnFocus(void(*func)(UI*))
{
	this->onFocus = func;
}

void UI::SetOnFocusOut(void(*func)(UI*))
{
	this->onFocusOut = func;
}

bool UI::isFocus()
{
	return this->isFocused;
}

void UI::Focus()
{
	this->isFocused = true;
	SetWindowText(this->wnd, this->parametrs.text.c_str());
	if (*this->onFocus)
		this->onFocus(this);
}

void UI::UnFocus()
{
	this->isFocused = false;
	SetWindowText(this->wnd, this->parametrs.text.c_str());
	if (*this->onFocusOut)
		this->onFocusOut(this);
}

void UI::Draw(DRAWITEMSTRUCT* dc)
{
	if (this->id < 0)
		return;
	switch (dc->itemAction) {
	case ODA_SELECT:
	case ODA_FOCUS: {
		this->DrawFocus(dc);
	}break;
	case ODA_DRAWENTIRE: {
		if (!isFocused) {
			this->DrawDef(dc);
		}
		else {
			this->DrawFocus(dc);
		}
	}break;
	}
}

void UI::Destroy()
{
	this->~UI();
}

HWND UI::GetUIWindow()
{
	return this->wnd;
}

UI::~UI()
{
	DestroyWindow(this->wnd);
	this->id = NULL;
	this->anyStyles = NULL;
	this->wnd = NULL;
	this->type = L"";
	this->defBack = 0;
	this->defText = 0;
	this->FocusBack = 0;
	this->FocusText = 0;
	this->isFocused = false;
	this->maxHeight = 0;
	this->parametrs.height = 0;
	this->parametrs.width = 0;
	this->parametrs.text.clear();
	this->parametrs.x = 0;
	this->parametrs.y = 0;
	this->UnLoadFont();
	DeleteObject(this->back);
	UpdateWindow(this->parentWnd);	
}






//operators
bool UI::operator==(int otherId)
{
	return this->id == otherId;
}

bool UI::operator!=(const int otherId)
{
	return !this->operator==(otherId);
}

bool UI::operator==(const UI otherUI)
{
	return this->operator==(otherUI.id);
}

bool UI::operator!=(const UI otherUI)
{
	return !this->operator==(otherUI);
}

bool UI::operator==(const HWND otherWindow)
{
	return this->wnd == otherWindow;
}

bool UI::operator!=(const HWND otherWindow)
{
	return !this->operator==(otherWindow);
}

void UI::operator=(const UI otherUI)
{
	CloseWindow(this->wnd);
	DestroyWindow(this->wnd);
	this->parametrs.text = otherUI.parametrs.text;
	this->anyStyles = otherUI.anyStyles;
	this->Create(otherUI.type, this->parametrs);
}
