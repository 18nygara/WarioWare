/*
 *  task_minigame5.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Minigame_5_Handle;

/* Initialization of resources for minigame 5 */
void Task_Minigame_5_Init(void)
{
    ece353_MKII_S1_Init();
}

/* A minigame task that involves shaking a bottle until it is ready to pop */
void Task_Minigame_5(void *pvParameters)
{
    JOYSTICK_t joystick;
    uint32_t acceler_arr[3];
    int shake_level;
    int shake_time;
    bool bottle_shaking;

    while (1)
    {
        // wait for the minigame to be called
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // draw the initial bottle to the screen
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        lcd_draw_image(66, 62, bottle_WidthPixels, bottle_HeightPixels,
                       WarioWare_Bottle_map, LCD_COLOR_GREEN, LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);

        // print directions
        printf("Shake the bottle!\r\n\r\n");

        // initialize used variables
        shake_level = 0;
        shake_time = 0;
        bottle_shaking = true;

        // let the timer start
        xTaskNotifyGive(Task_Timer_Handle);

        // loop while the game can still run
        while (time_available && bottle_shaking)
        {
            // receive the joystick queue so it isn't floating within our program
            xQueueReceive(Queue_Joystick, &joystick, portMAX_DELAY);

            // Wait until a message is received in the Accelerometer queue
            xQueueReceive(Queue_Accelerometer, &acceler_arr, portMAX_DELAY);

            // check the accelerometer values to see if any term is high, i.e. we are shaking the board
            if (acceler_arr[0] > VOLT_2P50 || acceler_arr[1] > VOLT_2P50
                    || acceler_arr[2] > VOLT_2P50)
            {
                // increase the shake time
                shake_time++;
                // check if the shake time is acceptable for a given level
                if ((shake_time > 5 && shake_level < 4)
                        || (shake_time > 12 && shake_level == 4))
                {
                    // increase your shake level if we've shook enough
                    shake_level++;
                    // reset shake time
                    shake_time = 0;
                    // draw the new bottle based on the shake level
                    xSemaphoreTake(LCD_Lock, portMAX_DELAY);
                    switch (shake_level)
                    {
                    case 0:
                        lcd_draw_image(66, 62, bottle_WidthPixels,
                                       bottle_HeightPixels,
                                       WarioWare_Bottle_map, LCD_COLOR_GREEN,
                                       LCD_COLOR_BLACK);
                        break;
                    case 1:
                        lcd_draw_image(66, 62, bottle_WidthPixels,
                                       bottle_HeightPixels,
                                       WarioWare_Bottle_map, LCD_COLOR_YELLOW,
                                       LCD_COLOR_BLACK);
                        break;
                    case 2:
                        lcd_draw_image(66, 62, bottle_WidthPixels,
                                       bottle_HeightPixels,
                                       WarioWare_Bottle_map, LCD_COLOR_ORANGE,
                                       LCD_COLOR_BLACK);
                        break;
                    case 3:
                        lcd_draw_image(66, 62, bottle_WidthPixels,
                                       bottle_HeightPixels,
                                       WarioWare_Bottle_map, LCD_COLOR_RED,
                                       LCD_COLOR_BLACK);
                        break;
                    case 4:
                        lcd_draw_rectangle(66, 62, bottle_WidthPixels,
                                           bottle_HeightPixels,
                                           LCD_COLOR_BLACK);
                        lcd_draw_image(66, 62, explosion_WidthPixels,
                                       explosion_HeightPixels, explosion_map,
                                       LCD_COLOR_RED,
                                       LCD_COLOR_BLACK);
                        bottle_shaking = false;
                        break;
                    default:
                        break;
                    }
                    xSemaphoreGive(LCD_Lock);
                }
            }

            // check if s1 was pressed, stop checking for shaking
            if (ece353_MKII_S1() && shake_level != 4)
            {
                // check if we pressed s1 at the right time
                if (shake_level == 3)
                {
                    minigame.win = true;
                    // draw the spray of the bottle
                    xSemaphoreTake(LCD_Lock, portMAX_DELAY);

                    lcd_draw_image(66, 62, bottle_WidthPixels,
                                   bottle_HeightPixels, WarioWare_Bottle_map,
                                   LCD_COLOR_WHITE,
                                   LCD_COLOR_BLACK);
                    lcd_draw_image(66, 10, splash_WidthPixels,
                                   splash_HeightPixels, WarioWare_Splash_map,
                                   LCD_COLOR_WHITE,
                                   LCD_COLOR_BLACK);
                    xSemaphoreGive(LCD_Lock);
                }
                // no longer check for shaking
                bottle_shaking = false;
            }
        }

        // win conditions
        if (time_available && minigame.win)
        {
            // animate the spray of the bottle
            int i;
            for (i = 0; i < 20; i++)
            {
                xSemaphoreTake(LCD_Lock, portMAX_DELAY);

                if (i % 2 == 0)
                {
                    lcd_draw_rectangle(90, 80, 2, 50, LCD_COLOR_BLACK);
                    lcd_draw_image(64, 62, bottle_WidthPixels,
                                   bottle_HeightPixels, WarioWare_Bottle_map,
                                   LCD_COLOR_WHITE,
                                   LCD_COLOR_BLACK);
                    lcd_draw_image(64, 10, splash_WidthPixels,
                                   splash_HeightPixels, WarioWare_Splash_map,
                                   LCD_COLOR_WHITE,
                                   LCD_COLOR_BLACK);
                }
                else
                {
                    lcd_draw_rectangle(42, 80, 2, 50, LCD_COLOR_BLACK);
                    lcd_draw_image(68, 62, bottle_WidthPixels,
                                   bottle_HeightPixels, WarioWare_Bottle_map,
                                   LCD_COLOR_WHITE,
                                   LCD_COLOR_BLACK);
                    lcd_draw_image(68, 10, splash_WidthPixels,
                                   splash_HeightPixels, WarioWare_Splash_map,
                                   LCD_COLOR_WHITE,
                                   LCD_COLOR_BLACK);
                }
                xSemaphoreGive(LCD_Lock);

                vTaskDelay(pdMS_TO_TICKS(50));
            }
        }
        // losing conditions
        else if (time_available)
        {
            // update to true so the timer stops
            minigame.win = true;
            // wait so the user can take in what is happening
            vTaskDelay(pdMS_TO_TICKS(1000));
            // lost the minigame
            minigame.win = false;
        }
        // send a mingame queue
        xQueueSend(Queue_Minigames, &minigame, portMAX_DELAY);
    }
}

