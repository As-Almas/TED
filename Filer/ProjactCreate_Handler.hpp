#pragma once
#include "pch.h"
#include "Headers.h"
#include "Projact.h"
#include <iostream>
#include <ShlObj.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

string ConvertToString(const wstring str) {
	const std::string s(str.begin(), str.end());

	return s;
}

int ProjactTypeID = 0;
bool isCreated = false;
HWND wnd, path, Author, Description, projName, ListBox, close, createProjact, BrowseFolder, title, progBar;

TCHAR prName[MAX_PATH],
prPath[MAX_PATH],
prAuthor[MAX_PATH],
prDesc[1024];

static int maxV = 100;
int current = 0;
string folderPath;
void OnClick(int id, HWND hWnd);


void Error(TCHAR* fpath, TCHAR* name, LPCWSTR errMessage, int id, HWND hWnd) {
	DeleteFolder(fpath, name);
	TerminateResources();
	path = NULL;
	Author = NULL;
	Description = NULL;
	projName = NULL;
	if (MessageBox(NULL, errMessage, L"ERROR #5", MB_ICONERROR | MB_RETRYCANCEL) == IDRETRY)
		return OnClick(id, hWnd);
	DestroyWindow(progBar);
	DestroyWindow(title);
	SendMessage(wnd, WM_CREATE, NULL, NULL);
	SetWindowText(path, prPath);
	SetWindowText(Author, prAuthor);
	SetWindowText(Description, prDesc);
	SetWindowText(projName, prName);
}

void FullProgress(HWND bar) {
	for (; current < maxV;current++) {
		SendMessage(bar, PBM_STEPIT, 0, 0);
	}
	UpdateWindow(bar);
	UpdateWindow(wnd);
}
void AddProgress(HWND progressBar, int count) {

	current += count;
	if (current < maxV)
		for (int n = 0; n < count; n++) {
			SendMessage(progressBar, PBM_STEPIT, 0, 0);
		}
	else current = maxV;
	UpdateWindow(progressBar);
	UpdateWindow(wnd);
}

int CALLBACK BrowseForFolderCallback(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	TCHAR szPath[MAX_PATH];

	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
		break;

	case BFFM_SELCHANGED:
		if (SHGetPathFromIDList((LPITEMIDLIST)lp, szPath))
		{
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szPath);

		}
		break;
	}

	return 0;
}

void OnClick(int id, HWND hWnd) {
	if (id == FOLDER_VIEW_BTN) {
		BROWSEINFO bi;
		TCHAR* szPath = new TCHAR[MAX_PATH];
		LPITEMIDLIST pidl;
		BOOL bResult = FALSE;

		LPMALLOC pMalloc;

		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			bi.hwndOwner = wnd;
			bi.pidlRoot = NULL;
			bi.pszDisplayName = NULL;
			bi.lpszTitle = L"Title";
			bi.ulFlags = BIF_STATUSTEXT; //BIF_EDITBOX 
			bi.lpfn = BrowseForFolderCallback;
			bi.lParam = (LPARAM)L"C:/";

			pidl = SHBrowseForFolder(&bi);
			if (pidl)
			{
				if (SHGetPathFromIDList(pidl, szPath))
				{
					SetWindowText(path, szPath);
					delete szPath;
				}
				CoTaskMemFree(pidl);
			}
			pMalloc->Release();
			
		}
	}
	if (id == CANCEL) {
		DestroyWindow(wnd);
	}
	if (id == CREATE) {
		RECT rc;
		GetClientRect(wnd, &rc);
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;

		if(projName)
			GetWindowText(projName, prName, MAX_PATH);
		if(path)
			GetWindowText(path, prPath, MAX_PATH);
		if(Author)
			GetWindowText(Author, prAuthor, MAX_PATH);
		if(Description)
			GetWindowText(Description, prDesc, 1024);

		DestroyWindow(projName);
		DestroyWindow(path);
		DestroyWindow(Author);
		DestroyWindow(Description);
		DestroyWindow(close);
		DestroyWindow(createProjact);
		DestroyWindow(ListBox);
		DestroyWindow(BrowseFolder);

		if (title)
			DestroyWindow(title);
		if (progBar)
			DestroyWindow(progBar);


		title = CreateWindow(WC_STATIC, L"Running...", WS_CHILD | WS_VISIBLE, rc.left+10, height / 2 - 35, width - 20, 25, wnd, (HMENU)101, NULL, NULL);
		progBar = CreateWindow(PROGRESS_CLASS, L"Init of projact", WS_BORDER | WS_CHILD | WS_VISIBLE | PBS_SMOOTH, rc.left+10, height / 2, width - 20, 25, wnd, (HMENU)100, NULL, NULL);
		current = 0;
		SendMessage(progBar, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, maxV));
		SendMessage(progBar, PBM_SETSTEP, (WPARAM)1, 0);
		SendMessage(progBar, PBM_SETBARCOLOR, 0, (LPARAM)RGB(0, 190, 0));
		UpdateWindow(progBar);
		UpdateWindow(wnd);

		AddProgress(progBar, 1);

		SetWindowText(title, L"Initing JSON module...");
		AddProgress(progBar, 2);

		if (!LoadResources()) {
			Error(prPath, prName, L"ERR: cannot init JSON module!", id, hWnd);
			return;
		}

		SetWindowText(title, L"Project folder creating...");
		AddProgress(progBar, 2);
		if (!CreateFolder(prPath, prName)) {
			Error(prPath, prName, L"ERR: cannot create project folder!", id, hWnd);
			return;
		}
		SetWindowText(title, L"Create project files...");
		AddProgress(progBar, 2);
		if (!GenerateFiles(prPath, prName, ProjactTypeID)) {
			Error(prPath, prName, L"ERR: project files creating was failed!", id, hWnd);
			return;
		}
		SetWindowText(title, L"File filling...");
		AddProgress(progBar, 15);
		if (!SetProjactParams(prPath,prName, ConvertToString(wstring(prAuthor)).c_str(), ConvertToString(wstring(prDesc)).c_str())) {
			Error(prPath, prName, L"ERR: file write error!", id, hWnd);
			return;
		}
		SetWindowText(title, L"Finishing proccess...");
		AddProgress(progBar, 15);

		TerminateResources();
		SetWindowText(title, L"Finish!");
		FullProgress(progBar);

		folderPath = ConvertToString(wstring(prPath) + L"\\" + prName + L"\0");
		isCreated = true;
		DestroyWindow(wnd);
	}
}


