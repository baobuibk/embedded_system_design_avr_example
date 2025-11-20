/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver (ATmega324, USART0)
 *
 * Author: Esraa Ali  (adapted for ATmega324)
 *
 ******************************************************************************/

#include "uart.h"
#include "avr/io.h"        /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like BIT_IS_CLEAR, SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 *
 * Configuration m?c ??nh: USART0, Asynchronous, 8 data bits, no parity, 1 stop bit, double speed.
 */
void UART_init(uint32 baud_rate, uint32 cpu_freq)
{
    uint16 ubrr_value = 0;

    /* U2X0 = 1 for double transmission speed on USART0 */
    UCSR0A = (1 << U2X0);

    /************************** UCSR0B Description **************************
     * RXCIE0 = 0 Disable USART RX Complete Interrupt Enable
     * TXCIE0 = 0 Disable USART Tx Complete Interrupt Enable
     * UDRIE0 = 0 Disable USART Data Register Empty Interrupt Enable
     * RXEN0  = 1 Receiver Enable
     * TXEN0  = 1 Transmitter Enable
     * UCSZ02 = 0 For 8-bit data mode (together with UCSZ01:0 in UCSR0C)
     ***********************************************************************/
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* N?u mu?n 9-bit data: set thêm UCSZ02 = 1 và x? lý RXB80/TXB80 */

    /************************** UCSR0C Description **************************
     * UMSEL01:0 = 00 Asynchronous Operation
     * UPM01:0   = 00 Disable parity bit (No parity)
     * USBS0     = 0 One stop bit
     * UCSZ01:0  = 11 For 8-bit data mode
     * UCPOL0    = 0 Used with the Synchronous operation only
     *
     * L?u ý: ATmega324 không có bit URSEL, ghi tr?c ti?p vào UCSR0C.
     ***********************************************************************/
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    /* Calculate the UBRR0 register value (double-speed mode) */
    ubrr_value = (uint16)((cpu_freq / (baud_rate * 8UL)) - 1);

    /* High bits in UBRR0H and low bits in UBRR0L */
    UBRR0H = (uint8)(ubrr_value >> 8);
    UBRR0L = (uint8)ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
    /*
     * UDRE0 flag is set when the Tx buffer (UDR0) is empty and ready for
     * transmitting a new byte so wait until this flag is set to one
     */
    while (BIT_IS_CLEAR(UCSR0A, UDRE0)) {}

    /*
     * Put the required data in the UDR0 register and it also clears the UDRE0 flag as
     * the UDR0 register is not empty now
     */
    UDR0 = data;

    /************************* Another Method *************************
    UDR0 = data;
    while (BIT_IS_CLEAR(UCSR0A, TXC0)) {} // Wait until the transmission is complete TXC0 = 1
    SET_BIT(UCSR0A, TXC0); // Clear the TXC0 flag
    *******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
    /* RXC0 flag is set when the UART0 receives data so wait until this flag is set to one */
    while (BIT_IS_CLEAR(UCSR0A, RXC0)) {}

    /*
     * Read the received data from the Rx buffer (UDR0)
     * The RXC0 flag will be cleared after reading the data
     */
    return UDR0;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
    uint8 i = 0;

    /* Send the whole string */
    while (Str[i] != '\0')
    {
        UART_sendByte(Str[i]);
        i++;
    }
    /************************* Another Method *************************
    while (*Str != '\0')
    {
        UART_sendByte(*Str);
        Str++;
    }
    *******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
    uint8 i = 0;

    /* Receive the first byte */
    Str[i] = UART_recieveByte();

    /* Receive the whole string until the '#' */
    while (Str[i] != '#')
    {
        i++;
        Str[i] = UART_recieveByte();
    }

    /* After receiving the whole string plus the '#', replace the '#' with '\0' */
    Str[i] = '\0';
}
