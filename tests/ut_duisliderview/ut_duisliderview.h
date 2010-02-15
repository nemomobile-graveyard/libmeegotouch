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

#ifndef UT_DUISLIDERVIEW_H
#define UT_DUISLIDERVIEW_H

#include <QObject>
#include <QtTest/QtTest>

class DuiSeekBar;
class DuiSliderView;

class Ut_DuiSliderView : public QObject
{
    Q_OBJECT
public:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void sliderResize();

private:
    DuiSeekBar *m_seekbar;
    DuiSliderView *m_subject;
};

#endif

