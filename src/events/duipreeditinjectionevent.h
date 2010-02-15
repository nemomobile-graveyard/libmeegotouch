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

#ifndef DUIPREEDITINJECTIONEVENT_H
#define DUIPREEDITINJECTIONEVENT_H

#include "duiexport.h"

#include <QEvent>

class DuiPreeditInjectionEventPrivate;

class DUI_EXPORT DuiPreeditInjectionEvent : public QEvent
{
public:
    DuiPreeditInjectionEvent(const QString &preedit);
    virtual ~DuiPreeditInjectionEvent();

    QString preedit() const;

    static QEvent::Type eventNumber();

protected:
    DuiPreeditInjectionEventPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(DuiPreeditInjectionEvent)
    Q_DISABLE_COPY(DuiPreeditInjectionEvent)
};

#endif
