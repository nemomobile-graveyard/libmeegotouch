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

#ifndef DUIINPUTMETHODSTATE_P_H
#define DUIINPUTMETHODSTATE_P_H

#include <QObject>

#include "duiinputmethodstate.h"
#include "duinamespace.h"

class DuiInputMethodStatePrivate
{
public:
    DuiInputMethodStatePrivate();

    //! Current input method area
    QRect region;

    //! Current orientation of application's active window
    Dui::OrientationAngle orientation;

private:
    Q_DISABLE_COPY(DuiInputMethodStatePrivate)
};

#endif
