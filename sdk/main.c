/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "kb.h"
#include "sdk_uart.h"
#include "pca9538.h"
#include "oled.h"
#include "fonts.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int timer_value = 0;          // Time in seconds
int is_running = 0;           // Timer state
uint32_t last_tick = 0;       // For 1 second delay tracking
int alarm_active = 0;         // Alarm state
uint32_t alarm_start_tick = 0;

// Keymap mapping physical matrix to numbers/actions
// R0: 1, 2, 3
// R1: 4, 5, 6
// R2: 7, 8, 9
// R3: Clear(*), 0, Start(#)
const char keymap[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'C', '0', 'S'}
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Timer_Logic(void);
void Update_Display(void);
char Scan_Keyboard(void);
void Init_Buzzer_Pin(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  oled_Init();
  Init_Buzzer_Pin(); // Manually initialize PE9
  Update_Display();
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Timer_Logic();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void Init_Buzzer_Pin(void) {
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
}

void Update_Display(void) {
    char buffer[32];
    oled_Fill(Black);
    oled_SetCursor(0, 0);
    oled_WriteString("Variant 2: Timer", Font_7x10, White);

    oled_SetCursor(0, 20);
    if(alarm_active) {
         oled_WriteString("== TIME UP ==", Font_11x18, White);
    } else {
         sprintf(buffer, "Time: %04d s", timer_value);
         oled_WriteString(buffer, Font_11x18, White);
    }

    oled_SetCursor(0, 50);
    if(is_running) {
        oled_WriteString("Status: RUNNING", Font_7x10, White);
    } else {
        oled_WriteString("Status: PAUSED", Font_7x10, White);
    }
    oled_UpdateScreen();
}

char Scan_Keyboard(void) {
    uint8_t Row[4] = {ROW1, ROW2, ROW3, ROW4};
    uint8_t Key;

    for (int i = 0; i < 4; i++) {
        Key = Check_Row(Row[i]);
        if (Key != 0) {
            HAL_Delay(50); // Debounce
            if (Check_Row(Row[i]) == Key) {
                while(Check_Row(Row[i]) != 0); // Wait for release
                if (Key == 0x04) return keymap[i][0]; // Left
                if (Key == 0x02) return keymap[i][1]; // Center
                if (Key == 0x01) return keymap[i][2]; // Right
            }
        }
    }
    return 0;
}

void Timer_Logic(void) {
    char key = Scan_Keyboard();

    // Handle Input
    if (key != 0) {
        if (key >= '0' && key <= '9' && !is_running) {
            if(timer_value < 999) { // Max 9999 seconds
                timer_value = (timer_value * 10) + (key - '0');
                Update_Display();
            }
        } else if (key == 'S') { // Start/Pause
            if(timer_value > 0) {
                is_running = !is_running;
                if(is_running) last_tick = HAL_GetTick();
                Update_Display();
            }
        } else if (key == 'C') { // Clear / Reset Alarm
            is_running = 0;
            timer_value = 0;
            alarm_active = 0;
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET); // Turn off buzzer
            Update_Display();
        }
    }

    // Handle Countdown
    if (is_running) {
        if (HAL_GetTick() - last_tick >= 1000) {
            last_tick += 1000;
            timer_value--;
            Update_Display();

            if (timer_value == 0) {
                is_running = 0;
                alarm_active = 1;
                alarm_start_tick = HAL_GetTick();
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET); // Turn ON buzzer
                Update_Display();
            }
        }
    }

    // Handle Alarm duration (3 seconds)
    if(alarm_active) {
        if(HAL_GetTick() - alarm_start_tick > 3000) {
            alarm_active = 0;
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET); // Turn OFF buzzer
            Update_Display();
        }
    }
}
/* USER CODE END 4 */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
