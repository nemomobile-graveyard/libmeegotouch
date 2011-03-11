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

#include "mcalendar.h"
#include "mcalendar_p.h"

#include <QString>
#include <QDateTime>
#include <QDebug>

#include "mlocale_p.h"
#include "micuconversions.h"


MCalendarPrivate::MCalendarPrivate(MLocale::CalendarType calendarType)
    : _calendar(0), _calendarType(calendarType), _valid(true)
{
    if (_calendarType == MLocale::DefaultCalendar) {
        MLocale defaultLocale;
        _calendarType = defaultLocale.calendarType();
    }
}


// copy constructor
MCalendarPrivate::MCalendarPrivate(const MCalendarPrivate &other)
    : _calendar(other._calendar->clone()),
      _calendarType(other._calendarType),
      _valid(other._valid)
{
    // nothing
}


MCalendarPrivate::~MCalendarPrivate()
{
    delete _calendar;
}


MCalendarPrivate &MCalendarPrivate::operator=(const MCalendarPrivate &other)
{
    delete _calendar;
    _calendar = other._calendar->clone();
    _calendarType = other._calendarType;
    _valid = other._valid;
    return *this;
}


MLocale::Weekday MCalendarPrivate::icuWeekdayToMWeekday(int uweekday)
{
    switch (uweekday) {
    case(UCAL_MONDAY):
        return MLocale::Monday;

    case(UCAL_TUESDAY):
        return MLocale::Tuesday;

    case(UCAL_WEDNESDAY):
        return MLocale::Wednesday;

    case(UCAL_THURSDAY):
        return MLocale::Thursday;

    case(UCAL_FRIDAY):
        return MLocale::Friday;

    case(UCAL_SATURDAY):
        return MLocale::Saturday;

    case(UCAL_SUNDAY):
        return MLocale::Sunday;

    default:
        return static_cast<MLocale::Weekday>(0);
    }
}


///////////////////////
// MCalendar class

/*!
  \class MCalendar

  \brief MCalendar can hold date and time value for with different calendar systems

  \note insufficient information: default values are used. e.g. with gregorian calendar
  the epoch value (1970-1-1)

  \note inconsistent information: preference is on fields set more recently
*/


