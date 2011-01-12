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

#ifndef PT_MSCALABLEIMAGE_H
#define PT_MSCALABLEIMAGE_H

#include <QtTest/QtTest>
#include <QObject>

class Pt_MScalableImage : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void initTestCase();
    void cleanupTestCase();

    void paintScalablePerformance();
    void paintScalablePerformance_data();

    void paintPOTScalablePerformance();
    void paintPOTScalablePerformance_data();

    void paintHugeScalablePerformance();
    void paintHugeScalablePerformance_data();

    void paintPixmapPerformance();
    void paintPixmapPerformance_data();

    void paintHugePixmapPerformance();
    void paintHugePixmapPerformance_data();

    void paintSmoothPixmapPerformance();
    void paintSmoothPixmapPerformance_data();

    void paintSmoothHugePixmapPerformance();
    void paintSmoothHugePixmapPerformance_data();

    void paintBorderPixmapPerformance();
    void paintBorderPixmapPerformance_data();

};

#endif
