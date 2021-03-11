/*
 *  task_console.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

#define RX_ARRAY_SIZE 100


/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 *
 * The configuration should be identical to what you have done
 * in a previous ICE-08 EXCEPT that you will need to set the priority of the
 * eUSCI to be a value that is equal to 2
 ******************************************************************************/
static void console_hw_init(void)
{
    // change pins 1.2 and 1.3 to secondary function
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);
    // put eUSCI in reset mode
    // remain eUSCI in reset, configure eUSCI clock source
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | EUSCI_A_CTLW0_SSEL__SMCLK;
    // baud rate calculation
    // 24000000/(16*115200) = 13.02083333
    // Fractional portion = 0.02083333
    // UCBRFx = int ( (13.0208333 - 13) * 16 ) = 0
    EUSCI_A0->BRW = 13;
    // set the fractional portion of the baud rate and turn on oversampling
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;
    // enable eUSCI in UART mode
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
    // clear any outstanding interrupts
    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG | EUSCI_A_IFG_TXIFG);
    //enable Rx interrupts
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    // enable EUSCIA0 Interrupt
    NVIC_EnableIRQ(EUSCIA0_IRQn);
    // change the priority
    NVIC_SetPriority(EUSCIA0_IRQn, 2);
}

/******************************************************************************
 * This function initializes the eUSCI_A0 hardware by calling console_hw_init().
 * It will also initialize Sem_Console.
 *
 * NOTE:  This function must be run prior to the FreeRTOS scheduler being
 * started.
 ******************************************************************************/
void Task_Console_Init(void)
{
    // Initialize UART
    console_hw_init();
}

/*****************************************************
 * Needed to get printf to work using polling when
 * transmitting characters.
 *****************************************************/
int fputc(int c, FILE* stream)
{
    // while UART is busy, wait
    while(EUSCI_A0->STATW & EUSCI_A_STATW_BUSY){};

    // echo the character to the UART
    EUSCI_A0->TXBUF = c;

    return 0;
}
