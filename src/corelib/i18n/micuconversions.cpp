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

#include <QDebug>
#include <QString>
#include <unicode/unistr.h>
#include <unicode/datefmt.h>

#include "mlocale_p.h"

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

QString MIcuConversions::layoutDirectionToString(Qt::LayoutDirection layoutDirection)
{
    QString layoutDirectionName;
    switch(layoutDirection) {
    case Qt::LeftToRight:
        layoutDirectionName = "ltr";
        break;
    case Qt::RightToLeft:
        layoutDirectionName = "rtl";
        break;
    case Qt::LayoutDirectionAuto:
    default:
        layoutDirectionName = "auto";
        break;
    }
    return layoutDirectionName;
}

Qt::LayoutDirection MIcuConversions::stringToLayoutDirection(QString layoutDirectionName)
{
    if(layoutDirectionName == "rtl")
        // force layout direction to RTL
        return Qt::RightToLeft;
    else if(layoutDirectionName == "ltr")
        // force layout direction to LTR
        return Qt::LeftToRight;
    else if(layoutDirectionName == "auto")
        // determine layout direction from the locale
        return Qt::LayoutDirectionAuto;
    else
        // no option means force layout direction to LTR:
        return Qt::LeftToRight;
}

QString MIcuConversions::collationToString(MLocale::Collation coll)
{
    QString collationName;
    switch(coll) {
    case MLocale::PhonebookCollation:
        collationName = "phonebook";
        break;
    case MLocale::PinyinCollation:
        collationName = "pinyin";
        break;
    case MLocale::TraditionalCollation:
        collationName = "traditional";
        break;
    case MLocale::StrokeCollation:
        collationName = "stroke";
        break;
    case MLocale::DirectCollation:
        collationName = "direct";
        break;
    case MLocale::PosixCollation:
        collationName = "posix";
        break;
    case MLocale::Big5hanCollation:
        collationName = "big5han";
        break;
    case MLocale::Gb2312hanCollation:
        collationName = "gb2312han";
        break;
    case MLocale::DefaultCollation:
    default:
        collationName = "";
        break;
    }
    return collationName;
}

MLocale::Collation MIcuConversions::stringToCollation(QString collationName)
{
    if(collationName == "phonebook")
        return MLocale::PhonebookCollation;
    else if (collationName == "pinyin")
        return MLocale::PinyinCollation;
    else if (collationName == "traditional")
        return MLocale::TraditionalCollation;
    else if (collationName == "stroke")
        return MLocale::StrokeCollation;
    else if (collationName == "direct")
        return MLocale::DirectCollation;
    else if (collationName == "posix")
        return MLocale::PosixCollation;
    else if (collationName == "big5han")
        return MLocale::Big5hanCollation;
    else if (collationName == "gb2312han")
        return MLocale::Gb2312hanCollation;
    else
        return MLocale::DefaultCollation;
}

QString MIcuConversions::calendarToString(MLocale::CalendarType calendarType)
{
    QString calendarTypeName;
    switch(calendarType) {
    case MLocale::GregorianCalendar:
        calendarTypeName = "gregorian";
        break;
    case MLocale::IslamicCalendar:
        calendarTypeName = "islamic";
        break;
    case MLocale::ChineseCalendar:
        calendarTypeName = "chinese";
        break;
    case MLocale::IslamicCivilCalendar:
        calendarTypeName = "islamic-civil";
        break;
    case MLocale::HebrewCalendar:
        calendarTypeName = "hebrew";
        break;
    case MLocale::JapaneseCalendar:
        calendarTypeName = "japanese";
        break;
    case MLocale::BuddhistCalendar:
        calendarTypeName = "buddhist";
        break;
    case MLocale::PersianCalendar:
        calendarTypeName = "persian";
        break;
    case MLocale::CopticCalendar:
        calendarTypeName = "coptic";
        break;
    case MLocale::EthiopicCalendar:
        calendarTypeName = "ethiopic";
        break;
    case MLocale::DefaultCalendar:
    default:
        calendarTypeName = "";
        break;
    }
    return calendarTypeName;
}

