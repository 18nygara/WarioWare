/*
 *  task_minigame2.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Minigame_2_Handle;

float temp;
float prev_temp;

/******************************************************************************
 * Initialize all hardware resources needed by Task_Minigame_2
 ******************************************************************************/
void Task_Minigame_2_Init(void)
{
    // Initialize the RGB LED
    ece353_MKII_RGB_IO_Init(false);

    // Reset the device using the CONFIG register
    i2c_write_16(I2C_TEMP_ADDR, I2C_TEMP_CONFIG, TMP006_RST);

    // Program the CONFIG register to POWER_UP and be in CR_2 mode
    i2c_write_16(I2C_TEMP_ADDR, I2C_TEMP_CONFIG,
    TMP006_POWER_UP | TMP006_CR_2);

    // Turn of the RBG LED if it was on
    ece353_MKII_RGB_LED(false, false, false);
}

/******************************************************************************
 * Returns the current temperature in degrees C
 ******************************************************************************/
float read_temp(void)
{
    // Read the ambient temperature
    uint16_t temp_reg = i2c_read_16(I2C_TEMP_ADDR, I2C_TEMP_AMBIENT_TEMP);

    // Return the data in degrees C.
    return (temp_reg >> 2) / 32;
}

/*******************************************************************************
 * Draw the appropriate image to the screen and turn on the appropriate LED based
 * on the temp value
 ******************************************************************************/
void display_temp(float temp)
{
    // Check if the temp is under 28 degrees and has changed more than .25 degrees
    if (temp < 28.0 && abs(prev_temp - temp) > .25)
    {
        // Draw the cold image
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        lcd_draw_image(66, 51, cold_WidthPixels, cold_HeightPixels,
                       WarioWare_Cold_map, LCD_COLOR_BLUE, LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);

        ece353_MKII_RGB_LED(false, false, true); // turn on BLUE

        // Record the previous temp
        prev_temp = temp;
    }

    // Check if the temp is under 29 degrees and has changed more than .25 degrees
    else if (temp < 29.0 && abs(prev_temp - temp) > .25)
    {
        // Draw the cold image
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        lcd_draw_image(66, 51, cold_WidthPixels, cold_HeightPixels,
                       WarioWare_Cold_map, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
        lcd_draw_rectangle(17, 62, 3, 33, LCD_COLOR_YELLOW);

        xSemaphoreGive(LCD_Lock);

        ece353_MKII_RGB_LED(true, true, false); // turn on YELLOW

        // Record the previous temp
        prev_temp = temp;
    }

    // Check is the temp is 30 or greater
    else if (temp >= 30.0)
    {
        // Draw the warm image
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        lcd_draw_image(66, 51, warm_WidthPixels, warm_HeightPixels,
                       WarioWare_Warm_map, LCD_COLOR_RED, LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);

        ece353_MKII_RGB_LED(true, false, false); // turn on RED
    }
}

/******************************************************************************
 * Task that runs the temperature sensor minigame
 ******************************************************************************/
void Task_Minigame_2(void *pvParameters)
{
    while (1)
    {
        // Wait for the minigame to be called
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Draw the cold image
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        lcd_draw_image(66, 51, cold_WidthPixels, cold_HeightPixels,
                               WarioWare_Cold_map, LCD_COLOR_BLUE, LCD_COLOR_BLACK);

        xSemaphoreGive(LCD_Lock);

        ece353_MKII_RGB_LED(false, false, true); // turn on BLUE

        // Initialize variables needed for a new minigame
        temp = 0;
        prev_temp = 0;

        // Print the directions
        printf("Warm him up!\r\n\r\n");

        // Start the timer
        xTaskNotifyGive(Task_Timer_Handle);

        // Read the temperature value
        temp = read_temp();

        // Draw an image and turn on the RGB LED based on the temp value
        display_temp(temp);

        // Loop withe the minigame can run
        while (temp < 30 && time_available)
        {
            temp = read_temp();
            display_temp(temp);
        }

        // Check if there is time left
        if (time_available)
        {
            // Draw the warm image
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);

            lcd_draw_image(66, 51, warm_WidthPixels, warm_HeightPixels,
                           WarioWare_Warm_map, LCD_COLOR_RED, LCD_COLOR_BLACK);

            xSemaphoreGive(LCD_Lock);

            // User won the minigame
            minigame.win = true;

            vTaskDelay(pdMS_TO_TICKS(1000));

            // Send the winning queue
            xQueueSend(Queue_Minigames, &minigame, portMAX_DELAY);
        }
        // turn off the LED if it was on
        ece353_MKII_RGB_LED(false, false, false);
    }
}
