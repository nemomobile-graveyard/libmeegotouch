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

#ifndef PT_MSLIDER_H
#define PT_MSLIDER_H

#include <QtTest/QtTest>
#include <QObject>

class MSlider;
class MWidgetView;

Q_DECLARE_METATYPE(MSlider *);

class Pt_MSlider : public QObject
{
    Q_OBJECT

public:
    void fetchNewRow();
    void createController();
    void createView();
    void createPaintDevice();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    // paint handler performance
    void paintPerformance();
    void paintPerformance_data();

private:
    MSlider *m_subject;
    QPixmap *pixmap;
    QPainter *painter;

    MWidgetView *currentView;

    qint32 orientation;

    QList<QString> written;
};

#endif
