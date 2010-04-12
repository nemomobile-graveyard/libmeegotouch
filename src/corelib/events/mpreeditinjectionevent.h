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

#ifndef MPREEDITINJECTIONEVENT_H
#define MPREEDITINJECTIONEVENT_H

#include "mexport.h"

#include <QEvent>

class MPreeditInjectionEventPrivate;

class M_EXPORT MPreeditInjectionEvent : public QEvent
{
public:
    MPreeditInjectionEvent(const QString &preedit);
    virtual ~MPreeditInjectionEvent();

    QString preedit() const;

    static QEvent::Type eventNumber();

protected:
    MPreeditInjectionEventPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(MPreeditInjectionEvent)
    Q_DISABLE_COPY(MPreeditInjectionEvent)
};

#endif
