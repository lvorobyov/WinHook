/**
 * winhook.c
 *
 * Программа для расширения возможности клавиатурного ввода
 * Copyright (c) 2018 Lev Vorobjev
 */

#include "winhook.h"
#include "autoscroll.h"

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

#define ST_CONTROL  0x01
#define ST_SHIFT    0x02

DLL_EI LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0)
        return CallNextHookEx(hHook, code, wParam, lParam);

    static BYTE bState = 0;
	static UINT_PTR idAutoscrollTimer = 0;

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
                    if (bState == 0 &&
                        (HIWORD(lParam) & KF_ALTDOWN) != 0) {
                        if ((HIWORD(lParam) & KF_UP) != 0)
                            ShellExecute(NULL,TEXT("open"),
                                TEXT("https://www.multitran.ru/"),NULL,NULL,SW_SHOW);
                        return 1;
                    }
                    break;
				case VK_DOWN:
                    if (bState == 0 &&
                        (HIWORD(lParam) & KF_ALTDOWN) != 0) {
                        if ((HIWORD(lParam) & KF_UP) != 0)
							if (idAutoscrollTimer == 0) {
                            	idAutoscrollTimer = SetAutoscrollTimer(100);
							} else {
								KillAutoscrollTimer(idAutoscrollTimer);
								idAutoscrollTimer = 0;
							}
                        return 1;
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
