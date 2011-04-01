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

#ifndef MSTYLE_P_H
#define MSTYLE_P_H

#include <QObject>
#include <QHash>
#include <QWeakPointer>

class MStyle;
class MWidgetController;
class MSceneManager;

class MStyleContainerPrivate
{
    friend class MStyleContainer;
public:
    MStyleContainerPrivate();
    virtual ~MStyleContainerPrivate();

private:
    void releaseStyles();

    QHash<QString, const MStyle*> cachedStyles[2];
    const MStyle *cachedCurrentStyle[2];
    const MStyle *activeStyle;

    QString objectName;
    QString type;
    QString currentMode;

    const MStyle *cachedOrientationIndependentStyle;

    MStyleContainer *q_ptr;
    const MWidgetController *parent;
    QWeakPointer<MSceneManager> sceneManager;
};

#endif
