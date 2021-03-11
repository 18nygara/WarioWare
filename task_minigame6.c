/*
 *  task_minigame6.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Minigame_6_Handle;

/******************************************************************************
 * Initialize all hardware resources needed for Task_Minigame_1
 ******************************************************************************/
void Task_Minigame_6_Init(void)
{
    ece353_MKII_S1_Init();
}

/******************************************************************************
 * Delete the active square cursor from the previous postion and draw the active
 * square cursor on the current position
 *
 * Returns: the number of the selected color based off the current position
 ******************************************************************************/
int Draw_Cursor(int curr_x_pos, int curr_y_pos, int prev_x_pos, int prev_y_pos)
{
    // Color that the user has selected based on the current position
    int selected_color;     // 0,1,2
                            // 3,4,5
                            // 6,7,8

    // Redraw the previously active square
    xSemaphoreTake(LCD_Lock, portMAX_DELAY);

    // Delete the cursor from the Red square if the previous postion was 0,0
    if (prev_x_pos == 0 && prev_y_pos == 0)
    {
        lcd_draw_rectangle(LEFT_COL, UPPER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_RED);
    }
    // Delete the cursor from the Green square if the previous position was 1,0
    else if (prev_x_pos == 1 && prev_y_pos == 0)
    {
        lcd_draw_rectangle(CENTER_COL, UPPER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_GREEN);
    }
    // Delete the cursor from the Blue square if the previous postion was 2,0
    else if (prev_x_pos == 2 && prev_y_pos == 0)
    {
        lcd_draw_rectangle(RIGHT_COL, UPPER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_BLUE);
    }
    // Delete the cursor from the Yellow square if the previous postion was 0,1
    else if (prev_x_pos == 0 && prev_y_pos == 1)
    {
        lcd_draw_rectangle(LEFT_COL, CENTER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_YELLOW);
    }
    // Delete the cursor from the Black square if the previous postion was 1,1
    else if (prev_x_pos == 1 && prev_y_pos == 1)
    {
        lcd_draw_rectangle(CENTER_COL, CENTER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_BLACK);
    }
    // // Delete the cursor from the Cyan square if the previous postion was 2,1
    else if (prev_x_pos == 2 && prev_y_pos == 1)
    {
        lcd_draw_rectangle(RIGHT_COL, CENTER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_CYAN);
    }
    // Delete the cursor from the Magenta square if the previous position was 0,2
    else if (prev_x_pos == 0 && prev_y_pos == 2)
    {
        lcd_draw_rectangle(LEFT_COL, LOWER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_MAGENTA);
    }
    // Delete the cursor from the Orange square if the previous position was 1,2
    else if (prev_x_pos == 1 && prev_y_pos == 2)
    {
        lcd_draw_rectangle(CENTER_COL, LOWER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_ORANGE);
    }
    // Delete the cursor from the White square if the previous postion was 2,2
    else if (prev_x_pos == 2 && prev_y_pos == 2)
    {
        lcd_draw_rectangle(RIGHT_COL, LOWER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);
    }

    // Draw active square cursor

    // Draw the cursor at the Red square if the current position is 0,0
    if (curr_x_pos == 0 && curr_y_pos == 0)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(LEFT_COL, UPPER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(LEFT_COL, UPPER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_RED);

        // Assign the selected color to the appropriate color number
        selected_color = 0;
    }
    // Draw the cursor at the Green square if the current position is 1,0
    else if (curr_x_pos == 1 && curr_y_pos == 0)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(CENTER_COL, UPPER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(CENTER_COL, UPPER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_GREEN);

        // Assign the selected color to the appropriate color number
        selected_color = 1;
    }
    // Draw the cursor at the Blue square if the current position is 2,0
    else if (curr_x_pos == 2 && curr_y_pos == 0)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(RIGHT_COL, UPPER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(RIGHT_COL, UPPER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_BLUE);

        // Assign the selected color to the appropriate color number
        selected_color = 2;
    }
    // Draw the cursor at the Yellow square if the current position is 0,1
    else if (curr_x_pos == 0 && curr_y_pos == 1)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(LEFT_COL, CENTER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(LEFT_COL, CENTER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_YELLOW);

        // Assign the selected color to the appropriate color number
        selected_color = 3;
    }
    // Draw the cursor at the Black square if the current position is 1,1
    else if (curr_x_pos == 1 && curr_y_pos == 1)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(CENTER_COL, CENTER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(CENTER_COL, CENTER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_BLACK);

        // Assign the selected color to the appropriate color number
        selected_color = 4;
    }
    // Draw the cursor at the Cyan square if the current position is 2,1
    else if (curr_x_pos == 2 && curr_y_pos == 1)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(RIGHT_COL, CENTER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(RIGHT_COL, CENTER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_CYAN);

        // Assign the selected color to the appropriate color number
        selected_color = 5;
    }
    // Draw the cursor at the Magenta square if the current position is 0,2
    else if (curr_x_pos == 0 && curr_y_pos == 2)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(LEFT_COL, LOWER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(LEFT_COL, LOWER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_MAGENTA);

        // Assign the selected color to the appropriate color number
        selected_color = 6;
    }
    // Draw the cursor at the Orange square if the current position is 1,2
    else if (curr_x_pos == 1 && curr_y_pos == 2)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(CENTER_COL, LOWER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(CENTER_COL, LOWER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_ORANGE);

        // Assign the selected color to the appropriate color number
        selected_color = 8;
    }
    // Draw the cursor at the White square if the current position is 2,2
    else if (curr_x_pos == 2 && curr_y_pos == 2)
    {
        // Draw Active Square Box
        lcd_draw_rectangle(RIGHT_COL, LOWER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_BLACK);

        lcd_draw_rectangle(RIGHT_COL, LOWER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_WHITE);

        // Assign the selected color to the appropriate color number
        selected_color = 7;
    }

    xSemaphoreGive(LCD_Lock);

    // Return the number of the selected color based off current position
    return selected_color;
}

