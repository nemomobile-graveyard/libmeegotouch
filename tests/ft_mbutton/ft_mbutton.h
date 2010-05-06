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

#ifndef FT_MBUTTON_H
#define FT_MBUTTON_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include "mtheme.h"
#include "mapplication.h"

class Ft_MButton : public QObject
{
    Q_OBJECT;

public:
    Ft_MButton();

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
    MApplication *app;


};

#endif

