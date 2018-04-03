/**
 * winhook.c
 *
 * Программа для расширения возможности клавиатурного ввода
 * Copyright (c) 2018 Lev Vorobjev
 */

#include "winhook.h"

static HHOOK hHook = NULL;
static HANDLE hInstDll;

BOOL WINAPI DllMainCRTStartup (
        HANDLE  hDllHandle,
        DWORD   dwReason,
        LPVOID  lpreserved) {
    hInstDll = hDllHandle;
    return TRUE;
}

DLL_EI HHOOK SetKeyboardHook(DWORD dwThreadId) {
    hHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC) KeyboardProc,
        hInstDll, dwThreadId);
    return hHook;
}

DLL_EI BOOL UnhookKeyboardHook() {
    BOOL bStatus = UnhookWindowsHookEx(hHook);
    if (bStatus)
        hHook = NULL;
    return bStatus;
}

#define ST_CONTROL  1
#define ST_SHIFT    2

DLL_EI LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0)
        return CallNextHookEx(hHook, code, wParam, lParam);

    static BYTE bState = 0;

    switch (code) {
        case HC_ACTION:
            switch (wParam) {
                case VK_CONTROL:
                    if ((HIWORD(lParam) & KF_UP) != 0) {
                        bState &= ~ST_CONTROL;
                    } else {
                        bState |= ST_CONTROL;
                    }
                    break;
                case VK_SHIFT:
                    if ((HIWORD(lParam) & KF_UP) != 0) {
                        bState &= ~ST_SHIFT;
                    } else {
                        bState |= ST_SHIFT;
                    }
                    break;
                case 'T':
                    if ((HIWORD(lParam) & KF_UP) != 0 &&
                        (bState & ST_SHIFT) != 0 &&
                        (HIWORD(lParam) & KF_ALTDOWN) != 0) {
                        ShellExecute(NULL,TEXT("open"),
                            TEXT("https://www.multitran.ru/"),NULL,NULL,SW_SHOW);
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return CallNextHookEx(hHook, code, wParam, lParam);
}