//! Constructs a MCalendar with explicit calendar type
//! \param calendar Calendar type. Default is the type used for default locale.
//! \param timezone Timezone to be used. Default is timezone of the default locale.
//! Possible values: id, e.g. PST, country/city, e.g. Europe/Helsinki or GMT offset, e.g. GTM+2:00.
//! supportedTimezones() can be used to enumerate choices.
MCalendar::MCalendar(MLocale::CalendarType calendarType,
                         const QString &timezone) : d_ptr(new MCalendarPrivate(calendarType))
{
    Q_D(MCalendar);

    // fetch default calendar system
    MLocale defaultLocale;

    QString timeCategory = defaultLocale.d_ptr->categoryName(MLocale::MLcTime);
    timeCategory = MIcuConversions::setCalendarOption(timeCategory, calendarType);
    icu::Locale calLocale = icu::Locale(qPrintable(timeCategory));

    UErrorCode status = U_ZERO_ERROR;

    if (timezone.isEmpty() == false) {
        // with explicit time zone
        UnicodeString tzString;
        tzString = MIcuConversions::qStringToUnicodeString(timezone);
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


//! Constructs a MCalendar based on calendar system used by given MLocale
MCalendar::MCalendar(const MLocale &mLocale, const QString &timezone)
    : d_ptr(new MCalendarPrivate(mLocale.calendarType()))
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    icu::Locale calLocale
    = mLocale.d_ptr->getCategoryLocale(MLocale::MLcTime);

    if (timezone.isEmpty() == false) {
        UnicodeString tzString;
        tzString = MIcuConversions::qStringToUnicodeString(timezone);
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
MCalendar::MCalendar(const MCalendar &other)
    : d_ptr(new MCalendarPrivate(*other.d_ptr))
{
    // nothing
}

/*!
  \brief Destroys the calendar.
 */
MCalendar::~MCalendar()
{
    delete d_ptr;
}


//! Assignment operator
MCalendar &MCalendar::operator=(const MCalendar &other)
{
    *d_ptr = *other.d_ptr;
    return *this;
}



//! checks the validity of the information of the calendar. Returns true if calendar is valid.
bool MCalendar::isValid() const
{
    Q_D(const MCalendar);

    // TODO: we might also check that there's enough information set for
    // unambiguous moment of time.
    return d->_valid;
}


//! returns the used calendar system
MLocale::CalendarType MCalendar::type() const
{
    Q_D(const MCalendar);

    return d->_calendarType;
}


//! Sets the calendar date. Parameters self explanatory
void MCalendar::setDate(int year, int month, int day)
{
    Q_D(MCalendar);

    // icu calendar uses 0 based numbering for months
    d->_calendar->set(year, month - 1, day);
}


//! Sets the calendar date from QDate
void MCalendar::setDate(const QDate &date)
{
    QDateTime datetime(date);
    setDateTime(datetime);
}

#define MSECS_PER_DAY 86400000

//! Sets the calendar according to given QDate
void MCalendar::setDateTime(QDateTime dateTime)
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;

    // we avoid time conversions made by qt
    Qt::TimeSpec originalTimeSpec = dateTime.timeSpec();
    dateTime.setTimeSpec(Qt::UTC);

    // We cannot use QDateTime::toTime_t because this
    // works only for dates after 1970-01-01T00:00:00.000.
#if QT_VERSION >= 0x040700
    UDate icuDate = dateTime.toMSecsSinceEpoch();
#else
    // Qt < 4.7 lacks QDateTime::toMSecsSinceEpoch(), we need to emulate it:
    int days = QDate(1970, 1, 1).daysTo(dateTime.date());
    qint64 msecs = qint64(QTime().secsTo(dateTime.time())) * 1000;
    UDate icuDate = (qint64(days) * MSECS_PER_DAY) + msecs;
#endif

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
QDateTime MCalendar::qDateTime(Qt::TimeSpec spec) const
{
    Q_D(const MCalendar);

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
    // We cannot use QDateTime::setTime_t because this
    // works only for dates after 1970-01-01T00:00:00.000.
    QDateTime dateTime;
    // avoid conversions by Qt
    dateTime.setTimeSpec(Qt::UTC);
#if QT_VERSION >= 0x040700
    dateTime.setMSecsSinceEpoch(qint64(icuDate));
#else
    // Qt < 4.7 lacks QDateTime::setMSecsSinceEpoch(), we need to emulate it.
    qint64 msecs = qint64(icuDate);
    int ddays = msecs / MSECS_PER_DAY;
    msecs %= MSECS_PER_DAY;
    if (msecs < 0) {
        // negative
        --ddays;
        msecs += MSECS_PER_DAY;
    }
    dateTime.setDate(QDate(1970, 1, 1).addDays(ddays));
    dateTime.setTime(QTime().addMSecs(msecs));
#endif
    // note: we set time spec after time value so Qt will not any
    // conversions of its own to UTC. We might let Qt handle it but
    // this might be more robust
    dateTime.setTimeSpec(spec);
    return dateTime;
}


/*!
  \brief Set the year of the date to \a year.
 */
void MCalendar::setYear(int year)
{
    Q_D(MCalendar);

    d->_calendar->set(UCAL_YEAR, year);
}


/*!
  \brief Set the month of the date to \a month.
 */
void MCalendar::setMonth(int month)
{
    Q_D(MCalendar);

    d->_calendar->set(UCAL_MONTH, month - 1);
}

/*!
  \brief Set the day of the date to \a day.
 */
void MCalendar::setDay(int day)
{
    Q_D(MCalendar);

    d->_calendar->set(UCAL_DAY_OF_MONTH, day);
}


//! returns day number of the year. returns zero on error.
int MCalendar::dayOfYear() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    // status value is ignored because get returns zero on error
    return d->_calendar->get(UCAL_DAY_OF_YEAR, status);
}


//! Returns week number in the year
int MCalendar::weekOfYear() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_WEEK_OF_YEAR, status);
}


//! Returns month number
int MCalendar::month() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_MONTH, status) + 1; // icu month is zero based
}


