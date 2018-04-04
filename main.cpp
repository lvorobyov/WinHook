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
#include "winhook.h"

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

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpszClassName) {
    WNDCLASS wcWindowClass = {0};
    wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
    wcWindowClass.hInstance = hInst;
    wcWindowClass.lpszClassName = lpszClassName;
    return RegisterClass(&wcWindowClass);
}

#define ENABLE_COMMAND(idCommand) \
    EnableMenuItem(GetSubMenu(hMenu, 0), \
        idCommand, MF_BYCOMMAND | MF_ENABLED);

#define DISABLE_COMMAND(idCommand) \
    EnableMenuItem(GetSubMenu(hMenu, 0), \
        idCommand, MF_BYCOMMAND | MF_GRAYED);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam) {
    static HINSTANCE hInst;
	static LPTSTR lpszBuffer;
	DWORD dwStatus;

    static NOTIFYICONDATA nid = { sizeof(NOTIFYICONDATA) };
    static HMENU hMenu;
    static BOOL isHook = FALSE;

	switch (message) {
      case WM_CREATE:
		lpszBuffer = (LPTSTR)calloc(BUFFER_SIZE,sizeof(TCHAR));
        hInst = (HINSTANCE) GetModuleHandle(NULL);
        if (! hInst)
            HANDLE_ERROR("GetModuleHandle",GetLastError());
        hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_TRAYMENU));
        if (! hMenu)
            HANDLE_ERROR("LoadMenu",GetLastError());
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
                HMENU hPopup = GetSubMenu(hMenu, 0);
                if (! hPopup)
                    HANDLE_ERROR("GetSubMenu",GetLastError());
                SetForegroundWindow(hWnd);
                POINT pt;
                GetCursorPos(&pt);
                TrackPopupMenu(hPopup, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
            }
        }
        break;
	  case WM_COMMAND:
        switch (LOWORD(wParam)) {
          case IDM_ITEMQUIT:
            DestroyWindow(hWnd);
            break;
          case IDM_ITEMATTACH:
            // TODO: Search threadId
            if (! SetKeyboardHook(NULL))
                HANDLE_ERROR("SetWindowsHookEx",GetLastError());
            isHook = TRUE;
            ENABLE_COMMAND(IDM_ITEMDETACH);
            DISABLE_COMMAND(IDM_ITEMATTACH);
            break;
          case IDM_ITEMDETACH:
            if (! UnhookKeyboardHook())
                HANDLE_ERROR("UnhookWindowsHookEx",GetLastError());
            isHook = FALSE;
            ENABLE_COMMAND(IDM_ITEMATTACH);
            DISABLE_COMMAND(IDM_ITEMDETACH);
            break;
          case IDM_ITEMSETTINGS:
            // TODO: Show settings
            break;
		}
		break;
	  case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid);
        if (isHook)
            UnhookKeyboardHook();
        DestroyIcon(nid.hIcon);
        DestroyMenu(hMenu);
        free(lpszBuffer);
		PostQuitMessage(0);
		break;
	  default:
        return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
