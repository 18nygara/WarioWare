/*
 *  task_minigame3.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Minigame_3_Handle;

/* function that initializes resources related to minigame 3 */
void Task_Minigame_3_Init(void)
{
    ece353_MKII_S1_Init();
}

/* Minigame that involves a spaceship abducting a cow */
void Task_Minigame_3(void *pvParameters)
{
    JOYSTICK_t joystick;
    int x_cow;
    int x_ship;
    int cow_count;
    int cow_direction;
    bool s1_pressed;
    uint32_t acceler_arr[3];

    while (1)
    {
        // wait for the minigame to be called
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // initialize the variables for a new minigame
        x_ship = (rand() % 100) + 9;
        x_cow = (rand() % 100) + 13;
        cow_direction = rand() % 2;
        cow_count = 0;
        s1_pressed = false;

        // draw the cow and ship
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        lcd_draw_image(x_cow, 93, cow_WidthPixels, cow_HeightPixels,
                       WarioWare_Cow_map, LCD_COLOR_WHITE, LCD_COLOR_BLACK);

        lcd_draw_image(x_ship, 10, ship_WidthPixels, ship_HeightPixels - 12,
                       WarioWare_Ship_map, LCD_COLOR_WHITE, LCD_COLOR_BLACK);

        lcd_draw_rectangle(66, 101, 132, 1, LCD_COLOR_WHITE);

        xSemaphoreGive(LCD_Lock);

        // print the directions
        printf("Abduct the cow! (Press S1 to abduct)\r\n\r\n");

        // start the timer
        xTaskNotifyGive(Task_Timer_Handle);

        // loop while the minigame can run
        while (time_available && !s1_pressed)
        {
            // Wait until a message is received in the joystick queue
            xQueueReceive(Queue_Joystick, &joystick, portMAX_DELAY);

            // receive the Accelerometer queue so it isn't floating within our program
            xQueueReceive(Queue_Accelerometer, &acceler_arr, portMAX_DELAY);

            // check the direction of the joystick
            switch (joystick)
            {
            case JOYSTICK_LEFT:
                // boundary checking
                if (x_ship >= 11)
                {
                    // update the position of the ship
                    x_ship -= 1;
                }
                break;
            case JOYSTICK_RIGHT:
                if (x_ship <= 122)
                {
                    x_ship += 1;

                }
                break;
            default:
                break;
            }

            // update the position of the cow
            if (cow_direction)
            {
                // boundary checking
                if (x_cow > 13)
                {
                    // update cow_count so we don't go too fast
                    cow_count++;
                    // only move every other frame
                    if (cow_count == 2)
                    {
                        x_cow--;
                        cow_count = 0;
                    }
                }
                // if we hit a wall, change the direction of the cow
                else
                {
                    cow_direction = 0;
                }
            }
            else
            {
                if (x_cow < 118)
                {
                    cow_count++;
                    if (cow_count == 2)
                    {
                        x_cow++;
                        cow_count = 0;
                    }
                }
                else
                {
                    cow_direction = 1;
                }
            }

            // draw the new position of the cow and ship
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);

            lcd_draw_image(x_ship, 10, ship_WidthPixels, ship_HeightPixels - 12,
                           WarioWare_Ship_map,
                           LCD_COLOR_WHITE,
                           LCD_COLOR_BLACK);

            lcd_draw_image(x_cow, 93, cow_WidthPixels, cow_HeightPixels,
                           WarioWare_Cow_map, LCD_COLOR_WHITE, LCD_COLOR_BLACK);

            xSemaphoreGive(LCD_Lock);

            // check if S1 was pressed
            s1_pressed = ece353_MKII_S1();
        }
        // check S1 condition pressed early
        if (time_available)
        {
            minigame.win = true;
            // draw the rest of the space_ship's beam
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);

            lcd_draw_image(x_ship, 16, ship_WidthPixels, ship_HeightPixels,
                           WarioWare_Ship_map,
                           LCD_COLOR_WHITE,
                           LCD_COLOR_BLACK);

            xSemaphoreGive(LCD_Lock);

            // check if the spaceship was aligned with the cow
            if (abs(x_ship - x_cow) > 10)
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
                minigame.win = false;
            }
            // we were aligned
            else
            {
                // animate the cow moving up
                xSemaphoreTake(LCD_Lock, portMAX_DELAY);
                int i;
                for (i = 1; i < 35; i++)
                {
                    lcd_draw_image(x_cow, 93 - i, cow_WidthPixels, cow_HeightPixels,
                                   WarioWare_Cow_map, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
                    vTaskDelay(pdMS_TO_TICKS(33));
                }
                xSemaphoreGive(LCD_Lock);
            }
            // send the winning or losing queue
            xQueueSend(Queue_Minigames, &minigame, portMAX_DELAY);
        }
    }
}
