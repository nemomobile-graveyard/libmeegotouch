/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MCALENDAR_H
#define MCALENDAR_H

#include <QDateTime>

#include "mlocale.h"
#include "mexport.h"

class MCalendarPrivate;
class QDateTime;
class QDate;

class M_CORE_EXPORT MCalendar
{
public:
    explicit MCalendar(MLocale::CalendarType calendarType = MLocale::DefaultCalendar,
                         const QString &timezone = QString());
    explicit MCalendar(const MLocale &mLocale, const QString &timezone = QString());

    MCalendar(const MCalendar &other);

    virtual ~MCalendar();

    MCalendar &operator=(const MCalendar &other);


    bool isValid() const;

    MLocale::CalendarType type() const;

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

    bool after(const MCalendar &other) const;
    bool before(const MCalendar &other) const;
    bool equals(const MCalendar &other) const;

    bool operator<(const MCalendar &other) const;
    bool operator<=(const MCalendar &other) const;
    bool operator==(const MCalendar &other) const;
    bool operator!=(const MCalendar &other) const;
    bool operator>(const MCalendar &other) const;
    bool operator>=(const MCalendar &other) const;

    /*
     * \brief sets the system time zone
     *
     * \param timezone a time zone id, for example “Europe/Helsinki”.
     *
     * \sa systemTimeZone()
     * \sa supportedTimeZones()
     * \sa supportedTimeZones(const QString &country)
     */
    static void setSystemTimeZone(const QString &timezone);

    /*
     * \brief returns the id of the current system time zone
     *
     * Time zone ids are strings which look like “Europe/Helsinki”, for example.
     */
    static QString systemTimeZone();

    /*
     * \brief returns a list of all time zone ids supported on the system
     *
     * Time zone ids are strings which look like “Europe/Helsinki”, for example.
     *
     * \sa supportedTimeZones(const QString &country)
     * \sa systemTimeZone()
     * \sa setSystemTimeZone(const QString &timezone)
     */
    static QStringList supportedTimeZones();

    /*
     * \brief returns a list of all time zone ids supported in a specific country
     * \param country country code in ISO-3166 format
     *
     * Examples:
     *
     * \code
     * MCalendar::supportedTimeZones("JP");
     * \endcode
     *
     * might return the list “Asia/Tokyo”, “JST”, “Japan”, which
     * are just different names for the identical time zone.
     *
     * \code
     * MCalendar::supportedTimeZones("JP");
     * \endcode
     *
     * might return a list like “America/Cancun”,
     * “America/Chihuahua”, “America/Ensenada”,
     * “America/Hermosillo”, “America/Matamoros”,
     * “America/Mazatlan”, “America/Merida”,
     * “America/Mexico_City”, “America/Monterrey”,
     * “America/Ojinaga”, “America/Santa_Isabel”,
     * “America/Tijuana”, “Mexico/BajaNorte”, “Mexico/BajaSur”,
     * “Mexico/General”. Some of these are just different names
     * for the same thing, some are really different.
     *
     * \sa supportedTimeZones()
     * \sa systemTimeZone()
     * \sa setSystemTimeZone(const QString &timezone)
     */
    static QStringList supportedTimeZones(const QString &country);

private:
    MCalendarPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MCalendar)

    friend class MLocale;
};

#endif
