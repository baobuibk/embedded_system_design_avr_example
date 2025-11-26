/******************************************************************************
 *
 * Module: DS1307 RTC
 *
 * File Name: ds1307.h
 *
 * Description: Header file for DS1307 RTC driver (I2C/TWI)
 *
 * Author:
 *
 ******************************************************************************/

#ifndef DS1307_H_
#define DS1307_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* DS1307 7-bit address = 0x68 -> write = 0xD0, read = 0xD1 */
#define DS1307_ADDRESS_WRITE   0xD0
#define DS1307_ADDRESS_READ    0xD1

#define DS1307_REG_SECONDS     0x00
#define DS1307_REG_MINUTES     0x01
#define DS1307_REG_HOURS       0x02
#define DS1307_REG_DAY         0x03
#define DS1307_REG_DATE        0x04
#define DS1307_REG_MONTH       0x05
#define DS1307_REG_YEAR        0x06
#define DS1307_REG_CONTROL     0x07

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef struct
{
    uint8 seconds; /* 0–59 */
    uint8 minutes; /* 0–59 */
    uint8 hours;   /* 0–23, using 24-hour mode */
    uint8 day;     /* 1–7 (depending on convention, e.g. 1 = Sunday, ...) */
    uint8 date;    /* 1–31 */
    uint8 month;   /* 1–12 */
    uint8 year;    /* 0–99 (e.g. 25 = year 2025 if you choose base year 2000) */
} DS1307_DateTimeType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 *  - Initialize the DS1307:
 *    + Clear the CH (Clock Halt) bit in the seconds register to allow the
 *      oscillator to run.
 *    + Write Control register = 0x00 (disable SQW/OUT if not used).
 *  - Requirement: TWI_init() must be called beforehand.
 */
void DS1307_init(void);

/*
 * Description:
 *  - Write the full date/time into the DS1307, using 24-hour format.
 *  - Field values in the struct are in decimal (0–59, 0–23, etc.).
 */
void DS1307_setDateTime(const DS1307_DateTimeType *datetimePtr);

/*
 * Description:
 *  - Read the full date/time from the DS1307.
 *  - Field values returned in the struct are in decimal (0–59, 0–23, etc.).
 */
void DS1307_getDateTime(DS1307_DateTimeType *datetimePtr);

/*
 * Description:
 *  - Stop the DS1307 oscillator (set CH = 1 in the seconds register).
 *    The clock will stop running.
 */
void DS1307_stopClock(void);

/*
 * Description:
 *  - Restart the DS1307 oscillator (clear CH = 0 in the seconds register),
 *    keeping the current seconds value.
 */
void DS1307_startClock(void);

#endif /* DS1307_H_ */
