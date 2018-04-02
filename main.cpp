/**
 * main.cpp
 * Программа для расширения возможности клавиатурного ввода
 * Copyright (c) 2018 Lev Vorobjev
 */

#include <windows.h>
#include <shellapi.h>
#include <string.h>
#include <tchar.h>
#include "resource.h"

#define IDN_ICON     40050
#define WM_SHALLICON (WM_APP + 1)

#define TRAY_TITLE TEXT("Расширения клавиатурного ввода")
#define TRAY_MENU_NAME NULL
#define MSG_TITLE TEXT("WinHook")
#define BUFFER_SIZE 512

static
void WinErrorHandle(HWND hWnd, LPTSTR lpszBuffer, LPCTSTR lpszFunctionName, DWORD dwStatus) {
    _stprintf(lpszBuffer, TEXT("%s error.\nStatus code: %d"),
        lpszFunctionName, dwStatus);
    MessageBox(hWnd, lpszBuffer, MSG_TITLE, MB_OK | MB_ICONWARNING);
    PostQuitMessage(0);
}

#define HANDLE_ERROR(lpszFunctionName, dwStatus) \
{   WinErrorHandle(hWnd, lpszBuffer, TEXT(lpszFunctionName), dwStatus); \
    break;  \
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

int APIENTRY WinMain(HINSTANCE hInstance,
             HINSTANCE         hPrevInstance,
             LPSTR             lpCmdLine,
             int               nCmdShow) {
    LPCTSTR lpszClass = TEXT("WinHook_Window");
    LPCTSTR lpszTitle = TRAY_TITLE;
    HWND hWnd;
    MSG msg = {0};
    BOOL status;

    if (!RegMyWindowClass(hInstance, lpszClass))
        return 1;

    hWnd = CreateWindowEx(0, lpszClass, lpszTitle, 0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        HWND_MESSAGE, NULL, NULL, NULL);
    if(!hWnd) return 2;

    while ((status = GetMessage(&msg, NULL, 0, 0 )) != 0) {
        if (status == -1) return 3;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

#ifdef __cplusplus
extern "C" {
#endif

void WINAPI WinMainCRTStartup(void) {
    HINSTANCE hInst = GetModuleHandle(NULL);
    int res = WinMain(hInst, NULL, NULL, 0);
    ExitProcess(res);
}

#ifdef __cplusplus
}
#endif

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpszClassName) {
    WNDCLASS wcWindowClass = {0};
    wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
    wcWindowClass.hInstance = hInst;
    wcWindowClass.lpszClassName = lpszClassName;
    return RegisterClass(&wcWindowClass);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam) {
    static HINSTANCE hInst;
	static LPTSTR lpszBuffer;
	DWORD dwStatus;

    static NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };

	switch (message) {
      case WM_CREATE:
		lpszBuffer = (LPTSTR)calloc(BUFFER_SIZE,sizeof(TCHAR));
        hInst = (HINSTANCE) GetModuleHandle(NULL);
        if (! hInst)
            HANDLE_ERROR("GetModuleHandle",GetLastError());
        // Notify initialization
        nid.hWnd = hWnd;
        nid.uID = IDN_ICON;
        nid.uCallbackMessage = WM_SHALLICON;
        nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
        if (! nid.hIcon)
            HANDLE_ERROR("LoadIcon",GetLastError());
        _tcsncpy(nid.szTip, TRAY_TITLE, 64);
        nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
        Shell_NotifyIcon(NIM_ADD, &nid);
		break;
      case WM_SHALLICON:
        if (wParam == IDN_ICON) {
            if ((UINT)lParam == WM_RBUTTONDOWN) {
                HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_TRAYMENU));
                if (! hMenu)
                    HANDLE_ERROR("LoadMenu",GetLastError());
                HMENU hPopup = GetSubMenu(hMenu, 0);
                if (! hPopup)
                    HANDLE_ERROR("GetSubMenu",GetLastError());
                SetForegroundWindow(hWnd);
                POINT pt;
                GetCursorPos(&pt);
                TrackPopupMenu(hPopup, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
                DestroyMenu(hMenu);
            }
        }
        break;
	  case WM_COMMAND:
        switch (LOWORD(wParam)) {
          case IDM_ITEMQUIT:
            DestroyWindow(hWnd);
            break;
		}
		break;
	  case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid);
        DestroyIcon(nid.hIcon);
        free(lpszBuffer);
		PostQuitMessage(0);
		break;
	  default:
        return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
