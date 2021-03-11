/*
 *  task_timer.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_TIMER_H_
#define TASK_TIMER_H_

#include <main.h>

extern TaskHandle_t Task_Timer_Handle;
extern bool time_available;

/* Task that will display how much time is left for a minigame */
void Task_Timer(void *pvParameters);

#endif /* TASK_TIMER_H_ */
