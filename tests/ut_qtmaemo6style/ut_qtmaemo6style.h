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

#ifndef UT_QTMAEMO6STYLE_H
#define UT_QTMAEMO6STYLE_H

#include <QtTest/QtTest>
#include <QObject>

class QMainWindow;

// the real unit/QtMaemo6Style class declaration
#include <qtmaemo6style.h>

Q_DECLARE_METATYPE(QtMaemo6Style *);

class Ut_QtMaemo6Style : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testStyleLoadedByDefault();
    void testLoadOtherStyle();

    void testWindowDecoration();
    void testWindowDecorationSlot();

    void testFonts();

private:
    QApplication* m_app;
    QMainWindow* m_mw;
};

#endif
