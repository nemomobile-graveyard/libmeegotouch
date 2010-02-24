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

#ifndef DUISTYLE_P_H
#define DUISTYLE_P_H

#include <QObject>
#include <QHash>

class DuiStyle;
class DuiWidgetController;

class DuiStyleContainerPrivate
{
    friend class DuiStyleContainer;
public:
    DuiStyleContainerPrivate();
    virtual ~DuiStyleContainerPrivate();
private:
    void releaseStyles();

    QHash<QString, const DuiStyle*> cachedStyles[2];
    const DuiStyle *cachedCurrentStyle[2];

    QString objectName;
    QString type;
    QString currentMode;

    DuiStyleContainer *q_ptr;
    const DuiWidgetController *parent;
};

#endif
