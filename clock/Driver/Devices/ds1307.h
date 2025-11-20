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
    uint8 seconds; /* 0-59 */
    uint8 minutes; /* 0-59 */
    uint8 hours;   /* 0-23, dùng 24h mode */
    uint8 day;     /* 1-7 (tùy quy ??c: 1=Sunday, ...) */
    uint8 date;    /* 1-31 */
    uint8 month;   /* 1-12 */
    uint8 year;    /* 0-99 (ví d? 25 = 2025 n?u ch? quy ??c base 2000) */
} DS1307_DateTimeType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 *  - Kh?i t?o DS1307:
 *    + B? bit CH (Clock Halt) trong thanh ghi giây ?? cho phép dao ??ng.
 *    + Ghi Control register = 0x00 (t?t SQW/OUT n?u không dùng).
 *  - Yêu c?u: TWI_init() ph?i ???c g?i tr??c ?ó.
 */
void DS1307_init(void);

/*
 * Description:
 *  - Ghi toàn b? th?i gian/ngày tháng vào DS1307, dùng 24h mode.
 *  - Giá tr? trong struct ? d?ng decimal (0–59, 0–23,...).
 */
void DS1307_setDateTime(const DS1307_DateTimeType *datetimePtr);

/*
 * Description:
 *  - ??c toàn b? th?i gian/ngày tháng t? DS1307.
 *  - Giá tr? tr? v? trong struct ? d?ng decimal (0–59, 0–23,...).
 */
void DS1307_getDateTime(DS1307_DateTimeType *datetimePtr);

/*
 * Description:
 *  - D?ng dao ??ng (set CH = 1 trong thanh ghi giây), clock s? ng?ng ch?y.
 */
void DS1307_stopClock(void);

/*
 * Description:
 *  - Cho ch?y l?i dao ??ng (clear CH = 0), gi? nguyên giá tr? giây hi?n t?i.
 */
void DS1307_startClock(void);

#endif /* DS1307_H_ */
