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

#include "duicalendar.h"
#include "duicalendar_p.h"

#include <QString>
#include <QDateTime>

#include "duilocale_p.h"
#include "duiicuconversions.h"


DuiCalendarPrivate::DuiCalendarPrivate(DuiLocale::Calendar calendar)
    : _calendar(0), _calendarType(calendar), _valid(true)
{
    if (_calendarType == DuiLocale::DefaultCalendar) {
        DuiLocale defaultLocale;
        _calendarType = defaultLocale.calendar();
    }
}


// copy constructor
DuiCalendarPrivate::DuiCalendarPrivate(const DuiCalendarPrivate &other)
    : _calendar(other._calendar->clone()),
      _calendarType(other._calendarType),
      _valid(other._valid)
{
    // nothing
}


DuiCalendarPrivate::~DuiCalendarPrivate()
{
    delete _calendar;
}


DuiCalendarPrivate &DuiCalendarPrivate::operator=(const DuiCalendarPrivate &other)
{
    delete _calendar;
    _calendar = other._calendar->clone();
    _calendarType = other._calendarType;
    _valid = other._valid;
    return *this;
}


DuiLocale::Weekday DuiCalendarPrivate::icuWeekdayToDuiWeekday(int uweekday)
{
    switch (uweekday) {
    case(UCAL_MONDAY):
        return DuiLocale::Monday;

    case(UCAL_TUESDAY):
        return DuiLocale::Tuesday;

    case(UCAL_WEDNESDAY):
        return DuiLocale::Wednesday;

    case(UCAL_THURSDAY):
        return DuiLocale::Thursday;

    case(UCAL_FRIDAY):
        return DuiLocale::Friday;

    case(UCAL_SATURDAY):
        return DuiLocale::Saturday;

    case(UCAL_SUNDAY):
        return DuiLocale::Sunday;

    default:
        return static_cast<DuiLocale::Weekday>(0);
    }
}


///////////////////////
// DuiCalendar class

/*!
  \class DuiCalendar

  \brief DuiCalendar can hold date and time value for with different calendar systems

  \note insufficient information: default values are used. e.g. with gregorian calendar
  the epoch value (1970-1-1)

  \note inconsistent information: preference is on fields set more recently
*/


//! Constructs a DuiCalendar with explicit calendar type
//! \param calendar Calendar type. Default is the type used for default locale.
//! \param timezone Timezone to be used. Default is timezone of the default locale.
//! Possible values: id, e.g. PST, country/city, e.g. Europe/Helsinki or GMT offset, e.g. GTM+2:00.
//! supportedTimezones() can be used to enumerate choices.
DuiCalendar::DuiCalendar(DuiLocale::Calendar calendar,
                         const QString &timezone) : d_ptr(new DuiCalendarPrivate(calendar))
{
    Q_D(DuiCalendar);

    // fetch default calendar system
    DuiLocale defaultLocale;

    QString timeCategory = defaultLocale.d_ptr->categoryName(DuiLocale::DuiLcTime);

    icu::Locale calLocale = DuiIcuConversions::createLocale(timeCategory, calendar);

    UErrorCode status = U_ZERO_ERROR;

    if (timezone.isEmpty() == false) {
        // with explicit time zone
        UnicodeString tzString;
        tzString = DuiIcuConversions::qStringToUnicodeString(timezone);
        icu::TimeZone *tz = icu::TimeZone::createTimeZone(tzString);
        d->_calendar = icu::Calendar::createInstance(tz, calLocale, status);

    } else {
        // gets the timezone from system default automatically
        d->_calendar = icu::Calendar::createInstance(calLocale, status);
    }

    if (!U_SUCCESS(status)) {
        d->_valid = false;
    }
}


//! Constructs a DuiCalendar based on calendar system used by given DuiLocale
DuiCalendar::DuiCalendar(const DuiLocale &duiLocale, const QString &timezone)
    : d_ptr(new DuiCalendarPrivate(duiLocale.calendar()))
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    icu::Locale calLocale
    = duiLocale.d_ptr->getCategoryLocale(DuiLocale::DuiLcTime);

    if (timezone.isEmpty() == false) {
        UnicodeString tzString;
        tzString = DuiIcuConversions::qStringToUnicodeString(timezone);
        icu::TimeZone *tz = icu::TimeZone::createTimeZone(tzString);
        d->_calendar = icu::Calendar::createInstance(tz, calLocale, status);

    } else {
        d->_calendar = icu::Calendar::createInstance(calLocale, status);
    }

    if (!U_SUCCESS(status)) {
        d->_valid = false;
    }
}


