/*
 *  task_minigame3.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_MINIGAME_3_H_
#define TASK_MINIGAME_3_H_

#include <main.h>

extern TaskHandle_t Task_Minigame_3_Handle;

/* function that initializes resources related to minigame 3 */
void Task_Minigame_3_Init(void);

/* Minigame that involves a spaceship abducting a cow */
void Task_Minigame_3(void *pvParameters);

#endif /* TASK_MINIGAME_3_H_ */
