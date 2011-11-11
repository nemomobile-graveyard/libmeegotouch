/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "mcalendar.h"
#include "mlocale/mcalendar.h"

#include <QStringList>

#include "mlocale_proxy_p.h"
#include "mcalendar_proxy_p.h"

static bool g_mLocaleInited = false;

MCalendarPrivate::MCalendarPrivate() : pCalendar( 0 )
{
    if ( ! g_mLocaleInited )
    {
        MLocale::initMLocale();
        g_mLocaleInited = true;
    }
}

MCalendarPrivate::~MCalendarPrivate()
{
    delete pCalendar;
}


MCalendar::MCalendar(MLocale::CalendarType calendarType,
                     const QString &timezone)
    : d_ptr(new MCalendarPrivate)
{
    d_ptr->pCalendar = new ML10N::MCalendar( (ML10N::MLocale::CalendarType)calendarType, timezone );
}

MCalendar::MCalendar(const MLocale &mLocale, const QString &timezone)
    : d_ptr(new MCalendarPrivate)
{
    d_ptr->pCalendar = new ML10N::MCalendar( *mLocale.d_ptr->pLocale, timezone );
}

MCalendar::MCalendar(const MCalendar &other)
    : d_ptr(new MCalendarPrivate)
{
    d_ptr->pCalendar = new ML10N::MCalendar( *other.d_ptr->pCalendar );
}

MCalendar::~MCalendar()
{
    delete d_ptr;
}

MCalendar &MCalendar::operator=(const MCalendar &other)
{
    *d_ptr->pCalendar = *other.d_ptr->pCalendar;
    return *this;
}

bool MCalendar::isValid() const
{
    return d_ptr->pCalendar->isValid();
}

MLocale::CalendarType MCalendar::type() const
{
    return (MLocale::CalendarType)d_ptr->pCalendar->type();
}

void MCalendar::setDate(int year, int month, int day)
{
    d_ptr->pCalendar->setDate( year, month, day );
}

void MCalendar::setDate(const QDate &date)
{
    d_ptr->pCalendar->setDate( date );
}

void MCalendar::setDateTime(QDateTime dateTime)
{
    d_ptr->pCalendar->setDateTime( dateTime );
}

QDateTime MCalendar::qDateTime(Qt::TimeSpec spec) const
{
    return d_ptr->pCalendar->qDateTime( spec );
}

void MCalendar::setYear(int year)
{
    d_ptr->pCalendar->setYear( year );
}

void MCalendar::setMonth(int month)
{
    d_ptr->pCalendar->setMonth( month );
}

void MCalendar::setDay(int day)
{
    d_ptr->pCalendar->setDay( day );
}

int MCalendar::dayOfYear() const
{
    return d_ptr->pCalendar->dayOfYear();
}

int MCalendar::weekOfYear() const
{
    return d_ptr->pCalendar->weekOfYear();
}

int MCalendar::month() const
{
    return d_ptr->pCalendar->month();
}

int MCalendar::year() const
{
    return d_ptr->pCalendar->year();
}

int MCalendar::yearOfWeek() const
{
    return d_ptr->pCalendar->yearOfWeek();
}

int MCalendar::dayOfMonth() const
{
    return d_ptr->pCalendar->dayOfMonth();
}

int MCalendar::dayOfWeek() const
{
    return d_ptr->pCalendar->dayOfWeek();
}

void MCalendar::setHours(int hours)
{
    d_ptr->pCalendar->setHours( hours );
}

void MCalendar::setMinutes(int minutes)
{
    d_ptr->pCalendar->setMinutes( minutes );
}

void MCalendar::setSeconds(int seconds)
{
    d_ptr->pCalendar->setSeconds( seconds );
}

void MCalendar::setTime(int hours, int minutes, int seconds)
{
    d_ptr->pCalendar->setTime( hours, minutes, seconds );
}


int MCalendar::hour() const
{
    return d_ptr->pCalendar->hour();
}

int MCalendar::minute() const
{
    return d_ptr->pCalendar->minute();
}

int MCalendar::second() const
{
    return d_ptr->pCalendar->second();
}