//! Returns year number
int MCalendar::year() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_YEAR, status);
}


//! returns the year the current week "belongs to".
//
// e.g. 2008-12-31 belongs to week 1 of year 2009
int MCalendar::yearOfWeek() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_YEAR_WOY, status);
}


//! Returns day of month
int MCalendar::dayOfMonth() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_DAY_OF_MONTH, status);
}


//! Returns day of week. Monday = 1
int MCalendar::dayOfWeek() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    int uweekday = d->_calendar->get(UCAL_DAY_OF_WEEK, status);
    return MCalendarPrivate::icuWeekdayToMWeekday(uweekday);
}

/*!
  \brief Set the hours of the date/time to \a hours.
 */
void MCalendar::setHours(int hours)
{
    Q_D(MCalendar);

    d->_calendar->set(UCAL_HOUR_OF_DAY, hours);
}

/*!
  \brief Set the minutes of the time to \a minutes.
 */
void MCalendar::setMinutes(int minutes)
{
    Q_D(MCalendar);

    d->_calendar->set(UCAL_MINUTE, minutes);
}

/*!
  \brief Set the seconds of the time to \a seconds.
 */
void MCalendar::setSeconds(int seconds)
{
    Q_D(MCalendar);

    d->_calendar->set(UCAL_SECOND, seconds);
}


//! Set calendar time of the day
void MCalendar::setTime(int hours, int minutes, int seconds)
{
    setHours(hours);
    setMinutes(minutes);
    setSeconds(seconds);
}


/*!
  \brief Returns the current hours.
 */
int MCalendar::hour() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    // hour of day follows 24h clock
    return d->_calendar->get(UCAL_HOUR_OF_DAY, status);
}

/*!
  \brief Returns the current minutes.
 */
int MCalendar::minute() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_MINUTE, status);
}

/*!
  \brief Returns the current seconds.
 */
int MCalendar::second() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_SECOND, status);
}

/*!
  \brief Add number of \a years years to the current date.
 */
void MCalendar::addYears(int years)
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_YEAR, years, status);
}

/*!
  \brief Add number of \a months months to the current date.
 */
void MCalendar::addMonths(int months)
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_MONTH, months, status);
}

/*!
  \brief Add number of \a days days to the current date.
 */
void MCalendar::addDays(int days)
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_DATE, days, status);
}

/*!
  \brief Add number of \a hours hours to the current time.
 */
void MCalendar::addHours(int hours)
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_HOUR, hours, status);
}

/*!
  \brief Add number of \a minutes minutes to the current time.
 */
void MCalendar::addMinutes(int minutes)
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_MINUTE, minutes, status);
}

/*!
  \brief Add number of \a seconds seconds to the current time.
 */
void MCalendar::addSeconds(int seconds)
{
    Q_D(MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    d->_calendar->add(UCAL_SECOND, seconds, status);
}


/*!
  \brief Returns first day of a month.
 */
int MCalendar::firstDayOfMonth() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->getActualMinimum(UCAL_DATE, status);
}

/*!
  \brief Returns last day of a month.
 */
int MCalendar::lastDayOfMonth() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->getActualMaximum(UCAL_DATE, status);
}

/*!
  \brief Sets the first day of the week to \a weekday.
 */
void MCalendar::setFirstDayOfWeek(int weekday)
{
    Q_D(MCalendar);

    d->_calendar->setFirstDayOfWeek(MIcuConversions::icuWeekday(weekday));
}

/*!
  \brief Returns the first day of a week.
 */
int MCalendar::firstDayOfWeek() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    int weekday = d->_calendar->getFirstDayOfWeek(status);
    return MCalendarPrivate::icuWeekdayToMWeekday(weekday);
}


//! sets what is the required amount of days for the first week of the year.
void MCalendar::setMinimalDaysInFirstWeek(int days)
{
    Q_D(MCalendar);

    d->_calendar->setMinimalDaysInFirstWeek(days);
}


//! returns the number of days required for the first week in the year
int MCalendar::minimalDaysInFirstWeek() const
{
    Q_D(const MCalendar);

    return d->_calendar->getMinimalDaysInFirstWeek();
}