//! Copy constructor
DuiCalendar::DuiCalendar(const DuiCalendar &other)
    : d_ptr(new DuiCalendarPrivate(*other.d_ptr))
{
    // nothing
}

/*!
  \brief Destroys the calendar.
 */
DuiCalendar::~DuiCalendar()
{
    delete d_ptr;
}


//! Assignment operator
DuiCalendar &DuiCalendar::operator=(const DuiCalendar &other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}



//! checks the validity of the information of the calendar. Returns true if calendar is valid.
bool DuiCalendar::isValid() const
{
    Q_D(const DuiCalendar);

    // TODO: we might also check that there's enough information set for
    // unambiguous moment of time.
    return d->_valid;
}


//! returns the used calendar system
DuiLocale::Calendar DuiCalendar::type() const
{
    Q_D(const DuiCalendar);

    return d->_calendarType;
}


//! Sets the calendar date. Parameters self explanatory
void DuiCalendar::setDate(int year, int month, int day)
{
    Q_D(DuiCalendar);

    // icu calendar uses 0 based numbering for months
    d->_calendar->set(year, month - 1, day);
}


//! Sets the calendar date from QDate
void DuiCalendar::setDate(const QDate &date)
{
    QDateTime datetime(date);
    setDateTime(datetime);
}


//! Sets the calendar according to given QDate
void DuiCalendar::setDateTime(QDateTime date)
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;

    // we avoid time conversions made by qt
    Qt::TimeSpec originalTimeSpec = date.timeSpec();
    date.setTimeSpec(Qt::UTC);
    UDate icuDate = date.toTime_t() * 1000.0;

    if (originalTimeSpec == Qt::LocalTime) {
        // convert from local time to UTC
        const icu::TimeZone &tz = d->_calendar->getTimeZone();
        qint32 rawOffset;
        qint32 dstOffset;
        tz.getOffset(icuDate, true /*local */, rawOffset, dstOffset, status);
        icuDate = icuDate - rawOffset - dstOffset;
    }

    d->_calendar->setTime(icuDate, status);
}


//! Converts calendar into QDate
//! \param spec (optional) specification for resulting QDateTime, Qt::LocalTime (default) or Qt::UTC
QDateTime DuiCalendar::qDateTime(Qt::TimeSpec spec) const
{
    Q_D(const DuiCalendar);

    QDateTime time;

    UErrorCode status = U_ZERO_ERROR;
    UDate icuDate = d->_calendar->getTime(status);

    if (spec == Qt::LocalTime) {
        // convert from UTC to local time
        const icu::TimeZone &tz = d->_calendar->getTimeZone();
        qint32 rawOffset;
        qint32 dstOffset;
        tz.getOffset(icuDate, true /*local */, rawOffset, dstOffset, status);
        icuDate = icuDate + rawOffset + dstOffset;
    }

    time.setTime_t(icuDate / 1000.0); // takes time in seconds since epoch
    // note: we set time spec after time value so Qt will not any conversions
    // of its own to UTC. We might let Qt handle it but this might be more robust
    time.setTimeSpec(spec);
    return time;
}


/*!
  \brief Set the year of the date to \a year.
 */
void DuiCalendar::setYear(int year)
{
    Q_D(DuiCalendar);

    d->_calendar->set(UCAL_YEAR, year);
}


/*!
  \brief Set the month of the date to \a month.
 */
void DuiCalendar::setMonth(int month)
{
    Q_D(DuiCalendar);

    d->_calendar->set(UCAL_MONTH, month - 1);
}

/*!
  \brief Set the day of the date to \a day.
 */
void DuiCalendar::setDay(int day)
{
    Q_D(DuiCalendar);

    d->_calendar->set(UCAL_DAY_OF_MONTH, day);
}


//! returns day number of the year. returns zero on error.
int DuiCalendar::dayOfYear() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    // status value is ignored because get returns zero on error
    return d->_calendar->get(UCAL_DAY_OF_YEAR, status);
}