void MCalendar::addYears(int years)
{
    d_ptr->pCalendar->addYears( years );
}

void MCalendar::addMonths(int months)
{
    d_ptr->pCalendar->addMonths( months );
}

void MCalendar::addDays(int days)
{
    d_ptr->pCalendar->addDays( days );
}

void MCalendar::addHours(int hours)
{
    d_ptr->pCalendar->addHours( hours );
}

void MCalendar::addMinutes(int minutes)
{
    d_ptr->pCalendar->addMinutes( minutes );
}

void MCalendar::addSeconds(int seconds)
{
    d_ptr->pCalendar->addSeconds( seconds );
}

int MCalendar::firstDayOfMonth() const
{
    return d_ptr->pCalendar->firstDayOfMonth();
}

int MCalendar::lastDayOfMonth() const
{
    return d_ptr->pCalendar->lastDayOfMonth();
}

void MCalendar::setFirstDayOfWeek(int weekday)
{
    d_ptr->pCalendar->setFirstDayOfWeek( weekday );
}

int MCalendar::firstDayOfWeek() const
{
    return d_ptr->pCalendar->firstDayOfWeek();
}

void MCalendar::setMinimalDaysInFirstWeek(int days)
{
    d_ptr->pCalendar->setMinimalDaysInFirstWeek( days );
}

int MCalendar::minimalDaysInFirstWeek() const
{
    return d_ptr->pCalendar->minimalDaysInFirstWeek();
}

MLocale::WeekdayType MCalendar::getDayOfWeekType(MLocale::Weekday weekday) const
{
    return (MLocale::WeekdayType)d_ptr->pCalendar->getDayOfWeekType( (ML10N::MLocale::Weekday)weekday );
}

qint32 MCalendar::getWeekendTransition(MLocale::Weekday weekday) const
{
    return d_ptr->pCalendar->getWeekendTransition( (ML10N::MLocale::Weekday)weekday );
}

int MCalendar::weekNumber() const
{
    return d_ptr->pCalendar->weekNumber();
}

int MCalendar::maximumWeeksInMonth() const
{
    return d_ptr->pCalendar->maximumWeeksInMonth();
}

int MCalendar::daysInWeek() const
{
    return d_ptr->pCalendar->daysInWeek();
}

bool MCalendar::after(const MCalendar &other) const
{
    return d_ptr->pCalendar->after( *other.d_ptr->pCalendar );
}

bool MCalendar::before(const MCalendar &other) const
{
    return d_ptr->pCalendar->before( *other.d_ptr->pCalendar );
}

bool MCalendar::equals(const MCalendar &other) const
{
    return d_ptr->pCalendar->equals( *other.d_ptr->pCalendar );
}

bool MCalendar::operator<(const MCalendar &other) const
{
    return d_ptr->pCalendar->operator<( *other.d_ptr->pCalendar );
}

bool MCalendar::operator<=(const MCalendar &other) const
{
    return d_ptr->pCalendar->operator<=( *other.d_ptr->pCalendar );
}

bool MCalendar::operator==(const MCalendar &other) const
{
    return d_ptr->pCalendar->operator==( *other.d_ptr->pCalendar );
}

bool MCalendar::operator!=(const MCalendar &other) const
{
    return d_ptr->pCalendar->operator!=( *other.d_ptr->pCalendar );
}

bool MCalendar::operator>(const MCalendar &other) const
{
    return d_ptr->pCalendar->operator>( *other.d_ptr->pCalendar );
}

bool MCalendar::operator>=(const MCalendar &other) const
{
    return d_ptr->pCalendar->operator>=( *other.d_ptr->pCalendar );
}

//static
QString MCalendar::systemTimeZone()
{
    return ML10N::MCalendar::systemTimeZone();
}

//static
void MCalendar::setSystemTimeZone(const QString &timezone)
{
    ML10N::MCalendar::setSystemTimeZone( timezone );
}

//static
QStringList MCalendar::supportedTimeZones()
{
    return ML10N::MCalendar::supportedTimeZones();
}

//static
QStringList MCalendar::supportedTimeZones(const QString &country)
{
    return ML10N::MCalendar::supportedTimeZones( country );
}
