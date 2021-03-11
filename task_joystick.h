/*
 *  task_joystick.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_JOYSTICK_H_
#define TASK_JOYSTICK_H_

#include "main.h"

#define VOLT_0P85  1056      // 0.85 /(3.3/4096)
#define VOLT_2P50  3103      // 2.50 /(3.3/4096)

typedef enum
{
    JOYSTICK_LEFT, JOYSTICK_RIGHT, JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_CENTER
} JOYSTICK_t;

extern TaskHandle_t Task_Joystick_Handle;
extern TaskHandle_t Task_Joystick_Timer_Handle;
extern QueueHandle_t Queue_Joystick;
extern QueueHandle_t Queue_Accelerometer;
extern JOYSTICK_t joystick;
extern volatile uint32_t JOYSTICK_X_DIR;
extern volatile uint32_t JOYSTICK_Y_DIR;
extern volatile uint32_t ACCELER_X;
extern volatile uint32_t ACCELER_Y;
extern volatile uint32_t ACCELER_Z;
extern uint32_t ACCELER_ARR[3];

/******************************************************************************
 * Configure the IO pins for BOTH the X and Y directions of the analog
 * joystick.  The X direction should be configured to place the results in
 * MEM[0].  The Y direction should be configured to place the results in MEM[1].
 *
 * After BOTH analog signals have finished being converted, a SINGLE interrupt
 * should be generated.
 *
 * Parameters
 *      None
 * Returns
 *      None
 ******************************************************************************/
void Task_Joystick_Init(void);

/******************************************************************************
 * Used to start an ADC14 Conversion
 ******************************************************************************/
void Task_Joystick_Timer(void *pvParameters);

/******************************************************************************
 * Examines the ADC data from the joystick on the MKII
 ******************************************************************************/
void Task_Joystick_Bottom_Half(void *pvParameters);

#endif /* TASK_JOYSTICK_H_ */
