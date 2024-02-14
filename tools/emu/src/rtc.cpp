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

#include "rtc.h"

//! Constructor for Real Time Clock functions
RTC::RTC(QObject *parent) : QObject(parent)
{
	this->reset();
}

//! Reset RTC to default values
void RTC::reset()
{
	this->dateTime.setTime(QTime(0, 0, 0));
	this->dateTime.setDate(QDate(2000, 1, 1));
}

/**
 * Set date and time
 *
 * @param dateTime Date and time to set in Qt format
 */
void RTC::setDateTime(const QDateTime &dateTime)
{
	this->dateTime = dateTime;

	emit updateDateTimeSignal(this->dateTime);
}

/**
 * Set date and time
 *
 * @param dateTime Date and time to set in own RTC format
 */
void RTC::setDateTime(const RTC::DateTime &dateTime)
{
	this->dateTime.setTime(QTime(dateTime.hour, dateTime.minute, dateTime.second));
	this->dateTime.setDate(QDate((static_cast<int>(dateTime.year) + 2000), dateTime.month, dateTime.day));

	emit updateDateTimeSignal(this->dateTime);
}

/**
 * Get date and time
 *
 * @return Date and time in own RTC format
 */
RTC::DateTime RTC::getDateTime()
{
	RTC::DateTime dateTime = {};

	dateTime.year = static_cast<unsigned char>(this->dateTime.date().year() % 100);
	dateTime.month = static_cast<unsigned char>(this->dateTime.date().month());
	dateTime.day = static_cast<unsigned char>(this->dateTime.date().day());

	dateTime.hour = static_cast<unsigned char>(this->dateTime.time().hour());
	dateTime.minute = static_cast<unsigned char>(this->dateTime.time().minute());
	dateTime.second = static_cast<unsigned char>(this->dateTime.time().second());

	return(dateTime);
}
