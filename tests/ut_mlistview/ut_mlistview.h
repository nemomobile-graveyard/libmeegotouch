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

#ifndef UT_MLISTVIEW_H
#define UT_MLISTVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MWidget>

class MListView;

class MDummyTestWidget;

class Ut_MListView : public QObject
{
    Q_OBJECT
public:
    Ut_MListView();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testItemClick();
    void testLongTap();

    void testPrivateMovingDetectionTimerTimeout();
    void testPrivateCellClicked();
    void testPrivateAppendTargetsToDeleteAnimation();

    void testPrivateResetAnimatedWidgets();
    void testPrivateUpdateItemConnections();
    void testPrivateCreateVisibleItems();
    void testPrivateReplaceItem();
    void testPrivateLocateStartScrollToPosition();
    void testPrivateScrollToPos();

    void testPrivateUpdateRecyclerMaxItemsCount();
    void testPrivateLocateLastVisibleIndexInRowAt();
    void testPrivateUpdateItemSize();

    void testPrivateMaxVisibleItemsCount();

    void testIndexToFlatRowAssert();

private:
    MListView *m_subject;

    MDummyTestWidget *m_dummyWidget;
};

class MDummyTestWidget : public MWidget
{
	Q_OBJECT

signals:
	void itemClickSignal();

public:
	void emitClickItem();
};

#endif
