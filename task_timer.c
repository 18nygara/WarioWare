/*
 *  task_timer.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Timer_Handle;
bool time_available; // boolean that will let a minigame know if it needs to end or not

/* Task that will display how much time is left for a minigame */
void Task_Timer(void *pvParameters)
{
    while (1)
    {
        // wait until the task is notified at the start of a minigame
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // wait based on the MEASURE_DURATION and then remove a portion of the bomb, notifying how much time is left
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);
            lcd_draw_rectangle(125, 114, 10, 18, LCD_COLOR_BLACK);
            xSemaphoreGive(LCD_Lock);
        }
        // repeat until time is out
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);
            lcd_draw_rectangle(118, 125, 15, 5, LCD_COLOR_BLACK);
            xSemaphoreGive(LCD_Lock);
        }
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);
            lcd_draw_rectangle(101, 127, 20, 2, LCD_COLOR_BLACK);
            xSemaphoreGive(LCD_Lock);
        }
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);
            lcd_draw_rectangle(80, 127, 21, 1, LCD_COLOR_BLACK);
            xSemaphoreGive(LCD_Lock);
        }
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);
            lcd_draw_rectangle(58, 127, 23, 1, LCD_COLOR_BLACK);
            xSemaphoreGive(LCD_Lock);
        }
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);
            lcd_draw_rectangle(39, 126, 25, 10, LCD_COLOR_BLACK);
            xSemaphoreGive(LCD_Lock);
        }
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);
            lcd_draw_rectangle(26, 116, 10, 21, LCD_COLOR_BLACK);
            lcd_draw_rectangle(19, 105, 10, 1, LCD_COLOR_BLACK);
            xSemaphoreGive(LCD_Lock);
        }
        if (!minigame.win)
        {
            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
        }
        // last check, update the losing conditions and draw the explosion
        if (!minigame.win)
        {
            time_available = false;
            minigame.win = false;

            xSemaphoreTake(LCD_Lock, portMAX_DELAY);

            lcd_draw_image(15, 117, explosion_WidthPixels,
                           explosion_HeightPixels, explosion_map, LCD_COLOR_RED,
                           LCD_COLOR_BLACK);

            xSemaphoreGive(LCD_Lock);

            vTaskDelay(pdMS_TO_TICKS(MEASURE_DURATION / 37500));
            // send a losing queue
            xQueueSend(Queue_Minigames, &minigame, portMAX_DELAY);
        }
    }
}
