/*
 *  task_console.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_CONSOLE_H_
#define TASK_CONSOLE_H_

#include <main.h>

extern QueueHandle_t Queue_Console;
extern TaskHandle_t Task_Console_Handle;
extern SemaphoreHandle_t Sem_Console;

/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 *
 * The configuration should be identical to what you have done
 * in a previous ICE-08 EXCEPT that you will need to set the priority of the
 * eUSCI to be a value that is equal to 2
 ******************************************************************************/
void Task_Console_Init(void);

/******************************************************************************
* Task used to parse the commands received by the UART
*
* Command  |    Description
*
* L 10          Move Left by 10 pixels
* R 5           Move Right by 5 pixels
* U 20          Move Up by 20 pixels
* D 4           Move Down by 4 pixels
* S 1           Set Ship Speed to 1mS/Pixel
******************************************************************************/
void Task_Console_Bottom_Half(void *pvParameters);

#endif /* TASK_CONSOLE_H_ */