void Sender(HWND window, HWND main){
	wnd = window;

	MSG msg;
	while (GetMessage(&msg,NULL,NULL,NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
		// projact interface creating start
		case WM_CREATE:{
			RECT rc;
			GetClientRect(hWnd, &rc);
			// List box creating 
			ListBox = CreateWindow(WC_COMBOBOX,
				L"Empty projact",
				CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
				rc.left + 10, rc.top + 10,
				rc.right / 2 -10, 1000,
				hWnd,
				(HMENU)WINDOW_LIST,
				NULL,
				NULL
			);
			
			// List box filling
			SendMessage(ListBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Console projact");
			SendMessage(ListBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Graphics projact");
			SendMessage(ListBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Empty projact");
			SendMessage(ListBox, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);

			// User path to projact creating
			path = CreateWindow(WC_EDIT,
				L"C:/",
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL ,
				rc.right / 2 + 10, rc.top + 10,
				rc.right / 2 -50, 25,
				hWnd,
				(HMENU)PROJACT_PATH,
				NULL,
				NULL
			);
			// set max path simbols
			SendMessage(path, EM_SETLIMITTEXT, MAX_PATH, 0);

			// creating btn to folder view
			BrowseFolder = CreateWindow(WC_BUTTON,
				L"...",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				rc.right - 50, 10,
				40, 25,
				hWnd,
				(HMENU)FOLDER_VIEW_BTN,
				NULL,
				NULL
			);
			// creating author name text box
			Author = CreateWindow(WC_EDIT,
				L"Author (optional)",
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				rc.left + 10, rc.top + 40,
				rc.right / 2 - 10, 25,
				hWnd,
				(HMENU)AUTHOR,
				NULL,
				NULL
			);
			SendMessage(Author, EM_SETLIMITTEXT, MAX_PATH, 0);

			// projact name input creating
			projName = CreateWindow(WC_EDIT,
				L"Projact name",
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				rc.right / 2+10, 40,
				rc.right / 2 - 20, 25,
				hWnd,
				(HMENU)PROJACT_NAME,
				NULL,
				NULL
			);
			SendMessage(projName, EM_SETLIMITTEXT, MAX_PATH, 0);

			// Description creating
			Description = CreateWindow(WC_EDIT,
				L"Description (optional)",
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
				10, 70,
				rc.right - 20, rc.bottom / 2 + 30,
				hWnd,
				(HMENU)DESCRIPTION,
				NULL,
				NULL
			);
			SendMessage(projName, EM_SETLIMITTEXT, 1024, 0);
			// cancel btn creating
			close = CreateWindow(
				WC_BUTTON,
				L"Cancel",
				WS_CHILD | WS_VISIBLE,
				10, rc.bottom - 25,
				rc.right / 2 - 10, 20,
				hWnd,
				(HMENU)CANCEL,
				NULL,
				NULL
			);

			// create projact btn creating
			createProjact =  CreateWindow(
				WC_BUTTON,
				L"Create projact",
				WS_CHILD | WS_VISIBLE ,
				rc.right /2 + 10, rc.bottom - 25,
				rc.right / 2 - 20, 20,
				hWnd,
				(HMENU)CREATE,
				NULL,
				NULL
			);
			

		}break;
		// projact interface creating end
		// projact interface handler start
		case WM_COMMAND: {
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				// Get selected item id
				ProjactTypeID = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				std::cout << ProjactTypeID;
			}
			if(HIWORD(wParam) == BN_CLICKED)
				OnClick(LOWORD(wParam), hWnd); 
			
		}break;
		// projact interface handler end

		// destroy window
		case WM_DESTROY: {
			PostQuitMessage(0);
		}break;
	}
    return DefWindowProc(hWnd, message, wParam, lParam);
}