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

#ifndef PT_MLABEL_H
#define PT_MLABEL_H

#include <QtTest/QtTest>
#include <QObject>

class MLabel;
class MWidgetView;

class Pt_MLabel : public QObject
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

    void multiplePaintPerformance();
    void multiplePaintPerformance_data();

    void sizeHint();
    void sizeHint_data();
    
private:
    void data();
    
    MLabel *subject;
    MWidgetView *currentView;

    QPixmap *pixmap;
    QPainter *painter;
    qint32 width;
    qint32 height;
    QList<QString> written;
};

#endif
