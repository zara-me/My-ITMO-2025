#ifndef TM1637_H
#define TM1637_H

#include "main.h"

#define TM1637_CLK_PIN 6
#define TM1637_DIO_PIN 7

#define TM1637_CMD1 0x40
#define TM1637_CMD2 0xC0
#define TM1637_CMD3 0x8F

void tm1637_init(void);
void tm1637_start(void);
void tm1637_stop(void);
void tm1637_write_byte(uint8_t byte);
void tm1637_display_raw(uint8_t pos, uint8_t data);
void tm1637_display_digit_char(uint8_t pos, uint8_t num);
void tm1637_clear(void);

#endif