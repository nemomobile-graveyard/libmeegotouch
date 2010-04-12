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

#ifndef MICUCONVERSIONS_H
#define MICUCONVERSIONS_H

#include <unicode/unistr.h>
#include <unicode/datefmt.h>
#include <unicode/dtfmtsym.h>

#include "mlocale.h"

class QString;

//! \internal

namespace MIcuConversions
{
    /*!
     * \brief converts a QString into an icu::UnicodeString
     *
     * @param sourceStr The QString which will be converted into an icu::UnicodeString.
     *
     * \sa MIcuConversions::unicodeStringToQString()
     */
    icu::UnicodeString qStringToUnicodeString(const QString &sourceStr);

    /*!
     * \brief converts an icu::UnicodeString into a QString
     *
     * @param sourceStr The icu::UnicodeString which will be converted into a QString.
     *
     * \sa MIcuConversions::qStringToUnicodeString()
     */
    QString unicodeStringToQString(const icu::UnicodeString &sourceStr);

    /*!
     * \brief transforms MLocale::DateType enums to icu::DateFormat::EStyle enums
     *
     * @param dateType The MLocale::DateType enum
     *
     * \sa MIcuConversions::toEStyle(MLocale::TimeType &timeType)
     */
    icu::DateFormat::EStyle toEStyle(MLocale::DateType dateType);

    /*!
     * \brief transforms MLocale::TimeType enums to icu::DateFormat::EStyle enums
     *
     * @param timeType The MLocale::TimeType enum
     *
     * \sa MIcuConversions::toEStyle(MLocale::DateType &dateType)
     */
    icu::DateFormat::EStyle toEStyle(MLocale::TimeType timeType);

    /*!
     * \brief transforms MLocale::Collation enum to icu string presentations
     *
     * @param coll MLocale::Collation enum
     *
     * \sa MIcuConversions::calendarToString(MLocale::CalendarType cal)
     */
    QString collationToString(MLocale::Collation coll);

    /*!
     * \brief transforms MLocale::CalendarType enum to icu string presentations
     *
     * @param cal MLocale::CalendarType enum
     *
     * \sa MIcuConversions::collationToString(MLocale::Collation coll);
     */
    QString calendarToString(MLocale::CalendarType cal);

    /*!
     * \brief transforms MLocale::DateSymbolContext to icu::DateFormatSymbols::DtContextType
     *
     * @param context MLocale::DateSymbolContext
     *
     * \sa MIcuConversions::mDateWidthToIcu(MLocale::DateSymbolLength length)
     */
    icu::DateFormatSymbols::DtContextType mDateContextToIcu(MLocale::DateSymbolContext context);

    /*!
     * \brief transforms MLocale::DateSymbolLength to icu::DateFormatSymbols::DtWidthType
     *
     * @param length MLocale::DateSymbolLength
     *
     * \sa MIcuConversions::mDateContextToIcu(MLocale::DateSymbolContext context)
     */
    icu::DateFormatSymbols::DtWidthType mDateWidthToIcu(MLocale::DateSymbolLength length);

    /*!
     * \brief transforms a M weekday to an ICU weekday.
     *
     * ICU and M use different numbering for weekdays:
     * ICU: 1 = Sunday, 2 Monday, ..., 7 = Saturday
     * M: 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
     *
     * \sa mWeekday(int icuWeekday)
     */
    UCalendarDaysOfWeek icuWeekday(int mWeekday);

    /*!
     * \brief transforms an ICU weekday to a M weekday
     *
     * ICU and M use different numbering for weekdays:
     * ICU: 1 = Sunday, 2 Monday, ..., 7 = Saturday
     * M: 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
     *
     * \sa icuWeekday(int mWeekday);
     */
    int mWeekday(int icuWeekday);

    /*!
     * \brief escapes all 's in a string as ''
     *
     * ICU date patterns need to have 's escaped as ''
     *
     */
    QString icuDatePatternEscaped(const QString &str);

    /*!
     * \brief creates an icu::Locale based on a locale string and additional parameters
     */
    icu::Locale createLocale(const QString &baseString,
                             MLocale::CalendarType calendarType = MLocale::DefaultCalendar,
                             MLocale::Collation collation = MLocale::DefaultCollation);
}

//! \internal_end

#endif
