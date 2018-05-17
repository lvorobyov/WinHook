/**
 * autoscroll.c
 *
 * Программа для расширения возможности клавиатурного ввода
 * Copyright (c) 2018 Lev Vorobjev
 */

#include "autoscroll.h"

static
void CALLBACK AutoScrollTimerProc(
	HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	INPUT input = {
		INPUT_MOUSE,
		{0, 0, -1, MOUSEEVENTF_WHEEL, 0}
	};
	SendInput(1, &input, sizeof(input));
}

UINT_PTR SetAutoscrollTimer(UINT uElapse) {
	return SetTimer(NULL, 0, uElapse, (TIMERPROC) AutoScrollTimerProc);
}

BOOL KillAutoscrollTimer(UINT_PTR uIDEvent) {
	return KillTimer(NULL, uIDEvent);
}
