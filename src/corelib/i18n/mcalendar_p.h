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

#ifndef MCALENDAR_P_H
#define MCALENDAR_P_H

#include <unicode/calendar.h>

#include "mlocale.h"
#include "mcalendar.h"

#ifdef HAVE_QMSYSTEM2
#include <qmtime.h>
#endif

class MTimeZoneWatcher;


class MCalendarPrivate
{
public:
    MCalendarPrivate(MLocale::CalendarType calendarType);
    MCalendarPrivate(const MCalendarPrivate &other);

    virtual ~MCalendarPrivate();

    MCalendarPrivate &operator=(const MCalendarPrivate &other);

    static MLocale::Weekday icuWeekdayToMWeekday(int uweekday);

    icu::Calendar *_calendar;
    MLocale::CalendarType _calendarType;
    bool _valid;
    static MTimeZoneWatcher *_watcher;

private:


};

class MTimeZoneWatcher : public QObject
{
    Q_OBJECT
public:
    MTimeZoneWatcher();
    virtual ~MTimeZoneWatcher();

private Q_SLOTS:
#ifdef HAVE_QMSYSTEM2
    void timeOrSettingsChangedSlot( MeeGo::QmTime::WhatChanged );

private:
    MeeGo::QmTime *_qmtime;
#endif
};

#endif
