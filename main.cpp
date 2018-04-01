/**
 * @Author: Lev Vorobjev
 * @Date:   01.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: main.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 01.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include <windows.h>

#define MSG_TITLE TEXT("WinHook")
#define BUFFER_SIZE 512

#define HANDLE_ERROR(lpszFunctionName, dwStatus) \
    MultiByteToWideChar(CP_ACP, 0, \
        lpszFunctionName, -1, lpszBuffer, BUFFER_SIZE); \
    _stprintf(lpszBuffer, TEXT("%s error.\nStatus code: %d"), \
        lpszBuffer, dwStatus); \
    MessageBox(hWnd, lpszBuffer, MSG_TITLE, MB_OK | MB_ICONWARNING);

int APIENTRY WinMain(HINSTANCE hInstance,
             HINSTANCE         hPrevInstance,
             LPSTR             lpCmdLine,
             int               nCmdShow) {
    HWND hWnd;
    MSG msg = {0};
    BOOL status;

    while ((status = GetMessage(&msg, NULL, 0, 0 )) != 0) {
        if (status == -1) return 3;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
