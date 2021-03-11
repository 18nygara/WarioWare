/*
 *  task_driver.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_DRIVER_H_
#define TASK_DRIVER_H_

#include <main.h>

typedef struct
{
    bool win;
} MINIGAME_MSG_t;

#define NUM_MINIGAMES   6

extern TaskHandle_t Task_Driver_Handle;
extern QueueHandle_t Queue_Minigames;
extern SemaphoreHandle_t LCD_Lock;

extern MINIGAME_MSG_t minigame;
extern int num_lives;
extern int score;
extern bool allow_ADC;
extern time_t t;
extern int curr_minigame;

/******************************************************************************
 * This function will initialize Queue_Minigames and initialize the LCD
 ******************************************************************************/
void Task_Driver_Init(void);

/******************************************************************************
 * This task manages the playing of the minigames. A timer running out, or a
 * winning condition can send a message to Queue_Minigames
 ******************************************************************************/
void Task_Driver(void *pvParameters);

#endif /* TASK_DRIVER_H_ */
