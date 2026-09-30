#include "main.h"
#include "tm1637.h"
#include "keyboard.h"

volatile uint32_t tickCount;
uint32_t lastScanTime = 0;
char lastKey = '\0';

char current_pin[5] = "1234";
char entered_pin[5] = "    ";
uint8_t pin_index = 0;
uint8_t is_unlocked = 0;      

void osSystickHandler(void) {
  tickCount++;
}

void initGPIO() {
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
  GPIOA->MODER = (GPIOA->MODER & ~(3U << (15 * 2))) | (1U << (15 * 2));
  GPIOA->MODER = (GPIOA->MODER & ~(3U << (9 * 2))) | (1U << (9 * 2));
  GPIOA->OTYPER &= ~((1 << 15) | (1 << 9));
  GPIOA->OSPEEDR |= (1U << (15 * 2)) | (1U << (9 * 2));
  GPIOA->BRR = (1 << 15) | (1 << 9);
}

void initUSART2() {
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  GPIOA->MODER = (GPIOA->MODER & ~(0xF << 4)) | (0xA << 4);
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~(0xFF << 8)) | (1 << 8) | (1 << 12);
  USART2->BRR = 417; 
  USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void initSysTick() {
  SysTick->LOAD = 47999; 
  SysTick->VAL = 0;
  SysTick->CTRL = (1 << 2) | (1 << 1) | (1 << 0);
}

int _write(int file, uint8_t *ptr, int len) {
  for (int i = 0; i < len; i++) {
    while (!(USART2->ISR & USART_ISR_TXE));
    USART2->TDR = ptr[i];
  }
  return len;
}

void delay_ms(uint32_t ms) {
    uint32_t start = tickCount;
    while ((tickCount - start) < ms);
}

void update_display_code() {
    tm1637_clear();
    for(int i = 0; i < 4; i++) {
        if(entered_pin[i] >= '0' && entered_pin[i] <= '9') {
            tm1637_display_digit_char(3 - i, entered_pin[i] - '0'); 
        } else if (entered_pin[i] == '-') {
             tm1637_display_raw(3 - i, 0x40); 
        }
    }
}

void reset_input() {
    pin_index = 0;
    for(int i = 0; i < 4; i++) entered_pin[i] = ' ';
    update_display_code();
}

int main(void) {
  initGPIO();
  initUSART2();
  initSysTick();
  initKeyboard();
  tm1637_init();

  printf("System Started - Locked\n");
  reset_input();

  while (1) {
    if (tickCount - lastScanTime > 100) {
      lastScanTime = tickCount;
      char currentKey = readKey();

      if (currentKey != '\0' && currentKey != lastKey) {
        printf("Pressed: %c\n", currentKey);
        
        if (currentKey == '#') {
            if (is_unlocked == 0) {
                // سیستم قفل است -> بررسی رمز
                uint8_t correct = 1;
                for(int i = 0; i < 4; i++) {
                    if (entered_pin[i] != current_pin[i]) correct = 0;
                }
                
                if (correct) {
                    printf("Access Granted! Enter new PIN and press #, or * to lock.\n");
                    is_unlocked = 1;
                    GPIOA->BSRR = (1 << 9);  
                    GPIOA->BRR = (1 << 15); 
                    
                    for(int i = 0; i < 4; i++) entered_pin[i] = '-';
                    update_display_code();
                    delay_ms(1000);
                    reset_input();
                } else {
                    printf("Access Denied!\n");
                    GPIOA->BSRR = (1 << 15); 
                    delay_ms(2000);
                    GPIOA->BRR = (1 << 15); 
                    reset_input();
                }
            } else {
                
                if (pin_index == 4) {
                    for(int i = 0; i < 4; i++) {
                        current_pin[i] = entered_pin[i];
                    }
                    printf("PIN Changed successfully!\n");
                    
                  
                    for (int i=0; i<3; i++) {
                        GPIOA->BRR = (1 << 9); delay_ms(200);
                        GPIOA->BSRR = (1 << 9); delay_ms(200);
                    }
                    reset_input();
                } else {
                    printf("Error: New PIN must be 4 digits!\n");
                    GPIOA->BSRR = (1 << 15); delay_ms(500); GPIOA->BRR = (1 << 15); // چشمک قرمز خطا
                    reset_input();
                }
            }
            
        } else if (currentKey == '*') {
            if (is_unlocked == 1) {
                printf("System Locked!\n");
                is_unlocked = 0;
                GPIOA->BRR = (1 << 9); 
            }
            reset_input();
            
        } else if (currentKey >= '0' && currentKey <= '9') {
            if (pin_index < 4) {
                entered_pin[pin_index] = currentKey;
                pin_index++;
                update_display_code();
            }
        }
        lastKey = currentKey;
      } else if (currentKey == '\0') {
        lastKey = '\0';
      }
    }
  }
  return 0;
}