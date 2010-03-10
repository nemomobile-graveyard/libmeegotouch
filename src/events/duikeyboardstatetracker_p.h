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

#ifndef DUIKEYBOARDSTATETRACKER_P_H
#define DUIKEYBOARDSTATETRACKER_P_H

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#elif defined(HAVE_N900)
#include "duigconfitem.h"
#endif

class DuiKeyboardStateTracker;

class DuiKeyboardStateTrackerPrivate
{
public:
    DuiKeyboardStateTrackerPrivate(DuiKeyboardStateTracker *controller);

protected:
    static DuiKeyboardStateTracker *tracker;
#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty keyboardOpenProperty;
#elif defined(HAVE_N900)
    DuiGConfItem keyboardOpenConf;
#endif
    DuiKeyboardStateTracker *q_ptr;

private:
    void initContextSubscriber();

    bool present;
    Q_DECLARE_PUBLIC(DuiKeyboardStateTracker)
};

#endif // DUIKEYBOARDSTATETRACKER_P_H
