/*
 *  task_minigame4.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Minigame_4_Handle;

/* function to initialize resources used for minigame 4 */
void Task_Minigame_4_Init(void)
{
    ece353_MKII_RGB_IO_Init(false);
    ece353_MKII_S1_Init();
    ece353_MKII_S2_Init();
}

/* function that will update the LED based on which step we are on */
void update_LED(int update_num)
{
    switch (update_num)
    {
    case 0:
        ece353_MKII_RGB_LED(true, false, false);
        break;
    case 1:
        ece353_MKII_RGB_LED(true, true, false);
        break;
    case 2:
        ece353_MKII_RGB_LED(false, false, true);
        break;
    case 3:
        ece353_MKII_RGB_LED(false, true, false);
        break;
    default:
        break;
    }
}

/* Minigame that mimics simon says in the UART */
void Task_Minigame_4(void *pvParameters)
{
    JOYSTICK_t joystick;
    int i;
    int cmd_rand;
    bool valid_input;
    SIMON_COMMANDS cmds[4];
    bool simon_complete;
    uint32_t acceler_arr[3];

    while (1)
    {
        // wait for the minigame to be called
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // draw the UART notification arrow
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        lcd_draw_image(66, 51, arrow_WidthPixels, arrow_HeightPixels,
                       WarioWare_Arrow_map, LCD_COLOR_WHITE, LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);

        // update variables to initialize the game
        simon_complete = false;
        valid_input = true;

        // print instructions to the screen
        printf("Simon Says! INPUT THE FOLLOWING COMMANDS\r\n\r\n");

        // give the user timer to look up at the UART
        vTaskDelay(pdMS_TO_TICKS(250));

        // choose the commands and print them
        for (i = 0; i < 4; i++)
        {
            cmd_rand = rand() % 6;
            switch (cmd_rand)
            {
            case 0:
                printf("UP ");
                cmds[i] = UP;
                break;
            case 1:
                printf("DOWN ");
                cmds[i] = DOWN;
                break;
            case 2:
                printf("LEFT ");
                cmds[i] = LEFT;
                break;
            case 3:
                printf("RIGHT ");
                cmds[i] = RIGHT;
                break;
            case 4:
                printf("S1 ");
                cmds[i] = S1;
                break;
            case 5:
                printf("S2 ");
                cmds[i] = S2;
                break;
            default:
                break;
            }
        }

        // print new lines so input looks clean
        printf("\r\n\r\n");

        // start the timer
        xTaskNotifyGive(Task_Timer_Handle);

        // set i to 0, we will use this as our step variable
        i = 0;

        // loop while the game is still running
        while (time_available && !simon_complete)
        {
            // Wait until a message is received in the joystick queue
            xQueueReceive(Queue_Joystick, &joystick, portMAX_DELAY);

            // receive the Accelerometer queue so it isn't floating within our program
            xQueueReceive(Queue_Accelerometer, &acceler_arr, portMAX_DELAY);

            // check the position of the joystick
            switch (joystick)
            {
            case JOYSTICK_LEFT:
                // check if input is valid
                if (valid_input)
                {
                    // check if input is matched with the current command
                    if (cmds[i] == LEFT)
                    {
                        // increment the step
                        i++;
                    }
                    else
                    {
                        // restart the count if input was wrong
                        i = 0;
                    }
                    // update valid input to false
                    valid_input = false;
                }
                break;
            case JOYSTICK_RIGHT:
                if (valid_input)
                {
                    if (cmds[i] == RIGHT)
                    {
                        i++;
                    }
                    else
                    {
                        i = 0;
                    }
                    valid_input = false;
                }
                break;
            case JOYSTICK_UP:
                if (valid_input)
                {
                    if (cmds[i] == UP)
                    {
                        i++;
                    }
                    else
                    {
                        i = 0;
                    }
                    valid_input = false;
                }
                break;
            case JOYSTICK_DOWN:
                if (valid_input)
                {
                    if (cmds[i] == DOWN)
                    {
                        i++;
                    }
                    else
                    {
                        i = 0;
                    }
                    valid_input = false;
                }
                break;
            default:
                if (ece353_MKII_S1())
                {
                    if (valid_input)
                    {
                        if (cmds[i] == S1)
                        {
                            i++;
                        }
                        else
                        {
                            i = 0;
                        }
                        valid_input = false;
                    }
                }
                else if (ece353_MKII_S2())
                {
                    if (valid_input)
                    {
                        if (cmds[i] == S2)
                        {
                            i++;
                        }
                        else
                        {
                            i = 0;
                        }
                        valid_input = false;
                    }
                }
                else
                {
                    // if nothing is happening, reset the valid input field
                    valid_input = true;
                }
                break;
            }

            // update the LED to let the person know what step they're on
            update_LED(i);

            // check if we've finished
            if (i == 4)
            {
                simon_complete = true;
            }
        }
        // check if we won
        if (time_available)
        {
            // change win to true and exit the minigame
            minigame.win = true;
            printf("Simon ain't got nothing on you.\r\n\r\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            xQueueSend(Queue_Minigames, &minigame, portMAX_DELAY);
        }
        // turn off the LED no matter what happens
        ece353_MKII_RGB_LED(false, false, false);
    }
}
