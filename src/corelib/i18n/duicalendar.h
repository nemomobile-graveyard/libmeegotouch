/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUICALENDAR_H
#define DUICALENDAR_H

#include <QDateTime>

#include "duilocale.h"
#include "duiexport.h"

class DuiCalendarPrivate;
class QDateTime;
class QDate;

class DUI_EXPORT DuiCalendar
{
public:
    explicit DuiCalendar(DuiLocale::CalendarType calendarType = DuiLocale::DefaultCalendar,
                         const QString &timezone = QString());
    explicit DuiCalendar(const DuiLocale &duiLocale, const QString &timezone = QString());

    DuiCalendar(const DuiCalendar &other);

    virtual ~DuiCalendar();

    DuiCalendar &operator=(const DuiCalendar &other);


    bool isValid() const;

    DuiLocale::CalendarType type() const;

    // TODO: do we need to set era, week of year, week of month, day of year etc?

    void setDate(int year, int month, int day);
    void setDate(const QDate &date);

    void setDateTime(QDateTime datetime);

    QDateTime qDateTime(Qt::TimeSpec spec = Qt::LocalTime) const;

    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);


    int dayOfYear() const;
    int weekOfYear() const;
    int month() const;
    int year() const;
    int yearOfWeek() const;
    int dayOfMonth() const;
    int dayOfWeek() const;

    void setTime(int hours, int minutes, int seconds);
    void setHours(int hours);
    void setMinutes(int minutes);
    void setSeconds(int seconds);

    int hour() const;
    int minute() const;
    int second() const;

    void addYears(int years);
    void addMonths(int months);
    void addDays(int days);
    void addHours(int hours);
    void addMinutes(int minutes);
    void addSeconds(int seconds);

    int firstDayOfMonth() const;
    int lastDayOfMonth() const;

    void setFirstDayOfWeek(int weekday);
    int firstDayOfWeek() const;

    void setMinimalDaysInFirstWeek(int days);
    int minimalDaysInFirstWeek() const;

    int weekNumber() const;
    int maximumWeeksInMonth() const;
    int daysInWeek() const;

    bool after(const DuiCalendar &other) const;
    bool before(const DuiCalendar &other) const;
    bool equals(const DuiCalendar &other) const;

    bool operator<(const DuiCalendar &other) const;
    bool operator<=(const DuiCalendar &other) const;
    bool operator==(const DuiCalendar &other) const;
    bool operator!=(const DuiCalendar &other) const;
    bool operator>(const DuiCalendar &other) const;
    bool operator>=(const DuiCalendar &other) const;

    static void setSystemTimeZone(const QString &timezone);
    static QString systemTimeZone();

    static QStringList supportedTimeZones();
    static QStringList supportedTimeZones(const QString &country);


private:
    DuiCalendarPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(DuiCalendar)

    friend class DuiLocale;
};

#endif
