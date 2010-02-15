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

#ifndef DUIBUTTONGROUP_P_H
#define DUIBUTTONGROUP_P_H

#include <QObject>
#include <QMap>
#include <QList>
#include "duibuttongroup.h"

class DuiButton;

class DuiButtonGroupPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(DuiButtonGroup)

public:
    DuiButtonGroupPrivate();
    virtual ~DuiButtonGroupPrivate();
    DuiButton *checkedButton;
    bool exclusive;
    QList<DuiButton *> buttonList;
    QMap<DuiButton *, int> buttonIdMap;

    DuiButtonGroup *q_ptr;

public slots:

    void buttonClick();
    void buttonPress();
    void buttonRelease();
    void buttonToggle(bool);
};

#endif

