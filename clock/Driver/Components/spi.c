/******************************************************************************
 *
 * Module: SPI
 *
 * File Name: spi.c
 *
 * Description: Source file for the SPI AVR driver
 *              (ATmega324PB - dùng SPI0: SPCR0, SPSR0, SPDR0)
 *
 * Author: Esraa Ali (adapted)
 *
 ******************************************************************************/

#include "spi.h"
#include "gpio.h"          /* To Setup the SPI pins direction */
#include "common_macros.h" /* To use the macros like SET_BIT, BIT_IS_CLEAR */
#include "avr/io.h"        /* To use the SPI Registers */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the SPI device as Master.
 */
void SPI_initMaster(void)
{
    /******** Configure SPI Master Pins *********
     * L?U Ý: check l?i ?úng chân SPI0 c?a ATmega324PB theo schematic board c?a ch?.
     * Ví d? code g?c th??ng dùng mapping ki?u ATmega16/32:
     *   SS   -> PB4
     *   MOSI -> PB5
     *   MISO -> PB6
     *   SCK  -> PB7
     ********************************************/
    GPIO_setupPinDirection(PORTB_ID, PIN4_ID, PIN_OUTPUT); /* SS */
    GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT); /* MOSI */
    GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_INPUT);  /* MISO */
    GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_OUTPUT); /* SCK  */

    /* Optionally pull SS high in master mode */
    // GPIO_writePin(PORTB_ID, PIN4_ID, LOGIC_HIGH);

    /************************** SPCR0 Description **************************
     * SPIE0   = 0 Disable SPI Interrupt
     * SPE0    = 1 Enable SPI Driver
     * DORD0   = 0 Transmit the MSB first
     * MSTR0   = 1 Enable Master
     * CPOL0   = 0 SCK is low when idle
     * CPHA0   = 0 Sample Data with the rising edge
     * SPR01:0 = 00 Choose SPI clock = Fosc/4
     ***********************************************************************/
    SPCR0 = (1 << SPE0) | (1 << MSTR0);

    /* Clear the SPI2X0 bit in SPSR0 to choose SPI clock = Fosc/4 */
    SPSR0 &= ~(1 << SPI2X0);
}

/*
 * Description :
 * Initialize the SPI device as Slave.
 */
void SPI_initSlave(void)
{
    /******** Configure SPI Slave Pins *********
     * SS   -> Input
     * MOSI -> Input
     * MISO -> Output
     * SCK  -> Input
     ********************************************/
    GPIO_setupPinDirection(PORTB_ID, PIN4_ID, PIN_INPUT);  /* SS */
    GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_INPUT);  /* MOSI */
    GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_OUTPUT); /* MISO */
    GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_INPUT);  /* SCK  */

    /************************** SPCR0 Description **************************
     * SPIE0   = 0 Disable SPI Interrupt
     * SPE0    = 1 Enable SPI Driver
     * DORD0   = 0 Transmit the MSB first
     * MSTR0   = 0 Slave mode
     * CPOL0   = 0 SCK is low when idle
     * CPHA0   = 0 Sample Data with the rising edge
     * SPR01:0 = 00 (don't care in Slave mode)
     ***********************************************************************/
    SPCR0 = (1 << SPE0);

    /* Clear SPI2X0 bit (không ?nh h??ng trong Slave nh?ng ?? c?u hình s?ch) */
    SPSR0 &= ~(1 << SPI2X0);
}

/*
 * Description :
 * Send the required data through SPI to the other SPI device.
 * In the same time data will be received from the other device.
 */
uint8 SPI_sendReceiveByte(uint8 data)
{
    /* Initiate the communication and send data by SPI */
    SPDR0 = data;

    /* Wait until SPI interrupt flag SPIF0 = 1 (data has been sent/received correctly) */
    while (BIT_IS_CLEAR(SPSR0, SPIF0)) {}

    /*
     * Note: SPIF0 flag is cleared by first reading SPSR0 (with SPIF0 set),
     * and then accessing SPDR0 like the below line.
     */
    return SPDR0;
}

/*
 * Description :
 * Send the required string through SPI to the other SPI device.
 */
void SPI_sendString(const uint8 *str)
{
    uint8 i = 0;
    uint8 received_data;

    /* Send the whole string */
    while (str[i] != '\0')
    {
        /* Dummy read for received data, just to keep the interface consistent */
        received_data = SPI_sendReceiveByte(str[i]);
        (void)received_data; /* tránh warning bi?n không dùng */
        i++;
    }
}

/*
 * Description :
 * Receive the required string until the '#' symbol through SPI from the other SPI device.
 */
void SPI_receiveString(uint8 *str)
{
    uint8 i = 0;

    /* Receive the first byte */
    str[i] = SPI_sendReceiveByte(SPI_DEFAULT_DATA_VALUE);

    /* Receive the whole string until the '#' */
    while (str[i] != '#')
    {
        i++;
        str[i] = SPI_sendReceiveByte(SPI_DEFAULT_DATA_VALUE);
    }

    /* After receiving the whole string plus the '#', replace the '#' with '\0' */
    str[i] = '\0';
}
