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

#ifndef FT_DUIBUTTON_H
#define FT_DUIBUTTON_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include "duitheme.h"
#include "duiapplication.h"

class Ft_DuiButton : public QObject
{
    Q_OBJECT;

public:
    Ft_DuiButton();

private slots:
    void initTestCase();
    void cleanupTestCase();


    void buttonPaint();
    void buttonSetObjectName();
    void buttonSetValues();
    void testSignalClicked();
    void testSignalToggled();
    void testSignalPressed();
    void testSignalReleased();


    /* void buttonBoxed(); */

private:
    DuiApplication *app;


};

#endif