MLocale::CalendarType MIcuConversions::stringToCalendar(QString calendarTypeName)
{
    if(calendarTypeName == "gregorian")
        return MLocale::GregorianCalendar;
    else if (calendarTypeName == "islamic")
        return MLocale::IslamicCalendar;
    else if (calendarTypeName == "chinese")
        return MLocale::ChineseCalendar;
    else if (calendarTypeName == "islamic-civil")
        return MLocale::IslamicCivilCalendar;
    else if (calendarTypeName == "hebrew")
        return MLocale::HebrewCalendar;
    else if (calendarTypeName == "japanese")
        return MLocale::JapaneseCalendar;
    else if (calendarTypeName == "buddhist")
        return MLocale::BuddhistCalendar;
    else if (calendarTypeName == "persian")
        return MLocale::PersianCalendar;
    else if (calendarTypeName == "coptic")
        return MLocale::CopticCalendar;
    else if (calendarTypeName == "ethiopic")
        return MLocale::EthiopicCalendar;
    else
        return MLocale::DefaultCalendar;
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

QString MIcuConversions::parseOption(const QString &localeName, const QString &option)
{
    QString value;
    QRegExp regexp("^[^@]+@.*"+QRegExp::escape(option)+"=([^@=;]+)($|;.*$)");
    if(regexp.indexIn(localeName) >= 0 && regexp.capturedTexts().size() == 3) {
        value = regexp.capturedTexts().at(1);
    }
    return value;
}

QString MIcuConversions::setOption(const QString &localeName, const QString &option, const QString &value)
{
    QString newLocaleName = localeName;
    if(!newLocaleName.isEmpty() && !option.isEmpty()) {
        if(value.isEmpty()) { // remove option completely
            if(newLocaleName.contains('@') && newLocaleName.contains(option)) {
                QRegExp regexp("^([^@]+@.*)"+QRegExp::escape(option)+"=[^@=;]+($|;.*$)");
                newLocaleName.replace(regexp, "\\1\\2");
                newLocaleName.replace(QLatin1String(";;"), QLatin1String(";"));
                newLocaleName.replace(QLatin1String("@;"), QLatin1String("@"));
                while(newLocaleName.endsWith(';'))
                    newLocaleName.chop(1);
                if(newLocaleName.endsWith('@'))
                    newLocaleName.chop(1);
            }
        }
        else { // replace option value
            if(!newLocaleName.contains('@')) {
                newLocaleName += '@' + option + '=' + value;
            }
            else if(!newLocaleName.contains(option)) {
                if(newLocaleName.endsWith(';'))
                    newLocaleName += option + '=' + value;
                else
                    newLocaleName += ';' + option + '=' + value;
            }
            else {
                QRegExp regexp("^([^@]+@.*"+QRegExp::escape(option)+"=)[^@=;]+($|;.*$)");
                newLocaleName.replace(regexp, "\\1"+value+"\\2");
            }
        }
    }
    return newLocaleName;
}

Qt::LayoutDirection MIcuConversions::parseLayoutDirectionOption(const QString &localeName)
{
    return MIcuConversions::stringToLayoutDirection(
        MIcuConversions::parseOption(localeName, "layout-direction"));
}

QString MIcuConversions::setLayoutDirectionOption(const QString &localeName, Qt::LayoutDirection layoutDirection)
{
    return MIcuConversions::setOption(
        localeName, "layout-direction",
        MIcuConversions::layoutDirectionToString(layoutDirection));
}

MLocale::CalendarType MIcuConversions::parseCalendarOption(const QString &localeName)
{
    return MIcuConversions::stringToCalendar(
        MIcuConversions::parseOption(localeName, "calendar"));
}

QString MIcuConversions::setCalendarOption(const QString &localeName, MLocale::CalendarType calendarType)
{
    return MIcuConversions::setOption(
        localeName, "calendar",
        MIcuConversions::calendarToString(calendarType));
}

MLocale::Collation MIcuConversions::parseCollationOption(const QString &localeName)
{
    return MIcuConversions::stringToCollation(
        MIcuConversions::parseOption(localeName, "collation"));
}

QString MIcuConversions::setCollationOption(const QString &localeName, MLocale::Collation collation)
{
    return MIcuConversions::setOption(
        localeName, "collation",
        MIcuConversions::collationToString(collation));
}
