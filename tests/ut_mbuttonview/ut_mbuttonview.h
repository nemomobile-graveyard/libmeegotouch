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

#ifndef UT_MBUTTONVIEW_H
#define UT_MBUTTONVIEW_H

#include <QObject>
#include <QtTest/QtTest>

class MButton;
class MButtonView;

class Ut_MButtonView : public QObject
{
    Q_OBJECT
public:

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testDrawText_data();
    void testDrawText();

    void testDrawIcon_data();
    void testDrawIcon();

    void testDrawIconText_data();
    void testDrawIconText();

    void testMouseEvents();

    void testSwitchView();
    void testCheckboxView();

    //regression test for NB#220043
    void testSizeHint();

    void testDisabledSelected();

private:

    QImage captureImage(MButton *button);


    MButton *m_button;
    MButtonView *m_subject;
};

#endif

