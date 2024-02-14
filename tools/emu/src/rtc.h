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

#ifndef RTC_H
#define RTC_H

#include <QObject>
#include <QDateTime>

//! This class contains Real Time Clock functions
class RTC : public QObject
{
	Q_OBJECT

	public:
		//! Fields used by RTC to describe date and time
		struct DateTime
		{
			unsigned char year; //!< year
			unsigned char month; //!< month
			unsigned char day; //!< day
			unsigned char hour; //!< hour
			unsigned char minute; //!< minute
			unsigned char second; //!< second
		};

		RTC(QObject *parent = nullptr);

		void reset();

		void setDateTime(const QDateTime &dateTime);
		void setDateTime(const RTC::DateTime &dateTime);

		RTC::DateTime getDateTime();

	private:
		QDateTime dateTime; //!< Current date and time

	signals:
		void updateDateTimeSignal(QDateTime dateTime);
};

#endif
