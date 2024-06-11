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

#ifndef _RTC_H
#define _RTC_H

//! Fields used by RTC to describe date and time
struct RTCDateTime
{
	unsigned char year; //!< year
	unsigned char month; //!< month
	unsigned char day; //!< day
	unsigned char hour; //!< hour
	unsigned char minute; //!< minute
	unsigned char second; //!< second
};

void rtcInit(void);

void rtcSetDateTime(struct RTCDateTime dateTime);
struct RTCDateTime rtcGetDateTime(void);

#endif
