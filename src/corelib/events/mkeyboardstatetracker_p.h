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

#ifndef MKEYBOARDSTATETRACKER_P_H
#define MKEYBOARDSTATETRACKER_P_H

#if defined(M_OS_MAEMO5)
#include "mgconfitem.h"
#endif

#ifdef HAVE_CONTEXTSUBSCRIBER
    class ContextProperty;
#endif

class MKeyboardStateTracker;

class MKeyboardStateTrackerPrivate
{
public:
    MKeyboardStateTrackerPrivate(MKeyboardStateTracker *controller);
    ~MKeyboardStateTrackerPrivate();

    static MKeyboardStateTracker *tracker;
#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty *keyboardOpenProperty;
    bool isSubscribed;
    void checkKeyboardPresent();
#elif defined(M_OS_MAEMO5)
    MGConfItem keyboardOpenConf;
#endif
    MKeyboardStateTracker *q_ptr;

    void initContextSubscriber();
    void unsubscribe();
    void subscribe();

    bool present;
    Q_DECLARE_PUBLIC(MKeyboardStateTracker)
};

#endif // MKEYBOARDSTATETRACKER_P_H