//! Returns week number in the year
int DuiCalendar::weekOfYear() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_WEEK_OF_YEAR, status);
}


//! Returns month number
int DuiCalendar::month() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_MONTH, status) + 1; // icu month is zero based
}


//! Returns year number
int DuiCalendar::year() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_YEAR, status);
}


//! returns the year the current week "belongs to".
//
// e.g. 2008-12-31 belongs to week 1 of year 2009
int DuiCalendar::yearOfWeek() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_YEAR_WOY, status);
}


//! Returns day of month
int DuiCalendar::dayOfMonth() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_DAY_OF_MONTH, status);
}


//! Returns day of week. Monday = 1
int DuiCalendar::dayOfWeek() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    int uweekday = d->_calendar->get(UCAL_DAY_OF_WEEK, status);
    return DuiCalendarPrivate::icuWeekdayToDuiWeekday(uweekday);
}

/*!
  \brief Set the hours of the date/time to \a hours.
 */
void DuiCalendar::setHours(int hours)
{
    Q_D(DuiCalendar);

    d->_calendar->set(UCAL_HOUR_OF_DAY, hours);
}

/*!
  \brief Set the minutes of the time to \a minutes.
 */
void DuiCalendar::setMinutes(int minutes)
{
    Q_D(DuiCalendar);

    d->_calendar->set(UCAL_MINUTE, minutes);
}

/*!
  \brief Set the seconds of the time to \a seconds.
 */
void DuiCalendar::setSeconds(int seconds)
{
    Q_D(DuiCalendar);

    d->_calendar->set(UCAL_SECOND, seconds);
}


//! Set calendar time of the day
void DuiCalendar::setTime(int hours, int minutes, int seconds)
{
    setHours(hours);
    setMinutes(minutes);
    setSeconds(seconds);
}


/*!
  \brief Returns the current hours.
 */
int DuiCalendar::hour() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    // hour of day follows 24h clock
    return d->_calendar->get(UCAL_HOUR_OF_DAY, status);
}

/*!
  \brief Returns the current minutes.
 */
int DuiCalendar::minute() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_MINUTE, status);
}

/*!
  \brief Returns the current seconds.
 */
int DuiCalendar::second() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_SECOND, status);
}

/*!
  \brief Add number of \a years years to the current date.
 */
void DuiCalendar::addYears(int years)
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_YEAR, years, status);
}

/*!
  \brief Add number of \a months months to the current date.
 */
void DuiCalendar::addMonths(int months)
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_MONTH, months, status);
}

/*!
  \brief Add number of \a days days to the current date.
 */
void DuiCalendar::addDays(int days)
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_DATE, days, status);
}

/*!
  \brief Add number of \a hours hours to the current time.
 */
void DuiCalendar::addHours(int hours)
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_HOUR, hours, status);
}

/*!
  \brief Add number of \a minutes minutes to the current time.
 */
void DuiCalendar::addMinutes(int minutes)
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_MINUTE, minutes, status);
}

/*!
  \brief Add number of \a seconds seconds to the current time.
 */
void DuiCalendar::addSeconds(int seconds)
{
    Q_D(DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_SECOND, seconds, status);
}


/*!
  \brief Returns first day of a month.
 */
int DuiCalendar::firstDayOfMonth() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->getActualMinimum(UCAL_DATE, status);
}

/*!
  \brief Returns last day of a month.
 */
int DuiCalendar::lastDayOfMonth() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->getActualMaximum(UCAL_DATE, status);
}

/*!
  \brief Sets the first day of the week to \a weekday.
 */
void DuiCalendar::setFirstDayOfWeek(int weekday)
{
    Q_D(DuiCalendar);

    d->_calendar->setFirstDayOfWeek(DuiIcuConversions::icuWeekday(weekday));
}

/*!
  \brief Returns the first day of a week.
 */
int DuiCalendar::firstDayOfWeek() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    int weekday = d->_calendar->getFirstDayOfWeek(status);
    return DuiCalendarPrivate::icuWeekdayToDuiWeekday(weekday);
}


//! sets what is the required amount of days for the first week of the year.
void DuiCalendar::setMinimalDaysInFirstWeek(int days)
{
    Q_D(DuiCalendar);

    d->_calendar->setMinimalDaysInFirstWeek(days);
}


