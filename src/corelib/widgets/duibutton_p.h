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

#ifndef DUIBUTTON_P_H
#define DUIBUTTON_P_H

#include "private/duiwidgetcontroller_p.h"

class DuiStyle;
class DuiButtonGroup;

class DUI_EXPORT DuiButtonPrivate : protected DuiWidgetControllerPrivate
{
public:
    Q_DECLARE_PUBLIC(DuiButton)
    DuiButtonPrivate();
    virtual ~DuiButtonPrivate();

    void connectFeedbacks();
    void init();
    void init(const QString &iconID, const QString &text);

    DuiButtonGroup *buttonGroup;
};

#endif
