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

#ifndef PT_DUIPROGRESSINDICATOR_H
#define PT_DUIPROGRESSINDICATOR_H

#include <QtTest/QtTest>
#include <QObject>

class DuiProgressIndicator;
class DuiWidgetView;

Q_DECLARE_METATYPE(DuiProgressIndicator *);

class Pt_DuiProgressIndicator : public QObject
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
    DuiProgressIndicator *m_subject;
    QPixmap *pixmap;
    QPainter *painter;

    enum ViewName {
        FirstView = 0,
        Default = 0,
        Bar,
        NoViews
    };

    DuiWidgetView *currentView;

    ViewName currentViewName;
    qint32 width;
    qint32 height;
    qint32 orientation;

    QList<QString> written;
};

#endif
