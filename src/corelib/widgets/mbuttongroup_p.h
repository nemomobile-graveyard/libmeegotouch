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

#ifndef MBUTTONGROUP_P_H
#define MBUTTONGROUP_P_H

#include <QObject>
#include <QMap>
#include <QList>
#include "mbuttongroup.h"

class MButton;

class MButtonGroupPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MButtonGroup)

public:
    MButtonGroupPrivate();
    virtual ~MButtonGroupPrivate();
    MButton *checkedButton;
    bool exclusive;
    QList<MButton *> buttonList;
    QMap<MButton *, int> buttonIdMap;

    MButtonGroup *q_ptr;

public slots:

    void buttonClick();
    void buttonPress();
    void buttonRelease();
    void buttonToggle(bool);
    void enforceExclusivity();
};

#endif

