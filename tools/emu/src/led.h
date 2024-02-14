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

#ifndef LED_H
#define LED_H

#include <QObject>

//! This class contains status LED functions
class LED : public QObject
{
	Q_OBJECT

	public:
		LED(QObject *parent = nullptr);

		void reset();

		void setRun(bool enable);
		void setError(bool enable);

		bool getRun();
		bool getError();

	private:
		bool run; //!< Run LED status
		bool error; //!< Error LED status

	signals:
		void updateRunSignal(bool enable);
		void updateErrorSignal(bool enable);
};

#endif
