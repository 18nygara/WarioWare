/*
 *  i2c.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include "i2c.h"

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_set_slave_address(uint8_t slave_address)
{
    EUSCI_B1->I2CSA = slave_address;
}
/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_tx_byte(uint8_t data)
{
    EUSCI_B1->TXBUF = data;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_set_tx_mode(void)
{
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_set_rx_mode(void)
{
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_TR;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_send_start(void)
{
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_send_stop(void)
{
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_wait_for_tx(void)
{
    while((EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0) == 0){

    };
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_wait_for_stop(void)
{
    while((EUSCI_B1->IFG & EUSCI_B_IFG_STPIFG) == 0){

    };

    EUSCI_B1->IFG &= ~EUSCI_B_IFG_STPIFG;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline uint8_t i2c_wait_for_rx(void)
{
    while((EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG0) == 0) {

    };

    return EUSCI_B1->RXBUF;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_wait_busy(void)
{
    while(EUSCI_B1->STATW & EUSCI_B_STATW_BBUSY){};
}

/**********************************************************************************************
 *
 **********************************************************************************************/
static __inline void i2c_clear_interrupts(void)
{
    EUSCI_B1->IFG = 0;
}

/**********************************************************************************************
 *
 **********************************************************************************************/
void i2c_init(void)
{
    P6->SEL0 |= (BIT5 | BIT4);                // I2C pins
    P6->SEL1 &= ~(BIT5 | BIT4);                // I2C pins

    EUSCI_B1->CTLW0 |=  EUSCI_A_CTLW0_SWRST;            // Software reset enabled

    EUSCI_B1->CTLW0 =   EUSCI_A_CTLW0_SWRST |           // Remain eUSCI in reset mode
                        EUSCI_B_CTLW0_MODE_3 |          // I2C mode
                        EUSCI_B_CTLW0_MST |             // Master mode
                        EUSCI_B_CTLW0_SYNC |            // Sync mode
                        EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK

    EUSCI_B1->BRW =     SystemCoreClock/100000;         // baudrate = SMCLK / ? = 100kHz

    EUSCI_B1->CTLW0 &=  ~EUSCI_A_CTLW0_SWRST;           // Release eUSCI from reset
}



/**********************************************************************************************
 *
 **********************************************************************************************/
void i2c_write_16(uint8_t slave_address, uint8_t dev_address, uint16_t data)
{

    // Wait until the I2C bus is not being used
    i2c_wait_busy();

    // Put in Transmit mode
    i2c_set_tx_mode();

    // clear any outstanding interrupts
    i2c_clear_interrupts();

    // Set the Slave Address
    i2c_set_slave_address(slave_address);

    // Send the Start Bit and I2C Address
    i2c_send_start();

    // Wait for Tx buffer to become available
    i2c_wait_for_tx();

    // Send device address
    i2c_tx_byte(dev_address);

    // Wait for Tx buffer to become available.
    i2c_wait_for_tx();

    // Send upper byte of data
    i2c_tx_byte(data>>8);

    // Wait for Tx buffer to become available
    i2c_wait_for_tx();

    // send lower byte of data
    i2c_tx_byte(data);

    i2c_wait_for_tx();

    // Send the STOP Bit
    i2c_send_stop();

    // Wait until the last byte is transmitted
    i2c_wait_for_stop();

}

/**********************************************************************************************
 *
 **********************************************************************************************/
uint16_t i2c_read_16(uint8_t slave_address, uint8_t dev_address)
{
    uint8_t upper_byte;
    uint8_t lower_byte;

    i2c_wait_busy();

    // Put in Transmit mode
    i2c_set_tx_mode();

    // clear interrupts
    i2c_clear_interrupts();

    // Set the Slave Address
    i2c_set_slave_address(slave_address);

    // Send the Start Bit and I2C Address
    i2c_send_start();

    // Wait for Tx buffer to become available
    i2c_wait_for_tx();

    // Send device address
    i2c_tx_byte(dev_address);

    i2c_wait_for_tx();

    // Send the STOP Bit
    i2c_send_stop();

    i2c_wait_for_stop();

    // Put in Receive mode
    i2c_set_rx_mode();

    // Send the Start Bit and I2C Address
    i2c_send_start();

    // Wait for the first byte
    upper_byte = i2c_wait_for_rx();

    // Send a NACK and STOP Bit
    i2c_send_stop();

    // Wait for the 2nd byte
    lower_byte = i2c_wait_for_rx();

    i2c_wait_for_stop();

    return ((uint16_t)(upper_byte) << 8) | lower_byte;

}
