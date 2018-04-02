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

DLL_EI LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0) {
        return CallNextHookEx(hHook, code, wParam, lParam);
    }

    switch (code) {
        case HC_ACTION:
            if ((HIWORD(lParam) & KF_ALTDOWN) != 0) {
                if (wParam == '1') {
                    MessageBox(NULL, TEXT("Alt+1"), TEXT("WinHook"), MB_OK);
                }
            }
            break;
        default:
            break;
    }

    return CallNextHookEx(hHook, code, wParam, lParam);
}
