//
// Created by Lev on 21.05.2019.
//

#include "arrows.h"

static WORD wArrows[4];
static HANDLE log_file;

static
DWORD __scan_bytes(void* data, int value) {
    DWORD count = 80;
    asm("repne scasb": "+c"(count), "+D"(data): "a"(value): "cc");
    return 80 - count;
}

static
void write_log(const char* msg) {
    DWORD written;
    DWORD count = __scan_bytes((LPVOID)msg, 0) - 1;
    WriteFile(log_file, msg, count, &written, NULL);
}

static
void __store_bytes(void* data, int value, size_t count) {
    asm("rep stosb": "+c" (count), "+D" (data): "a" (value): "cc");
}

static
void PressKeyboardKey(WORD wKeyCode) {
    INPUT inputs[2];
    __store_bytes(inputs, 0, sizeof(INPUT) * 2);
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = wKeyCode;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = wKeyCode;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(2, inputs, sizeof(INPUT));
}

DWORD WINAPI StartTimerThreadProc(LPVOID lpParameter) {
    log_file = CreateFileA("winhook.log", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
    timer_ctx_t* ctx = lpParameter;
    DWORD dwTimeouts[4] = { 400, 2000, 400, 3700 };
    int index = 2;
    write_log("started\n");
    do {
        Sleep(dwTimeouts[index]);
        PressKeyboardKey(wArrows[index]);
        if (index -- < 0)
            index = 3;
    } while (ctx->active);
    return 0;
}

void StartTyping(timer_ctx_t *ctx, WORD wKeyCode) {
    for (int i = 0; i < 4; ++i) {
        wArrows[i] = wKeyCode++;
        if (wKeyCode > VK_DOWN)
            wKeyCode = VK_LEFT;
    }
    if (ctx->active == FALSE) {
        ctx->active = TRUE;
        DWORD thread_id;
        if (NULL == (ctx->hThread = CreateThread(NULL, 0, StartTimerThreadProc, (LPVOID)ctx, 0, &thread_id)))
            write_log("error: CreateThread");
    }
}

void StopTyping(timer_ctx_t *ctx) {
    ctx->active = FALSE;
    WaitForSingleObject(ctx->hThread, INFINITE);
    write_log("stopped\n");
    CloseHandle(log_file);
}
