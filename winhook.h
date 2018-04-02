/**
 * winhook.h
 *
 * Программа для расширения возможности клавиатурного ввода
 * Copyright (c) 2018 Lev Vorobjev
 */

#ifndef WINHOOK_H
#define WINHOOK_H

#include <windows.h>

#ifdef BUILD_DLL
   // the dll exports
   #define DLL_EI __declspec(dllexport)
#else
   // the exe imports
   #define DLL_EI __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLL_EI HHOOK SetKeyboardHook(DWORD dwThreadId);
DLL_EI BOOL UnhookKeyboardHook();
DLL_EI LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
// TODO: функции настройки перехвата

#ifdef __cplusplus
}
#endif

#endif
