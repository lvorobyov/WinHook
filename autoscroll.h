/**
 * autoscroll.h
 *
 * Программа для расширения возможности клавиатурного ввода
 * Copyright (c) 2018 Lev Vorobjev
 */

#ifndef AUTOSCROLL_H
#define AUTOSCROLL_H

#include <windows.h>

UINT_PTR SetAutoscrollTimer(UINT uElapse);
BOOL KillAutoscrollTimer(UINT_PTR uIDEvent);

#endif
