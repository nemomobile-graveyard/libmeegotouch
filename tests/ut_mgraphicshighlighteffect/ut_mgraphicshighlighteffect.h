/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_MGRAPHICSCLIPEFFECT_H
#define UT_MGRAPHICSCLIPEFFECT_H

#include <QtTest/QtTest>
#include <QtGui/QMainWindow>
#include "mgraphicshighlighteffect.h"
#include "mgraphicshighlighteffect_p.h"

class MApplication;

class Ut_MGraphicsHighlightEffect : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testSetStrength();
    void testSetHighlightColor();

private:
    MApplication *app;
    MGraphicsHighlightEffect *effect;

};

#endif // UT_MGRAPHICSCLIPEFFECT_H
