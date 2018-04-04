/**
 * @Author: Lev Vorobjev
 * @Date:   04.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: crt.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 04.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void WINAPI WinMainCRTStartup(void) {
    HINSTANCE hInst = GetModuleHandle(NULL);
    LPSTR lpCmdLine = GetCommandLineA();
    int res = WinMain(hInst, NULL, lpCmdLine, SW_SHOWNORMAL);
    ExitProcess(res);
}

#ifdef __cplusplus
}
#endif