/******************************************************************************
 * Task that runs the Color Picker minigame
 ******************************************************************************/
void Task_Minigame_6(void *pvParameters)
{
    // Randomly chosen color of the led that the user must match
    int led_color;

    // Top left of grid is 0,0
    // Bottom left of grid is 2,2

    int curr_x_pos;     // (0,0) (1,0) (2,0)
    int curr_y_pos;     // (0,1) (1,1) (2,1)
                        // (0,2) (1,2) (2,2)
    int prev_x_pos;
    int prev_y_pos;

    // Color that the user has selected based on the current position
    int selected_color;     // 0,1,2
                            // 3,4,5
                            // 6,7,8

    bool s1_pressed;
    bool joystick_valid;

    uint32_t acceler_arr[3];

    JOYSTICK_t joystick;

    while (1)
    {

        // Wait for the minigame to be called
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Initialize variables for a new minigame
        curr_x_pos = 1;
        curr_y_pos = 1;

        prev_y_pos = 1;
        prev_x_pos = 1;

        selected_color = 4;

        joystick_valid = true;
        s1_pressed = false;

        // Draw colored boxes
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        // TOP LEFT
        lcd_draw_rectangle(LEFT_COL, UPPER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_RED);
        // TOP CENTER
        lcd_draw_rectangle(CENTER_COL, UPPER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_GREEN);

        // TOP RIGHT
        lcd_draw_rectangle(RIGHT_COL, UPPER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_BLUE);

        // CENTER LEFT
        lcd_draw_rectangle(LEFT_COL, CENTER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_YELLOW);

        // CENTER RIGHT
        lcd_draw_rectangle(RIGHT_COL, CENTER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_CYAN);

        // BOTTOM LEFT
        lcd_draw_rectangle(LEFT_COL, LOWER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_MAGENTA);

        // BOTTOM MIDDLE
        lcd_draw_rectangle(CENTER_COL, LOWER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_ORANGE);

        // BOTTOM RIGHT
        lcd_draw_rectangle(RIGHT_COL, LOWER_ROW,
        BOX_WIDTH,
                           BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        // Draw Active Square Box in the Center
        lcd_draw_rectangle(CENTER_COL, CENTER_ROW, BOX_WIDTH,
        BOX_HEIGHT,
                           LCD_COLOR_WHITE);

        lcd_draw_rectangle(CENTER_COL, CENTER_ROW, BOX_WIDTH - 10,
        BOX_HEIGHT - 10,
                           LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);

        // Randomly choose one of the colors
        // Can only do 8 values because can only produce 8 different colors with the RGB LED function
        led_color = rand() % 8;

        // Turn on LED to a color corresponding to led_color
        // 0-red, 1-green, 3-red, 4-yellow, 5-black, 6-cyan, 7-magenta, 8-white
        switch (led_color)
        {
        case 0:
            ece353_MKII_RGB_LED(true, false, false);
            break;
        case 1:
            ece353_MKII_RGB_LED(false, true, false);
            break;
        case 2:
            ece353_MKII_RGB_LED(false, false, true);
            break;
        case 3:
            ece353_MKII_RGB_LED(true, true, false);
            break;
        case 4:
            ece353_MKII_RGB_LED(false, false, false);
            break;
        case 5:
            ece353_MKII_RGB_LED(false, true, true);
            break;
        case 6:
            ece353_MKII_RGB_LED(true, false, true);
            break;
        case 7:
            ece353_MKII_RGB_LED(true, true, true);
            break;
        default:
            break;
        }

        // Print instructions
        printf("Match the LED and a Square! (Press S1 to choose)\r\n\r\n");

        // Start the timer
        xTaskNotifyGive(Task_Timer_Handle);

        // Loop while the minigame can run
        while (time_available && !s1_pressed)
        {
            // Wait until a message is received in the joystick queue
            xQueueReceive(Queue_Joystick, &joystick, portMAX_DELAY);

            // Receive the Accelerometer queue so it isn't floating within our program
            xQueueReceive(Queue_Accelerometer, &acceler_arr, portMAX_DELAY);

            // Based on the value in the joystick, update the current position
            switch (joystick)
            {
            case JOYSTICK_LEFT:
                // Only update the position if the joystick returns back to the center
                if (joystick_valid)
                {
                    // Wrap to right side, if moving left on the left border
                    if (curr_x_pos == 0)
                    {
                        curr_x_pos = 2;
                    }
                    else
                    {
                        curr_x_pos--;
                    }

                    joystick_valid = false;
                }
                break;
            case JOYSTICK_RIGHT:
                // Only update the position if the joystick returns back to the center
                if (joystick_valid)
                {
                    // Wrap to left side, if moving right on the right border
                    if (curr_x_pos == 2)
                    {
                        curr_x_pos = 0;
                    }
                    else
                    {
                        curr_x_pos++;
                    }

                    joystick_valid = false;
                }
                break;
            case JOYSTICK_UP:
                // Only update the position if the joystick returns back to the center
                if (joystick_valid)
                {
                    // Wrap to the bottom, if moving up on the top border
                    if (curr_y_pos == 0)
                    {
                        curr_y_pos = 2;
                    }
                    else
                    {
                        curr_y_pos--;
                    }

                    joystick_valid = false;
                }
                break;
            case JOYSTICK_DOWN:
                // Only update the position if the joystick returns back to the center
                if (joystick_valid)
                {
                    // Wrap to the top, if moving down on the bottom border
                    if (curr_y_pos == 2)
                    {
                        curr_y_pos = 0;
                    }
                    else
                    {
                        curr_y_pos++;
                    }

                    joystick_valid = false;
                }
                break;
            default:
                // Allow joystick updates if the joystick is in the center
                joystick_valid = true;
                break;
            }

            // Update the selected color and the active cursor if the previous position is different from the current position
            if (curr_x_pos != prev_x_pos || curr_y_pos != prev_y_pos)
            {
                selected_color = Draw_Cursor(curr_x_pos, curr_y_pos, prev_x_pos,
                                             prev_y_pos);
            }

            // Store the current values so they can be updated
            prev_x_pos = curr_x_pos;
            prev_y_pos = curr_y_pos;

            // Check if the user has choosen a color
            s1_pressed = ece353_MKII_S1();
        }
        // Assume the user has won the minigame
        minigame.win = true;

        // Check if there is time left
        if (time_available)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));

            // Check if the user selected the correct color
            if (selected_color != led_color)
            {
                // User has lost the minigame
                minigame.win = false;
            }

            // Send the winning or losing queue
            xQueueSend(Queue_Minigames, &minigame, portMAX_DELAY);
        }

        // Turn of the LED
        ece353_MKII_RGB_LED(false, false, false);
    }
}

