/*
 * bsp_rtc.c
 *
 * Created: 11/20/2025 11:39:56 PM
 *  Author: Admin
 */ 

#include "bsp_rtc.h"

void bsp_rtc_init(DS1307_DateTimeType * init)
{
	    TWI_init();      /* Dùng driver I2C c?a ch? */
	    DS1307_init();   /* Kh?i t?o DS1307 */
	    DS1307_setDateTime(init);
}

void bsp_rtc_get_date_time(DS1307_DateTimeType *pDateTime)
{
	return DS1307_getDateTime(pDateTime);

}