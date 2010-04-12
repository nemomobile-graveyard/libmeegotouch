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

#ifndef MT_MBUTTON_H
#define MT_MBUTTON_H

#include <QtTest/QtTest>
#include <QObject>

class Mt_MButton : public QObject
{
    Q_OBJECT

private:
    void initSchema();
    void memoryBenchmark(qint32 width, qint32 height, const QString &view, const QString &icon1, const QString &icon2, const QString &text);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void memoryMButtonView();
    void memoryMButtonView_data();

    void memoryMButtonDefaultView();
    void memoryMButtonDefaultView_data();

    void memoryMButtonIconView();
    void memoryMButtonIconView_data();

    void memoryMButtonViewTimeline();
    void memoryMButtonViewDefaultViewTimeline();
    void memoryMButtonViewIconViewTimeline();
};

#endif

