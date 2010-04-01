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

#ifndef DUIWIDGETACTION_P_H
#define DUIWIDGETACTION_P_H

#include "duiaction_p.h"

#include <QPointer>

#include "duiwidget.h"

class DuiWidgetActionPrivate : public DuiActionPrivate
{
public:
    DuiWidgetActionPrivate();
    virtual ~DuiWidgetActionPrivate();

    QPointer<DuiWidget> widget;
    bool widgetInUse;
};

#endif
