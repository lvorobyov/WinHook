//
// Created by Lev on 21.05.2019.
//

#include "arrows.h"
#include <stdio.h>

static WORD wArrows[4];
static WORD* ptr, *tmp;
static FILE* log_file;

static
void __store_bytes(void* data, int value, size_t count) {
    asm("rep stosb": "+c" (count), "+D" (data): "a" (value): "cc");
}

static
void CALLBACK ArrowsTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    INPUT inputs[4];
    __store_bytes(inputs, 0, sizeof(INPUT) * 4);
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = ptr[0];
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = ptr[0];
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = ptr[1];
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = ptr[1];
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(4, inputs, sizeof(INPUT));
    fprintf(log_file, "sent");
    WORD *temp = ptr;
    ptr = tmp;
    tmp = temp;
}

DWORD WINAPI StartTimerThreadProc(LPVOID lpParameter) {
    log_file = fopen("winhook.log", "a+");
    timer_ctx_t* ctx = lpParameter;
    if ((ctx->timer1 = SetTimer(NULL, 0, 2800, (TIMERPROC) ArrowsTimerProc)) == 0)
        fprintf(log_file, "error: SetTimer");
    Sleep(1000);
    if ((ctx->timer2 = SetTimer(NULL, 0, 2800, (TIMERPROC) ArrowsTimerProc)) == 0)
        fprintf(log_file, "error: SetTimer");
    fprintf(log_file, "started");
    return 0;
}

void StartTyping(timer_ctx_t *ctx, WORD wKeyCode) {
    for (int i = 0; i < 4; ++i) {
        wArrows[i] = wKeyCode++;
        if (wKeyCode > VK_DOWN)
            wKeyCode = VK_LEFT;
    }
    ptr = &wArrows[0];
    if (ctx->timer1 == 0) {
        CreateThread(NULL, 0, StartTimerThreadProc, (LPVOID)ctx, 0, NULL);
    }
}

void StopTyping(timer_ctx_t *ctx) {
    KillTimer(NULL, ctx->timer1);
    KillTimer(NULL, ctx->timer2);
    fprintf(log_file, "stopped");
    fclose(log_file);
}
