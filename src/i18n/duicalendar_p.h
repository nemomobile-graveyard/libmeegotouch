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

#ifndef DUICALENDAR_P_H
#define DUICALENDAR_P_H

#include <unicode/calendar.h>

#include "duilocale.h"
#include "duicalendar.h"

class DuiCalendarPrivate
{
public:
    DuiCalendarPrivate(DuiLocale::CalendarType calendarType);
    DuiCalendarPrivate(const DuiCalendarPrivate &other);

    virtual ~DuiCalendarPrivate();

    DuiCalendarPrivate &operator=(const DuiCalendarPrivate &other);

    static DuiLocale::Weekday icuWeekdayToDuiWeekday(int uweekday);

    icu::Calendar *_calendar;
    DuiLocale::CalendarType _calendarType;
    bool _valid;

private:


};

#endif
