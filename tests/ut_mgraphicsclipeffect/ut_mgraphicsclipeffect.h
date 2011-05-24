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
#include "mgraphicsclipeffect.h"
#include "mgraphicsclipeffect_p.h"

#define MARGIN 10

class MApplication;

class Ut_MGraphicsClipEffect : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // class testing
    void testSetDrawRect();

    void testSetClipMargin();

 private:
    MApplication *app;
    MGraphicsClipEffect *clipEffect;

};

#endif // UT_MGRAPHICSCLIPEFFECT_H
