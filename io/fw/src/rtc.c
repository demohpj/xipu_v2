/*
 * Author: Pawel Jablonski
 * E-mail: pj@xirx.net
 * WWW: xirx.net
 * GIT: git.xirx.net
 *
 * License: You can use this code however you like
 * but leave information about the original author.
 * Code is free for non-commercial and commercial use.
 */

#include <stm32f0xx.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include <stm32f0xx_hal_rtc.h>
#include <stm32f0xx_hal_rcc.h>
#include <system_stm32f0xx.h>

#include "rtc.h"

static RTC_HandleTypeDef rtcHandle; //!< RTC HAL handle

//! Init RTC and set up the RTC HAL handle
void rtcInit(void)
{
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_RTC_ENABLE();
	
	rtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	rtcHandle.Init.AsynchPrediv = 127;
	rtcHandle.Init.SynchPrediv = 255;
	rtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	rtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	rtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	rtcHandle.Instance = RTC;
	
	HAL_RTC_Init(&rtcHandle);
}

/**
 * Set date and time
 *
 * @param dateTime Date and time to set
 */
void rtcSetDateTime(struct RTCDateTime dateTime)
{
	RTC_TimeTypeDef rtcTime = {0};
	RTC_DateTypeDef rtcDate = {0};
	
	rtcTime.Hours = dateTime.hour;
	rtcTime.Minutes = dateTime.minute;
	rtcTime.Seconds = dateTime.second;
	rtcTime.TimeFormat = RTC_HOURFORMAT_24;
	rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtcTime.StoreOperation = RTC_STOREOPERATION_RESET;
	
	rtcDate.Year = dateTime.year;
	rtcDate.Month = dateTime.month;
	rtcDate.Date = dateTime.day;
	
	HAL_RTC_SetTime(&rtcHandle, &rtcTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&rtcHandle, &rtcDate, RTC_FORMAT_BIN);
}

/**
 * Get date and time
 *
 * @return Date and time
 */
struct RTCDateTime rtcGetDateTime(void)
{
	RTC_TimeTypeDef rtcTime = {0};
	RTC_DateTypeDef rtcDate = {0};
	
	HAL_RTC_GetTime(&rtcHandle, &rtcTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&rtcHandle, &rtcDate, RTC_FORMAT_BIN);
	
	struct RTCDateTime dateTime;
	
	dateTime.hour = rtcTime.Hours;
	dateTime.minute = rtcTime.Minutes;
	dateTime.second = rtcTime.Seconds;
	
	dateTime.year = rtcDate.Year;
	dateTime.month = rtcDate.Month;
	dateTime.day = rtcDate.Date;
	
	return(dateTime);
}
