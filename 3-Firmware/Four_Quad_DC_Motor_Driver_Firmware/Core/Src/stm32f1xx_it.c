/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

float pre_prop_error = 0;
float int_error = 0;
float I_start = 0;
float start_count = 0;

// Define PID parameters
extern uint32_t Kp;
extern uint32_t Kd;
extern uint32_t Ki;
extern float delta_t; // Control loop period, must be equal to TIM4 interrupt frequency (72KHz)

// Define the variables to store input values
extern float I_sense;	// Measured armature current
extern float I_set;  // Set value for armature current
float I_sense_av;  // Measured armature current average
extern float I_set_av ; // Set value moving average
float I_sense_temp; // Defined for calculation purposes
extern float I_set_temp; // Defined for calculation purposes
extern uint32_t Gen_mode; // Generator mode boolean (active low)
extern uint32_t Soft_st;  // Latching soft start
extern uint32_t Soft_st_done; // Enable if the motor is already started
extern uint32_t forward;  // 1 for forward motoring, 0 for reverse motoring

// Define duty variable
extern uint32_t duty; // will be written to TIM1->CCR1 & TIM1->CCR2

// ADC buffer variables
extern uint16_t adc_buf_set[ADC_AVE_SAMPLE];
extern uint16_t adc_buf_sense[ADC_AVE_SAMPLE];
uint16_t adc_ctr = 0;

// Tripping boolean
uint16_t  tripped = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern uint16_t ADC1_Read();
extern uint16_t ADC2_Read();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Initialize the goddamn buffers


/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim4;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  HAL_RCC_NMI_IRQHandler();
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
	/*
	// Enable soft start mode if it is the first time
	if(Soft_st_done == 0){
		if(I_set > 3){
			Soft_st = 1;
			I_start = I_set;
			forward = 1;
		}
		if(I_set < -3){
			Soft_st = 1;
			I_start = I_set;
			forward = 0;
		}
	}
	*/

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SOFT_ST_Pin);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

		// Check if generator mode
		if(duty == 3600){
			forward = !(HAL_GPIO_ReadPin(GPIOA, GEN_MODE_Pin));
		}

		// ------------ ADC READING ---------------- //

		// Read the ADCs by creating a ring buffer
		adc_ctr = adc_ctr + 1;
		if(adc_ctr == ADC_AVE_SAMPLE){
			adc_ctr = 0;
		}

		adc_buf_set[adc_ctr] = ADC1_Read();

		// Momentary values, in Amps
		I_set = ((float)15/2048)*(adc_buf_set[adc_ctr] - 2048);

		// Moving averages, in Amps
		int sum = 0;
		int i = 0;
		for(i = 0; i<100; i++){
			sum = sum + adc_buf_set[i];
		}
		duty = ((float)3600/4096)*((float)sum/100);
//		duty = 0;

		// ----------------------------------------- //




//			/* Do the soft start according to the set value*/
//			if(Soft_st == 1){
//				// No need to limit here, probably?
//				if(duty < 3300){
//					start_count = start_count + 1;
//					duty = start_count/100;
//				}
//
//				if( (I_sense - I_start < 0.1) && (I_sense - I_start > -0.1) ){
//					Soft_st_done = 1;
//					Soft_st = 0;
//				}
//


			/* Limit the duty */
			if(duty > 3500){
				duty = 3600;
			}
			if(duty < 100){
				duty = 0;
			}
			duty = 3600-duty;

			// H-Bridge implementation
			if(forward == 1){
				TIM1->CCR1 = duty; // Q1 is on for duty
				TIM1->CCR2 = 3600; // Q3 is off, Q4 is on
			}
			else{
				TIM1->CCR1 = 3600; // Q1 is off, Q2 is on
				TIM1->CCR2 = duty; // Q3 is on for duty
			}


  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