/*!
  \brief Returns the current week number.
 */
int MCalendar::weekNumber() const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->get(UCAL_WEEK_OF_YEAR, status);
}

/*!
  \brief Returns the maximum number of weeks in a month.
 */
int MCalendar::maximumWeeksInMonth() const
{
    Q_D(const MCalendar);

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
int MCalendar::daysInWeek() const
{
    Q_D(const MCalendar);

    return d->_calendar->getMaximum(UCAL_DAY_OF_WEEK);
}

/*!
  \brief Returns true if the current date/time of this calendar is after \a other.
 */
bool MCalendar::after(const MCalendar &other) const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->after(*other.d_ptr->_calendar, status);
}

/*!
  \brief Returns true if the current date/time of this calendar is before \a other.
 */
bool MCalendar::before(const MCalendar &other) const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->before(*other.d_ptr->_calendar, status);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal to \a other.
 */
bool MCalendar::equals(const MCalendar &other) const
{
    Q_D(const MCalendar);

    UErrorCode status = U_ZERO_ERROR;
    return d->_calendar->equals(*other.d_ptr->_calendar, status);
}

/*!
  \brief Returns true if the current date/time of this calendar is before \a other.
 */
bool MCalendar::operator<(const MCalendar &other) const
{
    return before(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal or before \a other.
 */
bool MCalendar::operator<=(const MCalendar &other) const
{
    return before(other) || equals(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal to \a other.
 */
bool MCalendar::operator==(const MCalendar &other) const
{
    return equals(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is different than \a other.
 */
bool MCalendar::operator!=(const MCalendar &other) const
{
    return !equals(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is after \a other.
 */
bool MCalendar::operator>(const MCalendar &other) const
{
    return after(other);
}

/*!
  \brief Returns true if the current date/time of this calendar is equal or after \a other.
 */
bool MCalendar::operator>=(const MCalendar &other) const
{
    return after(other) || equals(other);
}

//static
QString MCalendar::systemTimeZone()
{
    icu::TimeZone *defaultTz = icu::TimeZone::createDefault();
    icu::UnicodeString id;
    defaultTz->getID(id);
    delete defaultTz;
    return MIcuConversions::unicodeStringToQString(id);
}

//static
void MCalendar::setSystemTimeZone(const QString &timezone)
{
    icu::UnicodeString tzString = MIcuConversions::qStringToUnicodeString(timezone);
    icu::TimeZone *tz = icu::TimeZone::createTimeZone(tzString);
    // The documentation of says “Return result guaranteed to be non-null.”,
    // see http://icu-project.org/apiref/icu4c/classTimeZone.html
    // but I put in an assert anyway:
    Q_ASSERT_X(tz != NULL,
               "MCalendar::setSystemTimeZone",
               "icu::TimeZone::createTimeZone() returned NULL.");
    if (tz) {
        icu::UnicodeString tzStringReal;
        QString timezoneReal
            = MIcuConversions::unicodeStringToQString(tz->getID(tzStringReal));
        if (timezone != timezoneReal)
            qCritical()
                << __PRETTY_FUNCTION__
                << "icu::TimeZone::createTimeZone() created a different timezone.";
        icu::TimeZone::adoptDefault(tz);
    }
}

//static
QStringList MCalendar::supportedTimeZones()
{
    icu::StringEnumeration *strEnum = icu::TimeZone::createEnumeration();

    QStringList result;
    UErrorCode status = U_ZERO_ERROR;
    const UnicodeString *next = strEnum->snext(status);

    while (next != 0) {
        result << MIcuConversions::unicodeStringToQString(*next);
        next = strEnum->snext(status);
    }

    delete strEnum;

    return result;
}

//static
QStringList MCalendar::supportedTimeZones(const QString &country)
{
    icu::StringEnumeration *strEnum = icu::TimeZone::createEnumeration(qPrintable(country));

    QStringList result;
    UErrorCode status = U_ZERO_ERROR;
    const UnicodeString *next = strEnum->snext(status);

    while (next != 0) {
        result << MIcuConversions::unicodeStringToQString(*next);
        next = strEnum->snext(status);
    }

    delete strEnum;

    return result;
}
