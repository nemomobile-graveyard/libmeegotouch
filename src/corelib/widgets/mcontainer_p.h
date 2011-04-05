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

#ifndef MMASHUPCONTAINER_P_H
#define MMASHUPCONTAINER_P_H

#include "private/mwidgetcontroller_p.h"

class MStyle;
class MWidget;
class MProgressIndicator;

class MContainerPrivate : protected MWidgetControllerPrivate
{
public:
    Q_DECLARE_PUBLIC(MContainer)
    MContainerPrivate();
    virtual ~MContainerPrivate();

    void init(const QString &newTitle = QString::null);
    void _q_onCentralWidgetDestroyed();
};

#endif
