/*
 *  arrow.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

TaskHandle_t Task_Joystick_Handle;
TaskHandle_t Task_Joystick_Timer_Handle;
QueueHandle_t Queue_Joystick;
QueueHandle_t Queue_Accelerometer;
JOYSTICK_t joystick;

volatile uint32_t JOYSTICK_X_DIR = 0;
volatile uint32_t JOYSTICK_Y_DIR = 0;
volatile uint32_t ACCELER_X = 0;
volatile uint32_t ACCELER_Y = 0;
volatile uint32_t ACCELER_Z = 0;
uint32_t ACCELER_ARR[3];


/******************************************************************************
 * This function will run the same configuration code that you would have
 * written for HW02.
 ******************************************************************************/
void Task_Joystick_Init(void)
{
    // Configure the X direction as an analog input pin.
    P6->SEL0 |= BIT0;
    P6->SEL1 &= ~BIT0;

    // Configure the Y direction as an analog input pin.
    P4->SEL0 |= BIT4;
    P4->SEL1 &= ~BIT4;

    // Configure the X accelerometer as an analog input pin.
    P6->SEL0 |= BIT1;
    P6->SEL1 &= ~BIT1;

    // Configure the Y accelerometer as an analog input pin.
    P4->SEL0 |= BIT0;
    P4->SEL1 &= ~BIT0;

    // Configure the Z accelerometer as an analog input pin.
    P4->SEL0 |= BIT2;
    P4->SEL1 |= BIT2;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    // NEW -- Indicate that this is a sequence of samples.
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1;

    // Configure CTL1 to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2; // Use sampling timer, 12-bit conversion results

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_15;

    // Associate the Y direction analog signal with MEM[1]
    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_9;

    // Associate the X accelerometer signal with MEM[2]
    ADC14->MCTL[2] |= ADC14_MCTLN_INCH_14;

    // Associate the Y accelerometer signal with MEM[3]
    ADC14->MCTL[3] |= ADC14_MCTLN_INCH_13;

    // Associate the Z accelerometer signal with MEM[4], end of sequence
    ADC14->MCTL[4] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_EOS;

    // Enable interrupts AFTER a value is written into MEM[1].
    ADC14->IER0 |= ADC14_IER0_IE1;

    // Enable ADC Interrupt
    NVIC_EnableIRQ(ADC14_IRQn);

    // Interrupt priorities must NOT be set to 0.  It is preferred if they are set to a value that is > than 1 too.
    NVIC_SetPriority(ADC14_IRQn, 2);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;

    // create the queue and set the position of the joystick to center
    Queue_Joystick = xQueueCreate(1, sizeof(JOYSTICK_t));
    Queue_Accelerometer = xQueueCreate(1, sizeof(ACCELER_ARR));
    joystick = JOYSTICK_CENTER;

}

/******************************************************************************
 * Used to start an ADC14 Conversion
 * Was given in HW03 so we will utilize the same structure here
 ******************************************************************************/
void Task_Joystick_Timer(void *pvParameters)
{
    while (1)
    {
        // don't do ADC conversions unless our minigame requires it
        if (allow_ADC)
        {
            // start an ADC conversion
            ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;
            // delay 5ms
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
}

/******************************************************************************
 * Bottom Half Task.  Examines the ADC data from the joy stick on the MKII
 * Was given in HW03 so we will utilize the same structure here
 ******************************************************************************/
void Task_Joystick_Bottom_Half(void *pvParameters)
{
    while (1)
    {
        // Wait until a notification is received from the ADC14 ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Check if the X or Y values are out of center position
        if (JOYSTICK_X_DIR < VOLT_0P85)
        {
            // Set the Joystick direction
            joystick = JOYSTICK_LEFT;
        }
        else if (JOYSTICK_X_DIR > VOLT_2P50)
        {
            // Set the Joystick direction
            joystick = JOYSTICK_RIGHT;
        }
        else if (JOYSTICK_Y_DIR < VOLT_0P85)
        {
            // Set the Joystick direction
            joystick = JOYSTICK_DOWN;
        }
        else if (JOYSTICK_Y_DIR > VOLT_2P50)
        {
            // Set the Joystick direction
            joystick = JOYSTICK_UP;
        }
        else
        {
            // Set the Joystick direction
            joystick = JOYSTICK_CENTER;
        }

        // put the accelerometer readings into the array to be returned
        ACCELER_ARR[0] = ACCELER_X;
        ACCELER_ARR[1] = ACCELER_Y;
        ACCELER_ARR[2] = ACCELER_Z;

        // Send a joystick movement queue
        xQueueSend(Queue_Joystick, &joystick, portMAX_DELAY);
        // send a Accelerometer movement queue
        xQueueSend(Queue_Accelerometer, &ACCELER_ARR, portMAX_DELAY);
    }
}

/******************************************************************************
 * Top Half of ADC14 Handler.
 ******************************************************************************/
void ADC14_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    JOYSTICK_X_DIR = ADC14->MEM[0]; // Read the value and clear the interrupt
    JOYSTICK_Y_DIR = ADC14->MEM[1]; // Read the value and clear the interrupt
    ACCELER_X = ADC14->MEM[2]; // Read the value and clear the interrupt
    ACCELER_Y = ADC14->MEM[3]; // Read the value and clear the interrupt
    ACCELER_Z = ADC14->MEM[4]; // Read the value and clear the interrupt

    // Send a task notification to Task_Joystick_Bottom_Half
    vTaskNotifyGiveFromISR(Task_Joystick_Handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

