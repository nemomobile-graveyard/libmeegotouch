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

#ifndef UT_MBUTTONGROUP_H
#define UT_MBUTTONGROUP_H

#include <QtTest/QtTest>
#include <QObject>

#include <mbuttongroup.h>

Q_DECLARE_METATYPE(MButtonGroup *);
Q_DECLARE_METATYPE(MButton *);

class Ut_MButtonGroup : public QObject
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

    void testMakePopulatedGroupExclusive();
    void testMakePopulatedGroupWithTwoCkeckedButtonsExclusive();
    void testRemoveCheckedButton();
    void testNonCheckableButtonMadeChecked();

    void id();

    void removeButton();

    void setId();

    void buttonPressReleaseClick();

private:
    MButtonGroup *m_subject;
};

#endif
