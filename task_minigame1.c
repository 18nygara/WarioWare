/*
 *  task_minigame1.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Minigame_1_Handle;
uint16_t lux;

/******************************************************************************
 * Initialize all hardware resources needed for Task_Minigame_1
 ******************************************************************************/
void Task_Minigame_1_Init(void)
{
    // Initialize the RGB LED
    ece353_MKII_RGB_IO_Init(false);

    // Program the CONFIG register to be in continuous transmission mode and other settings reset
    i2c_write_16(LIGHT_SENSOR_ADDR, CONFIG_REG,
                 LIGHT_RESET | LIGHT_CONTINUOUS_MODE);

    // set the high and low limits
    i2c_write_16(LIGHT_SENSOR_ADDR, LOW_LIM_REG, LIGHT_LOW);
    i2c_write_16(LIGHT_SENSOR_ADDR, HIGH_LIM_REG, LIGHT_HIGH);

    // Turn off the RGB LED
    ece353_MKII_RGB_LED(false, false, false);
}

/******************************************************************************
 * Read the light sensor and return the lux value
 ******************************************************************************/
float read_light(void)
{
    lux = i2c_read_16(LIGHT_SENSOR_ADDR, RESULT_REG);

    // convert to a lux reading
    lux = (0.01 * (1 << (lux >> 12)) * (lux % 0xF000));

    return lux;
}

/******************************************************************************
 * Task that runs the light sensor minigame
 ******************************************************************************/
void Task_Minigame_1(void *pvParameters)
{
    while (1)
    {
        // Wait for the minigame to be called
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Draw the awake screen
        xSemaphoreTake(LCD_Lock, portMAX_DELAY);

        // Initialize the variables for a new minigame
        lux = 100;

        lcd_draw_image(66, 51, awake_WidthPixels, awake_HeightPixels,
                       WarioWare_Awake_map, LCD_COLOR_BLACK, LCD_COLOR_WHITE);

        xSemaphoreGive(LCD_Lock);

        // Turn the RGB LED yellow
        ece353_MKII_RGB_LED(true, true, false);

        // Print the directions
        printf("Make it dark!\r\n\r\n");

        // Start the timer
        xTaskNotifyGive(Task_Timer_Handle);

        // Loop while the minigame can run
        while (lux > 1 && time_available)
        {
            // Read a value from the light sensor
            while ((i2c_read_16(LIGHT_SENSOR_ADDR, CONFIG_REG)
                    | LIGHT_CONVERSION_RDY) == 0)
            {
                // busy wait until a conversion is ready
            }
            lux = read_light();
        }

        // Check if there is time left
        if (time_available)
        {
            // Turn off the LED
            ece353_MKII_RGB_LED(false, false, false);

            // Draw the sleeping image
            xSemaphoreTake(LCD_Lock, portMAX_DELAY);

            lcd_draw_image(66, 51, sleep_WidthPixels, sleep_HeightPixels,
                           WarioWare_Sleep_map, LCD_COLOR_WHITE,
                           LCD_COLOR_BLACK);

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

