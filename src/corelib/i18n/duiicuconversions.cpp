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

#include "duiicuconversions.h"

#include <QString>
#include <unicode/unistr.h>
#include <unicode/datefmt.h>

#include "duilocale_p.h"

namespace
{
// string presentations for DuiLocale::Collate and DuiLocale::CalendarType
// Keep in sync!

    const char *const CollationNames[] = {"", "phonebook", "pinyin", "traditional", "stroke", "direct",
                                          "posix", "big5han", "gb2312han"
                                         };


    const char *const CalendarNames[] = {"", "gregorian", "islamic", "chinese", "islamic-civil", "hebrew",
                                         "japanese", "buddhist", "persian", "coptic", "ethiopic"
                                        };
}

icu::UnicodeString DuiIcuConversions::qStringToUnicodeString(const QString &sourceStr)
{
    return UnicodeString(static_cast<const UChar *>(sourceStr.utf16()));
}

QString DuiIcuConversions::unicodeStringToQString(const icu::UnicodeString &sourceStr)
{
    return QString(reinterpret_cast<const QChar *>(sourceStr.getBuffer()),
                   sourceStr.length());
}

icu::DateFormat::EStyle DuiIcuConversions::toEStyle(DuiLocale::DateType dateType)
{
    if (dateType == DuiLocale::DateNone) {
        return icu::DateFormat::kNone;
    }

    if (dateType == DuiLocale::DateShort) {
        return icu::DateFormat::kShort;
    }

    if (dateType == DuiLocale::DateMedium) {
        return icu::DateFormat::kMedium;
    }

    if (dateType == DuiLocale::DateLong) {
        return icu::DateFormat::kLong;
    }

    return icu::DateFormat::kFull;
}

icu::DateFormat::EStyle DuiIcuConversions::toEStyle(DuiLocale::TimeType timeType)
{
    if (timeType == DuiLocale::TimeNone) {
        return icu::DateFormat::kNone;
    }

    if (timeType == DuiLocale::TimeShort) {
        return icu::DateFormat::kShort;
    }

    if (timeType == DuiLocale::TimeMedium) {
        return icu::DateFormat::kMedium;
    }

    if (timeType == DuiLocale::TimeLong) {
        return DateFormat::kLong;
    }

    return icu::DateFormat::kFull;
}

QString DuiIcuConversions::collationToString(DuiLocale::Collation coll)
{
    if (static_cast<unsigned int>(coll) >= (sizeof(CollationNames) / sizeof(char *))) {
        return "";
    }

    return CollationNames[coll];
}

QString DuiIcuConversions::calendarToString(DuiLocale::CalendarType cal)
{
    if (static_cast<unsigned int>(cal) >= (sizeof(CalendarNames) / sizeof(char *))) {
        return "";
    }

    return CalendarNames[cal];
}

icu::DateFormatSymbols::DtContextType
DuiIcuConversions::duiDateContextToIcu(DuiLocale::DateSymbolContext context)
{
    icu::DateFormatSymbols::DtContextType icuContext;

    if (context == DuiLocale::DateSymbolFormat) {
        icuContext = icu::DateFormatSymbols::FORMAT;
    } else {
        icuContext = icu::DateFormatSymbols::STANDALONE;
    }

    return icuContext;
}


icu::DateFormatSymbols::DtWidthType
DuiIcuConversions::duiDateWidthToIcu(DuiLocale::DateSymbolLength length)
{
    icu::DateFormatSymbols::DtWidthType icuWidth;

    switch (length) {
    case DuiLocale::DateSymbolAbbreviated:
        icuWidth = icu::DateFormatSymbols::ABBREVIATED;
        break;

    case DuiLocale::DateSymbolWide:
        icuWidth = icu::DateFormatSymbols::WIDE;
        break;

    case DuiLocale::DateSymbolNarrow:
    default:
        icuWidth = icu::DateFormatSymbols::NARROW;
        break;
    }

    return icuWidth;
}

UCalendarDaysOfWeek DuiIcuConversions::icuWeekday(int duiWeekday)
{
    int weekdayNum = duiWeekday;

    // sunday = 1, monday = 2 etc on the array
    if (duiWeekday == DuiLocale::Sunday) {
        weekdayNum = 1;
    } else {
        weekdayNum++;
    }

    return static_cast<UCalendarDaysOfWeek>(weekdayNum);
}

int DuiIcuConversions::duiWeekday(int icuWeekday)
{
    if (icuWeekday == UCAL_SUNDAY) {
        return DuiLocale::Sunday; // 1 -> 7
    } else {
        return icuWeekday - 1;
    }
}

QString DuiIcuConversions::icuDatePatternEscaped(const QString &str)
{
    QString result = str;
    return result.replace('\'', "''");
}

icu::Locale DuiIcuConversions::createLocale(const QString &baseString,
        DuiLocale::CalendarType calendarType,
        DuiLocale::Collation collation)
{
    // calendarType and collation are appended as @keyword=value;keyword2=value2;
    // first create the attribute part keyword=value;...
    QString attributeAccu;

    if (collation != DuiLocale::DefaultCollation) {
        attributeAccu.append("collation=");

        QString collName = DuiIcuConversions::collationToString(collation);
        attributeAccu.append(collName);
        attributeAccu.append(";");
    }

    if (calendarType != DuiLocale::DefaultCalendar) {
        attributeAccu.append("calendar=");
        QString calendarName = DuiIcuConversions::calendarToString(calendarType);
        attributeAccu.append(calendarName);
    }

    QString baseStringModified = baseString;
    // ICU doesn't accept trailing "@" with no actual attributes
    if (!attributeAccu.isEmpty()) {
        baseStringModified.append("@");
        baseStringModified.append(attributeAccu);
    }

    return icu::Locale(qPrintable(baseStringModified));
}

