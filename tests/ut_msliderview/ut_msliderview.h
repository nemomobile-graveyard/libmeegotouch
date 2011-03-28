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

#ifndef UT_MSLIDERVIEW_H
#define UT_MSLIDERVIEW_H

#include <QObject>
#include <QtTest/QtTest>

class MSeekBar;
class MSliderView;

class Ut_MSliderView : public QObject
{
    Q_OBJECT
public:
    Ut_MSliderView();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void sliderResize();
    void sliderGrooveMargin();
    void sliderPositionByValue();

private:
    MSeekBar *m_seekbar;
    MSliderView *m_subject;
};

#endif

