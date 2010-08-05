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

#ifndef UT_MWIDGET_H
#define UT_MWIDGET_H

#include <QtTest/QtTest>
#include <QObject>
#include <QPointer>
#include "mwidget.h"

Q_DECLARE_METATYPE(QPointF);

#define MAX_PARAMS 10
class Ut_MWidget : public QObject
{
    Q_OBJECT

private:
    QPointer<MWidget> widget;
    bool m_dummySlotCalled;

private slots:
    void init();
    void cleanup();

    void testShape_data();
    void testShape();
    void testClearActions();

    void testIsOnDisplay_onView();
    void testIsOnDisplay_offView();
    void testIsOnDisplay_widgetHidden();

    void testEnteredDisplay();
    void testExitedDisplay();

    void testShowHideSimple();
    void testShowHidePropagation();

protected slots:
    void dummySlot();
};

#endif // UT_MWIDGET_H

