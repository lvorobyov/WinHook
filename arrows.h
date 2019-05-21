//
// Created by Lev on 21.05.2019.
//

#ifndef _ARROWS_H_
#define _ARROWS_H_

#include <windows.h>


typedef struct timer_ctx_ {
    HANDLE hThread;
    volatile BOOL active;
} timer_ctx_t;

void StartTyping(timer_ctx_t* ctx, WORD wKeyCode);

void StopTyping(timer_ctx_t* ctx);

#endif //_ARROWS_H_
