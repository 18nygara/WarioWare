/*
 *  task_driver.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Driver_Handle;
QueueHandle_t Queue_Minigames;
SemaphoreHandle_t LCD_Lock;

MINIGAME_MSG_t minigame;
int num_lives;
int score;
bool allow_ADC;
time_t t;
int curr_minigame;

void draw_hearts(int lives)
{
    switch (lives)
    {
    case 3:
        lcd_draw_image(22, 66, heart_WidthPixels, heart_HeightPixels,
                       WarioWare_Heart_map,
                       LCD_COLOR_RED,
                       LCD_COLOR_BLACK);

        lcd_draw_image(66, 66, heart_WidthPixels, heart_HeightPixels,
                       WarioWare_Heart_map,
                       LCD_COLOR_RED,
                       LCD_COLOR_BLACK);

        lcd_draw_image(110, 66, heart_WidthPixels, heart_HeightPixels,
                       WarioWare_Heart_map,
                       LCD_COLOR_RED,
                       LCD_COLOR_BLACK);
        break;
    case 2:
        lcd_draw_image(22, 66, heart_WidthPixels, heart_HeightPixels,
                       Broken_Heart_map,
                       LCD_COLOR_WHITE,
                       LCD_COLOR_BLACK);

        lcd_draw_image(66, 66, heart_WidthPixels, heart_HeightPixels,
                       WarioWare_Heart_map,
                       LCD_COLOR_RED,
                       LCD_COLOR_BLACK);

        lcd_draw_image(110, 66, heart_WidthPixels, heart_HeightPixels,
                       WarioWare_Heart_map,
                       LCD_COLOR_RED,
                       LCD_COLOR_BLACK);
        break;
    case 1:
        lcd_draw_image(22, 66, heart_WidthPixels, heart_HeightPixels,
                       Broken_Heart_map,
                       LCD_COLOR_WHITE,
                       LCD_COLOR_BLACK);

        lcd_draw_image(66, 66, heart_WidthPixels, heart_HeightPixels,
                       Broken_Heart_map,
                       LCD_COLOR_WHITE,
                       LCD_COLOR_BLACK);

        lcd_draw_image(110, 66, heart_WidthPixels, heart_HeightPixels,
                       WarioWare_Heart_map,
                       LCD_COLOR_RED,
                       LCD_COLOR_BLACK);
        break;
    default:
        lcd_draw_image(22, 66, heart_WidthPixels, heart_HeightPixels,
                       Broken_Heart_map,
                       LCD_COLOR_WHITE,
                       LCD_COLOR_BLACK);

        lcd_draw_image(66, 66, heart_WidthPixels, heart_HeightPixels,
                       Broken_Heart_map,
                       LCD_COLOR_WHITE,
                       LCD_COLOR_BLACK);

        lcd_draw_image(110, 66, heart_WidthPixels, heart_HeightPixels,
                       Broken_Heart_map, LCD_COLOR_WHITE,
                       LCD_COLOR_BLACK);
        break;
    }
}

/******************************************************************************
 * This function will initialize Queue_Minigames
 ******************************************************************************/
void Task_Driver_Init(void)
{
    Queue_Minigames = xQueueCreate(1, sizeof(MINIGAME_MSG_t));
    num_lives = 3;
    score = 0;
    allow_ADC = false;
    minigame.win = false;
    curr_minigame = 0;
    srand((unsigned) time(&t));
    LCD_Lock = xSemaphoreCreateBinary();
    xSemaphoreGive(LCD_Lock);
}

/******************************************************************************
 * This task manages the playing of the minigames. A timer running out or a
 * winning condition can send a message to Queue_Minigames
 ******************************************************************************/
void Task_Driver(void *pvParameters)
{
    // draw the title screen, this is the only thing running so we don't need a semaphore
    lcd_draw_image(66, 66, title_WidthPixels, title_HeightPixels,
                   WarioWare_title_map, LCD_COLOR_WHITE, LCD_COLOR_BLACK);

    printf("*********************\r\n");
    printf("Welcome to WarioWare!\r\n");
    printf("*********************\r\n\r\n");
    printf("Instructions will appear here - complete as many minigames as you can within the time limit.\r\n");
    printf("You have 3 lives, good luck!\r\n\r\n");

    while (!ece353_MKII_S1())
    {
        // busy wait until S1 is pressed
    }

    // clear the screen
    lcd_draw_rectangle(66, 66, 132, 132, LCD_COLOR_BLACK);

    while (1)
    {
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        /* DRAW LIVES AND SCORE */

        // Draw the hearts, number of non-broken hearts based off of num_lives
        draw_hearts(num_lives);

        xSemaphoreGive(LCD_Lock);

        if (!num_lives)
        {
            // print game over and score to the screen
            printf("******************\r\n");
            printf("G A M E    O V E R\r\n");
            printf("******************\r\n");
            printf("Final Score: ");
            char str[3];
            sprintf(str, "%d", score);
            printf(str);
            printf("\r\n\r\n");

            // suspend the task forever, that's game over
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        }
        else
        {
            // print the lives and score
            char char_lives[1];
            sprintf(char_lives, "%d", num_lives);
            printf(char_lives);
            printf(" LIVES REMAINING\r\n");
            printf("Current Score: ");
            char char_score[3];
            sprintf(char_score, "%d", score);
            printf(char_score);
            printf("\r\n\r\n");
        }

        // Send a queue message to Task_Play_Song
        xQueueSend(Queue_Song, &minigame.win, portMAX_DELAY);

        // Wait until Task_Play_Song gives control back
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* INITALIZE THE MINIGAME */

        // initialize the conditions for win and fail
        minigame.win = false;
        time_available = true;

        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        // Clear the LCD screen
        lcd_draw_rectangle(66, 66, 132, 44, LCD_COLOR_BLACK);

        // Draw the bomb
        lcd_draw_image(66, 117, bomb_WidthPixels, bomb_HeightPixels,
                       WarioWare_Bomb_map,
                       LCD_COLOR_WHITE,
                       LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);

        /* START A MINIGAME */

        // pick a random minigame and play it
        curr_minigame = rand() % NUM_MINIGAMES;

        switch (curr_minigame)
        {
        case 0:
            xTaskNotifyGive(Task_Minigame_1_Handle);
            break;
        case 1:
            xTaskNotifyGive(Task_Minigame_2_Handle);
            break;
        case 2:
            allow_ADC = true;
            xTaskNotifyGive(Task_Minigame_3_Handle);
            break;
        case 3:
            allow_ADC = true;
            xTaskNotifyGive(Task_Minigame_4_Handle);
            break;
        case 4:
            allow_ADC = true;
            xTaskNotifyGive(Task_Minigame_5_Handle);
            break;
        case 5:
            allow_ADC = true;
            xTaskNotifyGive(Task_Minigame_6_Handle);
            break;
        default:
            break;
        }


        // Wait until a message is received in the Queue_Minigame
        xQueueReceive(Queue_Minigames, &minigame, portMAX_DELAY);

        /* TEARDOWN AFTER MINIGAME */

        // reset the joystick so we don't constantly check it
        allow_ADC = false;

        // Decrement the number of lives if the player failed the minigame
        if (!minigame.win)
        {
            num_lives--;
        }
        else
        {
            score++;
        }

        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        // Clear the minigame info from the LCD display
        lcd_draw_rectangle(66, 66, 132, 132, LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);
    }
}
