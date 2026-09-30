#include "tm1637.h"

const uint8_t tm1637_digit_codes[] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

void delay_us(uint32_t us) {
  volatile uint32_t cycles = us * 24;
  while (cycles-- > 0) {
    __asm__("nop");
  }
}

void tm1637_init(void) {
  GPIOA->MODER = (GPIOA->MODER & ~(0xF << (TM1637_CLK_PIN * 2))) | (0x5 << (TM1637_CLK_PIN * 2));
  GPIOA->OTYPER |= (1 << TM1637_CLK_PIN) | (1 << TM1637_DIO_PIN);
  GPIOA->OSPEEDR |= (0x3 << (TM1637_CLK_PIN * 2)) | (0x3 << (TM1637_DIO_PIN * 2));
  GPIOA->BSRR = (1 << TM1637_CLK_PIN) | (1 << TM1637_DIO_PIN);

  tm1637_start();
  tm1637_write_byte(TM1637_CMD1);
  tm1637_stop();
  tm1637_start();
  tm1637_write_byte(TM1637_CMD3);
  tm1637_stop();
  tm1637_clear();
}

void tm1637_start(void) {
  GPIOA->BSRR = (1 << TM1637_DIO_PIN) | (1 << TM1637_CLK_PIN);  
  delay_us(2);
  GPIOA->BRR = (1 << TM1637_DIO_PIN);  
  delay_us(2);
  GPIOA->BRR = (1 << TM1637_CLK_PIN);  
  delay_us(2);
}

void tm1637_stop(void) {
  GPIOA->BRR = (1 << TM1637_CLK_PIN);  
  delay_us(2);
  GPIOA->BRR = (1 << TM1637_DIO_PIN);  
  delay_us(2);
  GPIOA->BSRR = (1 << TM1637_CLK_PIN);  
  delay_us(2);
  GPIOA->BSRR = (1 << TM1637_DIO_PIN);  
  delay_us(2);
}

void tm1637_write_byte(uint8_t byte) {
  for (uint8_t i = 0; i < 8; i++) {
    GPIOA->BRR = (1 << TM1637_CLK_PIN);  
    delay_us(2);
    if (byte & 0x01) {
      GPIOA->BSRR = (1 << TM1637_DIO_PIN); 
    } else {
      GPIOA->BRR = (1 << TM1637_DIO_PIN);  
    }
    delay_us(2);
    GPIOA->BSRR = (1 << TM1637_CLK_PIN);  
    delay_us(2);
    byte >>= 1;
  }
  GPIOA->BRR = (1 << TM1637_CLK_PIN);  
  GPIOA->BSRR = (1 << TM1637_DIO_PIN);  
  delay_us(2);
  GPIOA->BSRR = (1 << TM1637_CLK_PIN);  
  delay_us(2);
  GPIOA->BRR = (1 << TM1637_CLK_PIN);  
  delay_us(2);
}

void tm1637_display_raw(uint8_t pos, uint8_t data) {
  tm1637_start();
  tm1637_write_byte(TM1637_CMD2 | pos); 
  tm1637_write_byte(data);               
  tm1637_stop();
  tm1637_start();
  tm1637_write_byte(TM1637_CMD3);
  tm1637_stop();
}

void tm1637_display_digit_char(uint8_t pos, uint8_t num) {
  if (num < 16) {
      tm1637_display_raw(pos, tm1637_digit_codes[num]);
  }
}

void tm1637_clear(void) {
  for (uint8_t i = 0; i < 4; i++) {
    tm1637_display_raw(i, 0x00);
  }
}