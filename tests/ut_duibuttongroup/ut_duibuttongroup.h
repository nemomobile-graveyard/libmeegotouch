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

#ifndef UT_DUIBUTTONGROUP_H
#define UT_DUIBUTTONGROUP_H

#include <QtTest/QtTest>
#include <QObject>

#include <duibuttongroup.h>

Q_DECLARE_METATYPE(DuiButtonGroup *);
Q_DECLARE_METATYPE(DuiButton *);

class Ut_DuiButtonGroup : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void addButton();

    void button();

    void buttons();

    void checkedButton();

    void checkedId();

    void exclusive();

    void switchModes();

    void id();

    void removeButton();

    void setId();

    void buttonPressReleaseClick();

private:
    DuiButtonGroup *m_subject;
};

#endif
