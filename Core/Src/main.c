/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SLAVE_ADDRESS_LCD (0x27 << 1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void lcd_send_cmd(char cmd);
void lcd_init(void);
uint8_t calculate_number(char zehner, char einer);
void write_char_4bit_mode(uint8_t destination[], uint16_t index, char data);
void write_cmd_4bit_mode(uint8_t destination[], uint16_t index, char data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// RTC
RTC_TimeTypeDef time_rtc;
RTC_DateTypeDef date_rtc;

// Am Anfang initialisieren!
char data = '\0';
char str_date[16] = {'\0'};
char datetime_string[22] = {'\0'};

// 5 times 2 digits, 6 spacing characters times 4 and reset command
uint8_t display_output[68];
HAL_StatusTypeDef lcd_write_result = 0;

uint8_t i = 0;
uint8_t schaltjahr = 0; // = Schaltjahr

// Terminal Handling Variables
char space[] = "\rTT:MM - HH:MM:SS\r";
char IllegalFormat[] = "\rFalsche Eingabe!";
uint8_t error = 0; // False
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

  // sets the cursor to the home position
  write_cmd_4bit_mode(display_output, 64, 0x02);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  lcd_init();

  // Terminal Initialisieren
  HAL_UART_Transmit(&huart3, (uint8_t *)&space, strlen(space), 1000);
  HAL_UART_Receive_IT(&huart3, (uint8_t *)&data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
   */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
   */
  sTime.Hours = 23;
  sTime.Minutes = 59;
  sTime.Seconds = 45;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 31;
  sDate.Year = 1;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
   */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
}

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.battery_charging_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // Eingabe wurde getätigt
  if (data != '\0')
  {
    HAL_UART_Transmit(&huart3, (uint8_t *)&data, 1, 100);

    str_date[i] = data;

    // Format überprüfen
    if (((i == 2 || i == 10 || i == 13) && data != ':') // Doppelpunkt
        || ((i == 5 || i == 7) && data != ' ')          // Leerzeichen
        || (i == 6 && data != '-'))                     // Bindestrich
    {
      error = 1;
    }
    else
    {
      // Überprüfung der Eingabe auf richtige Zahlen
      switch (i)
      {
      // Tage
      case 0: // Erste Stelle maximal 3
        if (str_date[i] < '0' || str_date[i] > '3')
        {
          error = 1;
        }
        break;

      case 1:
        if ((str_date[i] < '1' || str_date[i] > '9') || (str_date[i - 1] == '3' && str_date[i] > '1')) // >31
        {
          error = 1;
        }
        break;

        // Monate
      case 3: // Erste Stelle maximal 1
        if (str_date[i] < '0' || str_date[i] > '1')
        {
          error = 1;
        }
        break;

      case 4:
        if ((str_date[i] < '1' || str_date[i] > '9') || (str_date[i - 1] == '1' && str_date[i] > '2')) // >12
        {
          error = 1;
        }
        break;

      // Stunden
      case 8: // Erste Stelle maximal 2
        if (str_date[i] < '0' || str_date[i] > '2')
        {
          error = 1;
        }
        break;

      case 9:
        if ((str_date[i] < '0' || str_date[i] > '9') || (str_date[i - 1] == '2' && str_date[i] > '3')) // >23
        {
          error = 1;
        }
        break;

        // Handling Minuten & Sekunden
      case 11:
      case 14: // Erste Stelle maximal 5
        if (str_date[i] < '0' || str_date[i] > '5')
        {
          error = 1;
        }
        break;

      case 12:
      case 15:
        if (str_date[i] < '0' || str_date[i] > '9') // > 59
        {
          error = 1;
        }
        break;

      default: // default ist 2, 5, 6, 7, 10, 13 (Zeichen)
        break;
      }
    }
    // i wird hier ständig um 1 erhöht, wenn eine Eingabe kommt
    i++;

    // Reset & Error-Handling
    if (i > 15 || error > 0)
    {

      // Error Message:
      if (error > 0)
      {
        HAL_UART_Transmit(&huart3, (uint8_t *)&IllegalFormat, strlen(IllegalFormat), 1000);
        HAL_Delay(2000);
      }

      // Reset Variable
      i = 0;
      // Reset Terminal
      HAL_UART_Transmit(&huart3, (uint8_t *)&space, strlen(space), 1000);
      if (error < 1)
      {
        // Set RTC
        date_rtc.Date = calculate_number(str_date[0], str_date[1]);
        date_rtc.Month = calculate_number(str_date[3], str_date[4]);
        // Immer Schaltjahr:
        date_rtc.Year = schaltjahr;

        time_rtc.Hours = calculate_number(str_date[8], str_date[9]);
        time_rtc.Minutes = calculate_number(str_date[11], str_date[12]);
        time_rtc.Seconds = calculate_number(str_date[14], str_date[15]);

        if (date_rtc.Month == 2 && date_rtc.Date > 29)
          date_rtc.Date = 29;
        else if ((date_rtc.Month == 4 || date_rtc.Month == 6 || date_rtc.Month == 9 || date_rtc.Month == 11) && date_rtc.Date > 30)
        {
          date_rtc.Date = 30;
        }

        HAL_RTC_SetDate(&hrtc, &date_rtc, RTC_FORMAT_BIN);
        HAL_RTC_SetTime(&hrtc, &time_rtc, RTC_FORMAT_BIN);
      }
      else
      { // Error resetten
        error = 0;
      }
    }
    data = '\0';
  }
  // Reaktivieren: Interrupt
  HAL_UART_Receive_IT(&huart3, (uint8_t *)&data, 1);
}

/*
 * Interrupt handler of the real time clock alarm.
 * Is set to fire every second to update the display.
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *_hrtc)
{
  // Get RTC values
  HAL_RTC_GetTime(&hrtc, &time_rtc, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &date_rtc, RTC_FORMAT_BIN);

  // Immer Schaltjahr
  if (date_rtc.Year != schaltjahr)
    date_rtc.Year = schaltjahr;

  sprintf(datetime_string, "%02u:%02u - %02u:%02u:%02u", date_rtc.Date, date_rtc.Month, time_rtc.Hours, time_rtc.Minutes, time_rtc.Seconds);

  // convert data from datetime_LCD to display_time for LCD 4bit mode
  for (uint16_t j = 0; j < 16; j++)
  {
    write_char_4bit_mode(display_output, (j << 2), datetime_string[j]);
  }

  // send 4 times the data of datetime_LCD because of LCD 4bit mode -> 64 + 4 bit cursor reset cmd
  HAL_I2C_Master_Transmit_IT(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *)display_output, 68);

  // resets the alarm to trigger the interrupt in the next second again
  RTC_AlarmTypeDef rtcInterruptAlarm = {0};
  rtcInterruptAlarm.AlarmTime.Hours = 0;
  rtcInterruptAlarm.AlarmTime.Minutes = 0;
  rtcInterruptAlarm.AlarmTime.Seconds = 0;
  rtcInterruptAlarm.AlarmTime.SubSeconds = 0;
  rtcInterruptAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  rtcInterruptAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  rtcInterruptAlarm.AlarmMask = RTC_ALARMMASK_ALL;
  rtcInterruptAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  rtcInterruptAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  rtcInterruptAlarm.AlarmDateWeekDay = 0;
  rtcInterruptAlarm.Alarm = RTC_ALARM_A;
  HAL_RTC_SetAlarm_IT(&hrtc, &rtcInterruptAlarm, RTC_FORMAT_BIN);
}

/*
 * Triggers when an I²C communication failed.
 * Turns on the red status indicator led.
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, SET);
}

/*
 * Converts one (ascii) byte to I²C display instructions
 */
void write_char_4bit_mode(uint8_t destination[], uint16_t index, char data)
{
  char data_u, data_l;
  data_u = (data & 0xF0);
  data_l = ((data << 4) & 0xF0);
  destination[index] = data_u | 0x0D;     // en=1, rs=1
  destination[index + 1] = data_u | 0x09; // en=0, rs=1
  destination[index + 2] = data_l | 0x0D; // en=1, rs=1
  destination[index + 3] = data_l | 0x09; // en=0, rs=1
}

/*
Converts one byte command to I²C display instructions
*/
void write_cmd_4bit_mode(uint8_t destination[], uint16_t index, char data)
{
  char data_u, data_l;
  data_u = (data & 0xF0);
  data_l = ((data << 4) & 0xF0);
  destination[index] = data_u | 0x0C;   // en=1, rs=0
  destination[++index] = data_u | 0x08; // en=0, rs=0
  destination[++index] = data_l | 0x0C; // en=1, rs=0
  destination[++index] = data_l | 0x08; // en=0, rs=0
}

void lcd_init(void)
{
  // 4 bit initialisation
  HAL_Delay(50); // wait for >40ms
  lcd_send_cmd(0x30);
  HAL_Delay(5); // wait for >4.1ms
  lcd_send_cmd(0x30);
  HAL_Delay(1); // wait for >100us
  lcd_send_cmd(0x30);
  HAL_Delay(10);
  lcd_send_cmd(0x20); // 4bit mode
  HAL_Delay(10);

  // dislay initialisation
  lcd_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
                      //           00101000
                      //           001DNFxx
  HAL_Delay(1);
  lcd_send_cmd(0x08); // Display on/off control --> D=0,C=0, B=0  ---> display off
  HAL_Delay(1);
  lcd_send_cmd(0x01); // clear display
  HAL_Delay(2);
  lcd_send_cmd(0x06); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
  HAL_Delay(1);
  lcd_send_cmd(0x0C); // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_cmd(char cmd)
{
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  data_t[0] = data_u | 0x0C; // en=1, rs=0
  data_t[1] = data_u | 0x08; // en=0, rs=0
  data_t[2] = data_l | 0x0C; // en=1, rs=0
  data_t[3] = data_l | 0x08; // en=0, rs=0
  HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *)data_t, 4, 100);
}

uint8_t calculate_number(char zehner, char einer)
{
  uint8_t zehn = zehner - 48;
  uint8_t ein = einer - 48;

  return (zehn * 10) + ein;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
