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

#ifndef MINPUTMETHODSTATE_P_H
#define MINPUTMETHODSTATE_P_H

#include <QObject>
#include <QMap>

#include "minputmethodstate.h"
#include "mnamespace.h"

class QTimer;

class MInputMethodStatePrivate
{
    Q_DECLARE_PUBLIC(MInputMethodState)
public:
    MInputMethodStatePrivate();
    ~MInputMethodStatePrivate();

    void init();

    void _q_emitInputMethodAreaChanged();

    //! Current input method area
    QRect region;

    //! Current orientation of application's active window
    M::OrientationAngle orientation;

    QTimer *emitInputMethodAreaTimer;

    QMap<int, QString> toolbars;

private:
    Q_DISABLE_COPY(MInputMethodStatePrivate)

    MInputMethodState *q_ptr;
};

#endif
