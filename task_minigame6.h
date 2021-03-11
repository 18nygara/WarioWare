/*
 *  task_minigame6.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_MINIGAME6_H_
#define TASK_MINIGAME6_H_

#include <main.h>

#define SCREEN_X            132
#define SCREEN_Y            102

#define LEFT_COL            21
#define CENTER_COL          66
#define RIGHT_COL           111

#define UPPER_ROW           16
#define CENTER_ROW          51
#define LOWER_ROW           86

#define BOX_WIDTH           42
#define BOX_HEIGHT          32

extern TaskHandle_t Task_Minigame_6_Handle;

/******************************************************************************
 * Delete the active square cursor from the previous postion and draw the active
 * square cursor on the current position
 *
 * Returns: the number of the selected color based off the current position
 ******************************************************************************/
int Draw_Cursor(int curr_x_pos, int curr_y_pos, int prev_x_pos, int prev_y_pos);

/******************************************************************************
 * Initialize all hardware resources needed for Task_Minigame_1
 ******************************************************************************/
void Task_Minigame_6_Init(void);

/******************************************************************************
 * Task that runs the Color Picker minigame
 ******************************************************************************/
void Task_Minigame_6(void *pvParameters);

#endif /* TASK_MINIGAME6_H_ */
