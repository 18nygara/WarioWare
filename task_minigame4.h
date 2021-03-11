/*
 *  task_minigame4.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_MINIGAME_4_H_
#define TASK_MINIGAME_4_H_

#include <main.h>

typedef enum
{
    LEFT, RIGHT, UP, DOWN, S1, S2
} SIMON_COMMANDS;

extern TaskHandle_t Task_Minigame_4_Handle;

/* function to initialize resources used for minigame 4 */
void Task_Minigame_4_Init(void);

/* function that will update the LED based on which step we are on */
void update_LED(int update_num);

/* Minigame that mimics simon says in the UART */
void Task_Minigame_4(void *pvParameters);

#endif /* TASK_MINIGAME_4_H_ */
