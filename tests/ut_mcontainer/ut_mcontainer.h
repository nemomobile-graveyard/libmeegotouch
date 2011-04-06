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

#ifndef UT_MCONTAINER_H
#define UT_MCONTAINER_H

#include <QtTest/QtTest>
#include <QObject>

#include <mcontainer.h>

Q_DECLARE_METATYPE(MContainer *);

class Ut_MContainer : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void centralWidget();
    void setCentralWidget();
    void replaceCentralWidget();
    void replaceCentralWidgetWithoutDestroying();

    void title();
    void text();
    void iconID();
    void headerVisible();
    void toggleHeaderVisible();
    void testCreatingViewImmediatelyInLayout();
    void testCreatingViewImmediately();
    void testCancelEvent();
    void testSurviveCentralWidgetDoubleDelete();
    void testSurviveCentralWidgetReplace();
    void toggleProgressIndicatorVisible();

    void setTitleWithHeaderVisible();
    void setTitleWithHeaderInvisible();

    void setIconIDWithHeaderVisible();
    void setIconIDWithHeaderInvisible();

    void setTextWithHeaderVisible();
    void setTextWithHeaderInvisible();

    void setProgressIndicatorVisible();
    void setProgressIndicatorInVisible();

    void setProgressIndicatorWithHeaderVisible();
    void setProgressIndicatorWithHeaderInvisible();

private:
    MContainer *m_subject;
};

#endif
