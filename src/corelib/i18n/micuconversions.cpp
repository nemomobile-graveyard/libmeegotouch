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

#include "micuconversions.h"

#include <QString>
#include <unicode/unistr.h>
#include <unicode/datefmt.h>

#include "mlocale_p.h"

namespace
{
// string presentations for MLocale::Collate and MLocale::CalendarType
// Keep in sync!

    const char *const CollationNames[] = {"", "phonebook", "pinyin", "traditional", "stroke", "direct",
                                          "posix", "big5han", "gb2312han"
                                         };


    const char *const CalendarNames[] = {"", "gregorian", "islamic", "chinese", "islamic-civil", "hebrew",
                                         "japanese", "buddhist", "persian", "coptic", "ethiopic"
                                        };
}

icu::UnicodeString MIcuConversions::qStringToUnicodeString(const QString &sourceStr)
{
    return UnicodeString(static_cast<const UChar *>(sourceStr.utf16()));
}

QString MIcuConversions::unicodeStringToQString(const icu::UnicodeString &sourceStr)
{
    return QString(reinterpret_cast<const QChar *>(sourceStr.getBuffer()),
                   sourceStr.length());
}

icu::DateFormat::EStyle MIcuConversions::toEStyle(MLocale::DateType dateType)
{
    if (dateType == MLocale::DateNone) {
        return icu::DateFormat::kNone;
    }

    if (dateType == MLocale::DateShort) {
        return icu::DateFormat::kShort;
    }

    if (dateType == MLocale::DateMedium) {
        return icu::DateFormat::kMedium;
    }

    if (dateType == MLocale::DateLong) {
        return icu::DateFormat::kLong;
    }

    return icu::DateFormat::kFull;
}

icu::DateFormat::EStyle MIcuConversions::toEStyle(MLocale::TimeType timeType)
{
    if (timeType == MLocale::TimeNone) {
        return icu::DateFormat::kNone;
    }

    if (timeType == MLocale::TimeShort) {
        return icu::DateFormat::kShort;
    }

    if (timeType == MLocale::TimeMedium) {
        return icu::DateFormat::kMedium;
    }

    if (timeType == MLocale::TimeLong) {
        return DateFormat::kLong;
    }

    return icu::DateFormat::kFull;
}

QString MIcuConversions::collationToString(MLocale::Collation coll)
{
    if (static_cast<unsigned int>(coll) >= (sizeof(CollationNames) / sizeof(char *))) {
        return "";
    }

    return CollationNames[coll];
}

QString MIcuConversions::calendarToString(MLocale::CalendarType cal)
{
    if (static_cast<unsigned int>(cal) >= (sizeof(CalendarNames) / sizeof(char *))) {
        return "";
    }

    return CalendarNames[cal];
}

icu::DateFormatSymbols::DtContextType
MIcuConversions::mDateContextToIcu(MLocale::DateSymbolContext context)
{
    icu::DateFormatSymbols::DtContextType icuContext;

    if (context == MLocale::DateSymbolFormat) {
        icuContext = icu::DateFormatSymbols::FORMAT;
    } else {
        icuContext = icu::DateFormatSymbols::STANDALONE;
    }

    return icuContext;
}


icu::DateFormatSymbols::DtWidthType
MIcuConversions::mDateWidthToIcu(MLocale::DateSymbolLength length)
{
    icu::DateFormatSymbols::DtWidthType icuWidth;

    switch (length) {
    case MLocale::DateSymbolAbbreviated:
        icuWidth = icu::DateFormatSymbols::ABBREVIATED;
        break;

    case MLocale::DateSymbolWide:
        icuWidth = icu::DateFormatSymbols::WIDE;
        break;

    case MLocale::DateSymbolNarrow:
    default:
        icuWidth = icu::DateFormatSymbols::NARROW;
        break;
    }

    return icuWidth;
}

UCalendarDaysOfWeek MIcuConversions::icuWeekday(int mWeekday)
{
    int weekdayNum = mWeekday;

    // sunday = 1, monday = 2 etc on the array
    if (mWeekday == MLocale::Sunday) {
        weekdayNum = 1;
    } else {
        weekdayNum++;
    }

    return static_cast<UCalendarDaysOfWeek>(weekdayNum);
}

int MIcuConversions::mWeekday(int icuWeekday)
{
    if (icuWeekday == UCAL_SUNDAY) {
        return MLocale::Sunday; // 1 -> 7
    } else {
        return icuWeekday - 1;
    }
}

QString MIcuConversions::icuDatePatternEscaped(const QString &str)
{
    QString result = str;
    return result.replace('\'', "''");
}

icu::Locale MIcuConversions::createLocale(const QString &baseString,
        MLocale::CalendarType calendarType,
        MLocale::Collation collation)
{
    // calendarType and collation are appended as @keyword=value;keyword2=value2;
    // first create the attribute part keyword=value;...
    QString attributeAccu;

    if (collation != MLocale::DefaultCollation) {
        attributeAccu.append("collation=");

        QString collName = MIcuConversions::collationToString(collation);
        attributeAccu.append(collName);
        attributeAccu.append(";");
    }

    if (calendarType != MLocale::DefaultCalendar) {
        attributeAccu.append("calendar=");
        QString calendarName = MIcuConversions::calendarToString(calendarType);
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

