/*
 *  task_minigame5.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_MINIGAME5_H_
#define TASK_MINIGAME5_H_

#include <main.h>

extern TaskHandle_t Task_Minigame_5_Handle;

/* Initialization of resources for minigame 5 */
void Task_Minigame_5_Init(void);

/* A minigame task that involves shaking a bottle until it is ready to pop */
void Task_Minigame_5(void *pvParameters);

#endif /* TASK_MINIGAME5_H_ */
