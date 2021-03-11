/*
 *  task_minigame1.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_MINIGAME1_H_
#define TASK_MINIGAME1_H_

#include <main.h>

#define LIGHT_SENSOR_ADDR         0x44

#define RESULT_REG                0x00
#define CONFIG_REG                0x01
#define LOW_LIM_REG               0x02
#define HIGH_LIM_REG              0x03

#define LIGHT_RESET               0x0810
#define LIGHT_CONTINUOUS_MODE     0x0400
#define LIGHT_CONVERSION_RDY      0x0100 // read only
#define LIGHT_LOW                 0x0000
#define LIGHT_HIGH                0xBFFF

extern TaskHandle_t Task_Minigame_1_Handle;
extern uint16_t lux;

/******************************************************************************
 * Initialize all hardware resources needed for Task_Minigame_1
 ******************************************************************************/
void Task_Minigame_1_Init(void);

/******************************************************************************
 * Read the light sensor and return the lux value
 ******************************************************************************/
float read_light(void);

/******************************************************************************
 * Task that runs the light sensor minigame
 ******************************************************************************/
void Task_Minigame_1(void *pvParameters);

#endif /* TASK_MINIGAME1_H_ */
