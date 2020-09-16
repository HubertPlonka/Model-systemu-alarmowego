#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"


int Stan;
int Sens1, Sens2, Sens3;

void Status()  //tryb pracy systemu
{
	if((HAL_GPIO_ReadPin(GPIOB, Garden_Pin) == 1) && (HAL_GPIO_ReadPin(GPIOB, House_Pin) == 1))
	{
			Stan = 3;
	}
	if((HAL_GPIO_ReadPin(GPIOB, Garden_Pin) == 0) && (HAL_GPIO_ReadPin(GPIOB, House_Pin) == 1))
	{
			Stan = 2;
	}
	if((HAL_GPIO_ReadPin(GPIOB, Garden_Pin) == 1) && (HAL_GPIO_ReadPin(GPIOB, House_Pin) == 0))
	{
			Stan = 1;
	}
}



void Alarm()     //sygnalizacja zagrożenia
{
	HAL_GPIO_WritePin(GPIOB, Alarm_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_SET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, Alarm_Pin, GPIO_PIN_RESET);
}



void Odczyt()       //odczyt z aktywnych sensorów
{
switch (Stan) {     
    case 1:  
			HAL_ADC_Start(&hadc1);
		  if( HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK)
		  {
			Sens1 = HAL_ADC_GetValue(&hadc1); 
	    }
			HAL_ADC_Start(&hadc3);
		  if( HAL_ADC_PollForConversion(&hadc3, 1) == HAL_OK)
		  {
			Sens2 = HAL_ADC_GetValue(&hadc3);  
	    }
			if((Sens1 >= 4000) || (Sens2 >= 1500))
			{
				Alarm();
			}
      break;
    case 2:     
			HAL_ADC_Start(&hadc2);
		  if( HAL_ADC_PollForConversion(&hadc2, 1) == HAL_OK)
		  {
			Sens3 = HAL_ADC_GetValue(&hadc2);  
	    }
			if((Sens3 >= 4000))
			{
				Alarm();
			}
      break;
    case 3:   
			HAL_ADC_Start(&hadc1);
		  if( HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK)
		  {
			Sens1 = HAL_ADC_GetValue(&hadc1);  
	    }
			HAL_ADC_Start(&hadc3);
		  if( HAL_ADC_PollForConversion(&hadc3, 1) == HAL_OK)
		  {
			Sens2 = HAL_ADC_GetValue(&hadc3);  
	    }
			HAL_ADC_Start(&hadc2);
		  if( HAL_ADC_PollForConversion(&hadc2, 1) == HAL_OK)
		  {
			Sens1 = HAL_ADC_GetValue(&hadc2);  
	    }
			if((Sens1 >= 4000) || (Sens2 >= 1500) || (Sens3 >= 4000))
			{
				Alarm();
			}
      break;
  };
};

void SystemClock_Config(void);

int main(void)
{
 
  HAL_Init();


  SystemClock_Config();


  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();

    while (1)
  {
		Status();
    Odczyt();

  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

 
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif 
