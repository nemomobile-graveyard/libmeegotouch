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

#ifndef UT_DUITOOLBARVIEW_H
#define UT_DUITOOLBARVIEW_H

#include <QObject>
#include <QtTest/QtTest>

class DuiToolBar;
class DuiToolBarView;
class DuiWidget;
class DuiWidgetAction;

class Ut_DuiToolBarView : public QObject
{
    Q_OBJECT
public:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testDeleteAddAction();
private:
    DuiToolBar *m_toolbar;
    DuiToolBarView *m_subject;

    DuiWidgetAction *createTextEditAction(DuiWidget *parentWidget);
};

#endif