//! returns the number of days required for the first week in the year
int DuiCalendar::minimalDaysInFirstWeek() const
{
    Q_D(const DuiCalendar);

    return d->_calendar->getMinimalDaysInFirstWeek();
}

/*!
  \brief Returns the current week number.
 */
int DuiCalendar::weekNumber() const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_WEEK_OF_YEAR, status);
}

/*!
  \brief Returns the maximum number of weeks in a month.
 */
int DuiCalendar::maximumWeeksInMonth() const
{
    Q_D(const DuiCalendar);

    // unfortunately week numbering in the month doesn't necessarily
    // start from the first day of month.
    icu::Calendar *tmpCal = d->_calendar->clone();
    tmpCal->setMinimalDaysInFirstWeek(1);
    int val = tmpCal->getMaximum(UCAL_WEEK_OF_MONTH);
    delete tmpCal;
    return val;
}

/*!
  \brief Returns the maximum number of days in a week.
 */
int DuiCalendar::daysInWeek() const
{
    Q_D(const DuiCalendar);

    return d->_calendar->getMaximum(UCAL_DAY_OF_WEEK);
}

/*!
  \brief Returns true if the current date/time of this calendar is after \a other.
 */
bool DuiCalendar::after(const DuiCalendar &other) const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->after(*other.d_ptr->_calendar, status);
}

/*!
  \brief Returns true if the current date/time of this calendar is before \a other.
 */
bool DuiCalendar::before(const DuiCalendar &other) const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->before(*other.d_ptr->_calendar, status);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal to \a other.
 */
bool DuiCalendar::equals(const DuiCalendar &other) const
{
    Q_D(const DuiCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->equals(*other.d_ptr->_calendar, status);
}

/*!
  \brief Returns true if the current date/time of this calendar is before \a other.
 */
bool DuiCalendar::operator<(const DuiCalendar &other) const
{
    return before(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal or before \a other.
 */
bool DuiCalendar::operator<=(const DuiCalendar &other) const
{
    return before(other) || equals(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal to \a other.
 */
bool DuiCalendar::operator==(const DuiCalendar &other) const
{
    return equals(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is different than \a other.
 */
bool DuiCalendar::operator!=(const DuiCalendar &other) const
{
    return !equals(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is after \a other.
 */
bool DuiCalendar::operator>(const DuiCalendar &other) const
{
    return after(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal or after \a other.
 */
bool DuiCalendar::operator>=(const DuiCalendar &other) const
{
    return after(other) || equals(other);
}


//! Returns the system time zone string
QString DuiCalendar::systemTimeZone()
{
    icu::TimeZone *defaultTz = icu::TimeZone::createDefault();
    icu::UnicodeString id;
    defaultTz->getID(id);
    delete defaultTz;
    return DuiIcuConversions::unicodeStringToQString(id);
}


//static
//! Sets system time zone
void DuiCalendar::setSystemTimeZone(const QString &timezone)
{
    icu::UnicodeString tzString = DuiIcuConversions::qStringToUnicodeString(timezone);
    icu::TimeZone *tz = icu::TimeZone::createTimeZone(tzString);
    // FIXME: should we check the creation succeeds with the string?
    icu::TimeZone::adoptDefault(tz);
}


//static
//! Enumerates possible timezone IDs
QStringList DuiCalendar::supportedTimeZones()
{
    icu::StringEnumeration *strEnum = icu::TimeZone::createEnumeration();

    QStringList result;
    UErrorCode status = U_ZERO_ERROR;
    const UnicodeString *next = strEnum->snext(status);

    while (next != 0) {
        result << DuiIcuConversions::unicodeStringToQString(*next);
        next = strEnum->snext(status);
    }

    delete strEnum;

    return result;
}


//static
//! Enumerates possible timezone IDS for a specific country
QStringList DuiCalendar::supportedTimeZones(const QString &country)
{
    icu::StringEnumeration *strEnum = icu::TimeZone::createEnumeration(qPrintable(country));

    QStringList result;
    UErrorCode status = U_ZERO_ERROR;
    const UnicodeString *next = strEnum->snext(status);

    while (next != 0) {
        result << DuiIcuConversions::unicodeStringToQString(*next);
        next = strEnum->snext(status);
    }

    delete strEnum;

    return result;
}
