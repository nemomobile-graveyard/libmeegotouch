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

#ifndef UT_MLAYOUT_H
#define UT_MLAYOUT_H

#include <QtTest/QtTest>
#include <QObject>
#include <QPointer>
#include <QList>

#include <mlayout.h>

class QGraphicsScene;
class MWidgetController;
class QGraphicsProxyWidget;
class QPushButton;

class Ut_AllWidgets : public QObject
{
    Q_OBJECT

public:
    Ut_AllWidgets();
    virtual ~Ut_AllWidgets();

private slots:
    void cleanupTestCase();
    void init();
    void cleanup();

    void testAllWidgets_data();
    void testAllWidgets();
    void testSizeHints_data();
    void testSizeHints();
    void testMemoryLeaks_data();
    void testMemoryLeaks();

private:
    void debugDisplayPixmap(const QPixmap &pixmap, int seconds);
    void debugDisplayImage(const QImage &image, int seconds);
    bool paintAndTestWidget(QGraphicsWidget *widget, bool strict);
    bool checkChildrenBoundingRect(const QGraphicsWidget *widget) const;
    QList< QPointer<QGraphicsObject> > getAllLayoutItemObjects(QGraphicsLayoutItem *item);
    QList< QPointer<QGraphicsObject> > getAllSubWidgets(QGraphicsItem *widget);
    QGraphicsScene *m_scene;
};

#endif // Header guard

