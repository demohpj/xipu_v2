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

#include "led.h"

/**
 * Constructor for the status LED class
 *
 * @param parent Parent object
 */
LED::LED(QObject *parent) : QObject(parent)
{
	this->reset();
}

//! Reset the statuses of LEDs
void LED::reset()
{
	this->run = false;
	this->error = false;
}

/**
 * Set enable status of the run LED
 *
 * @param enable Enable status
 */
void LED::setRun(bool enable)
{
	this->run = enable;
	emit updateRunSignal(this->run);
}

/**
 * Set enable status of the error LED
 *
 * @param enable Enable status
 */
void LED::setError(bool enable)
{
	this->error = enable;
	emit updateErrorSignal(this->error);
}

/**
 * Get enable status of the run LED
 *
 * @return Enable status
 */
bool LED::getRun()
{
	return(this->run);
}

/**
 * Get enable status of the error LED
 *
 * @return Enable status
 */
bool LED::getError()
{
	return(this->error);
}
