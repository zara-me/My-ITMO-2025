#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <stdint.h>

#include "main.h"

void initKeyboard(void);
char readKey(void);
char scanKeyboard(void);

extern char lastKey;
extern uint32_t lastScanTime;

#endif