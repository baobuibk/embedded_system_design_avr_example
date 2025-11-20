/*
 * ds1307.c
 *
 * Created: 11/20/2025 11:25:13 PM
 *  Author: Admin
 */ 
/******************************************************************************
 *
 * Module: DS1307 RTC
 *
 * File Name: ds1307.c
 *
 * Description: Source file for DS1307 RTC driver (I2C/TWI)
 *
 * Author: (adapted)
 *
 ******************************************************************************/

#include "ds1307.h"
#include "twi.h"           /* Dùng TWI_init, TWI_start, TWI_writeByte, ... */
#include "common_macros.h" /* N?u c?n BIT macros */
#include <avr/io.h>        /* N?u c?n dùng F_CPU, v.v. */

/*******************************************************************************
 *                      Static Functions (Helpers)                             *
 *******************************************************************************/

/* Convert decimal (0-99) -> BCD */
static uint8 DEC_to_BCD(uint8 dec)
{
    return (uint8)(((dec / 10) << 4) | (dec % 10));
}

/* Convert BCD -> decimal (0-99) */
static uint8 BCD_to_DEC(uint8 bcd)
{
    return (uint8)(((bcd >> 4) * 10) + (bcd & 0x0F));
}

/*
 * Description:
 *  - Ghi 1 byte vào 1 thanh ghi c?a DS1307 (register address + value).
 */
static void DS1307_writeRegister(uint8 reg, uint8 data)
{
    TWI_start();
    TWI_writeByte(DS1307_ADDRESS_WRITE); /* SLA+W */
    TWI_writeByte(reg);                  /* Register address */
    TWI_writeByte(data);                 /* Data */
    TWI_stop();
}

/*
 * Description:
 *  - ??c 1 byte t? 1 thanh ghi c?a DS1307.
 */
