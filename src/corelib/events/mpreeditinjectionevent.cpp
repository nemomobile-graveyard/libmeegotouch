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

#include <QString>
#include <QMutex>

#include "mpreeditinjectionevent.h"
#include "mpreeditinjectionevent_p.h"


static int sPreeditEventNumber = -1;
static QMutex sPreeditInjectionMutex;

MPreeditInjectionEventPrivate::MPreeditInjectionEventPrivate(const QString &preedit)
    : preedit(preedit)
{
    // nothing
}


MPreeditInjectionEventPrivate::~MPreeditInjectionEventPrivate()
{
    // nothing
}


///////////////////////
// class implementation


MPreeditInjectionEvent::MPreeditInjectionEvent(const QString &preedit)
    : QEvent(MPreeditInjectionEvent::eventNumber()),
      d_ptr(new MPreeditInjectionEventPrivate(preedit))
{
    setAccepted(false);
}


MPreeditInjectionEvent::~MPreeditInjectionEvent()
{
    delete d_ptr;
}


QString MPreeditInjectionEvent::preedit() const
{
    Q_D(const MPreeditInjectionEvent);
    return d->preedit;
}


// static
QEvent::Type MPreeditInjectionEvent::eventNumber()
{
    if (sPreeditEventNumber < 0) {
        // no event number yet registered, do it now
        sPreeditInjectionMutex.lock();

        if (sPreeditEventNumber < 0) {
            sPreeditEventNumber = QEvent::registerEventType();
        }

        sPreeditInjectionMutex.unlock();
    }

    return static_cast<QEvent::Type>(sPreeditEventNumber);
}
