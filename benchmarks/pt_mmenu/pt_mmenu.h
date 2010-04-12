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

#ifndef PT_MMENU_H
#define PT_MMENU_H

#include <QtTest/QtTest>
#include <QObject>

class MApplicationMenu;
class MWidgetView;

class Pt_MMenu : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    // paint handler performance

    void paintPerformance();
    void paintPerformance_data();

private:
    MApplicationMenu *m_subject;
    MWidgetView *view;

    QPixmap *pixmap;
    QPainter *painter;

    QString type;
};

#endif