static uint8 DS1307_readRegister(uint8 reg)
{
    uint8 data;

    /* Ghi ??a ch? thanh ghi tr??c */
    TWI_start();
    TWI_writeByte(DS1307_ADDRESS_WRITE); /* SLA+W */
    TWI_writeByte(reg);

    /* Repeated START + SLA+R ?? ??c */
    TWI_start();
    TWI_writeByte(DS1307_ADDRESS_READ);  /* SLA+R */
    data = TWI_readByteWithNACK();
    TWI_stop();

    return data;
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void DS1307_init(void)
{
    uint8 sec_reg;

    /* ??m b?o bit CH = 0 ?? clock ch?y (CH = bit7 trong thanh ghi giây) */
    sec_reg = DS1307_readRegister(DS1307_REG_SECONDS);
    sec_reg &= 0x7F; /* Clear bit7 (CH) */
    DS1307_writeRegister(DS1307_REG_SECONDS, sec_reg);

    /* Control register: t?t SQW/OUT n?u không dùng (0x00) */
    DS1307_writeRegister(DS1307_REG_CONTROL, 0x00);
}

void DS1307_setDateTime(const DS1307_DateTimeType *datetimePtr)
{
    uint8 sec_bcd, min_bcd, hour_bcd, day_bcd, date_bcd, month_bcd, year_bcd;

    if (datetimePtr == NULL_PTR)
    {
        return;
    }

    /* Convert decimal -> BCD, mask các bit không dùng cho ?úng format DS1307 */

    /* Seconds: bit7 = CH (0 = run, 1 = stop) */
    sec_bcd   = DEC_to_BCD(datetimePtr->seconds) & 0x7F; /* CH=0 */

    /* Minutes: bits 6-0 */
    min_bcd   = DEC_to_BCD(datetimePtr->minutes) & 0x7F;

    /* Hours (24h mode): bit6 = 0 (24h), bits5-0 = hour */
    hour_bcd  = DEC_to_BCD(datetimePtr->hours) & 0x3F;   /* 24h mode */

    /* Day (1-7) */
    day_bcd   = DEC_to_BCD(datetimePtr->day) & 0x07;

    /* Date (1-31) */
    date_bcd  = DEC_to_BCD(datetimePtr->date) & 0x3F;

    /* Month (1-12) */
    month_bcd = DEC_to_BCD(datetimePtr->month) & 0x1F;

    /* Year (0-99) */
    year_bcd  = DEC_to_BCD(datetimePtr->year);

    /* Ghi liên t?c t? thanh ghi 0x00 (Seconds) */
    TWI_start();
    TWI_writeByte(DS1307_ADDRESS_WRITE);     /* SLA+W */
    TWI_writeByte(DS1307_REG_SECONDS);       /* Start at REG 0x00 */

    TWI_writeByte(sec_bcd);                  /* 0x00 Seconds */
    TWI_writeByte(min_bcd);                  /* 0x01 Minutes */
    TWI_writeByte(hour_bcd);                 /* 0x02 Hours */
    TWI_writeByte(day_bcd);                  /* 0x03 Day */
    TWI_writeByte(date_bcd);                 /* 0x04 Date */
    TWI_writeByte(month_bcd);                /* 0x05 Month */
    TWI_writeByte(year_bcd);                 /* 0x06 Year */

    TWI_stop();
}

void DS1307_getDateTime(DS1307_DateTimeType *datetimePtr)
{
    uint8 sec_reg, min_reg, hour_reg, day_reg, date_reg, month_reg, year_reg;

    if (datetimePtr == NULL_PTR)
    {
        return;
    }

    /* ??t con tr? ??a ch? v? thanh ghi Seconds (0x00) */
    TWI_start();
    TWI_writeByte(DS1307_ADDRESS_WRITE);      /* SLA+W */
    TWI_writeByte(DS1307_REG_SECONDS);        /* Register 0x00 */

    /* Repeated START ?? chuy?n sang ??c */
    TWI_start();
    TWI_writeByte(DS1307_ADDRESS_READ);       /* SLA+R */

    /* ??c l?n l??t 7 byte (Seconds..Year) */
    sec_reg   = TWI_readByteWithACK();        /* 0x00 Seconds */
    min_reg   = TWI_readByteWithACK();        /* 0x01 Minutes */
    hour_reg  = TWI_readByteWithACK();        /* 0x02 Hours */
    day_reg   = TWI_readByteWithACK();        /* 0x03 Day */
    date_reg  = TWI_readByteWithACK();        /* 0x04 Date */
    month_reg = TWI_readByteWithACK();        /* 0x05 Month */
    year_reg  = TWI_readByteWithNACK();       /* 0x06 Year, last byte */

    TWI_stop();

    /* Mask các bit không dùng và convert BCD -> decimal */
    datetimePtr->seconds = BCD_to_DEC(sec_reg & 0x7F);
    datetimePtr->minutes = BCD_to_DEC(min_reg & 0x7F);
    datetimePtr->hours   = BCD_to_DEC(hour_reg & 0x3F); /* 24h mode */
    datetimePtr->day     = BCD_to_DEC(day_reg & 0x07);
    datetimePtr->date    = BCD_to_DEC(date_reg & 0x3F);
    datetimePtr->month   = BCD_to_DEC(month_reg & 0x1F);
    datetimePtr->year    = BCD_to_DEC(year_reg);
}

void DS1307_stopClock(void)
{
    uint8 sec_reg = DS1307_readRegister(DS1307_REG_SECONDS);
    sec_reg |= 0x80; /* set CH = 1 */
    DS1307_writeRegister(DS1307_REG_SECONDS, sec_reg);
}

void DS1307_startClock(void)
{
    uint8 sec_reg = DS1307_readRegister(DS1307_REG_SECONDS);
    sec_reg &= 0x7F; /* clear CH = 0 */
    DS1307_writeRegister(DS1307_REG_SECONDS, sec_reg);
}
