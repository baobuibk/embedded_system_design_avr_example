/*
 * bsp_rtc.h
 *
 * Created: 11/20/2025 11:36:55 PM
 *  Author: Admin
 */ 


#ifndef BSP_RTC_H_
#define BSP_RTC_H_

#include "ds1307.h"
#include "twi.h"

void bsp_rtc_init(DS1307_DateTimeType *init);

void bsp_rtc_get_date_time(DS1307_DateTimeType *pDateTime);

#endif /* BSP_RTC_H_ */