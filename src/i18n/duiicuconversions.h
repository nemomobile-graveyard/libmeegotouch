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

#ifndef DUIICUCONVERSIONS_H
#define DUIICUCONVERSIONS_H

#include <unicode/unistr.h>
#include <unicode/datefmt.h>
#include <unicode/dtfmtsym.h>

#include "duilocale.h"

class QString;

//! \internal

namespace DuiIcuConversions
{
    /*!
     * \brief converts a QString into an icu::UnicodeString
     *
     * @param sourceStr The QString which will be converted into an icu::UnicodeString.
     *
     * \sa DuiIcuConversions::unicodeStringToQString()
     */
    icu::UnicodeString qStringToUnicodeString(const QString &sourceStr);

    /*!
     * \brief converts an icu::UnicodeString into a QString
     *
     * @param sourceStr The icu::UnicodeString which will be converted into a QString.
     *
     * \sa DuiIcuConversions::qStringToUnicodeString()
     */
    QString unicodeStringToQString(const icu::UnicodeString &sourceStr);

    /*!
     * \brief transforms DuiLocale::DateType enums to icu::DateFormat::EStyle enums
     *
     * @param dateType The DuiLocale::DateType enum
     *
     * \sa DuiIcuConversions::toEStyle(DuiLocale::TimeType &timeType)
     */
    icu::DateFormat::EStyle toEStyle(DuiLocale::DateType dateType);

    /*!
     * \brief transforms DuiLocale::TimeType enums to icu::DateFormat::EStyle enums
     *
     * @param timeType The DuiLocale::TimeType enum
     *
     * \sa DuiIcuConversions::toEStyle(DuiLocale::DateType &dateType)
     */
    icu::DateFormat::EStyle toEStyle(DuiLocale::TimeType timeType);

    /*!
     * \brief transforms DuiLocale::Collation enum to icu string presentations
     *
     * @param coll DuiLocale::Collation enum
     *
     * \sa DuiIcuConversions::calendarToString(DuiLocale::Calendar cal)
     */
    QString collationToString(DuiLocale::Collation coll);

    /*!
     * \brief transforms DuiLocale::Calendar enum to icu string presentations
     *
     * @param cal DuiLocale::Calendar enum
     *
     * \sa DuiIcuConversions::collationToString(DuiLocale::Collation coll);
     */
    QString calendarToString(DuiLocale::Calendar cal);

    /*!
     * \brief transforms DuiLocale::DateSymbolContext to icu::DateFormatSymbols::DtContextType
     *
     * @param context DuiLocale::DateSymbolContext
     *
     * \sa DuiIcuConversions::duiDateWidthToIcu(DuiLocale::DateSymbolLength length)
     */
    icu::DateFormatSymbols::DtContextType duiDateContextToIcu(DuiLocale::DateSymbolContext context);

    /*!
     * \brief transforms DuiLocale::DateSymbolLength to icu::DateFormatSymbols::DtWidthType
     *
     * @param length DuiLocale::DateSymbolLength
     *
     * \sa DuiIcuConversions::duiDateContextToIcu(DuiLocale::DateSymbolContext context)
     */
    icu::DateFormatSymbols::DtWidthType duiDateWidthToIcu(DuiLocale::DateSymbolLength length);

    /*!
     * \brief transforms a Dui weekday to an ICU weekday.
     *
     * ICU and Dui use different numbering for weekdays:
     * ICU: 1 = Sunday, 2 Monday, ..., 7 = Saturday
     * Dui: 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
     *
     * \sa duiWeekday(int icuWeekday)
     */
    UCalendarDaysOfWeek icuWeekday(int duiWeekday);

    /*!
     * \brief transforms an ICU weekday to a Dui weekday
     *
     * ICU and Dui use different numbering for weekdays:
     * ICU: 1 = Sunday, 2 Monday, ..., 7 = Saturday
     * Dui: 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
     *
     * \sa icuWeekday(int duiWeekday);
     */
    int duiWeekday(int icuWeekday);

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
                             DuiLocale::Calendar calendar = DuiLocale::DefaultCalendar,
                             DuiLocale::Collation collation = DuiLocale::DefaultCollation);
}

//! \internal_end

#endif
