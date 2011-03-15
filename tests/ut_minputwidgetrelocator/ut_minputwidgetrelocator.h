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

#ifndef UT_MINPUTWIDGETRELOCATOR_H
#define UT_MINPUTWIDGETRELOCATOR_H

#include <QGraphicsWidget>
#include <QtTest/QtTest>
#include <QObject>
#include "mapplication.h"

class MInputWidgetRelocator;
class MSceneManager;

class QGraphicsScene;
class QGraphicsView;
class InputWidget;
class ScrollableWidget;

class Ut_MInputWidgetRelocator : public QObject
{
    Q_OBJECT
public:
    enum Operation {
        RotationBegin,
        RotationEnd,
        SceneWindowAppearing,
        SceneWindowAppeared,
        SipChange,
        HwKbChange
    };

    typedef QList<Operation> Operations;

    enum Zone {
        UpperNoGoZone,
        LowerNoGoZone,
        AllowedZone
    };

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    void testPostponeAndUpdate_data();
    void testPostponeAndUpdate();
    void testTargetPosition_data();
    void testTargetPosition();
    void testDockBottom_data();
    void testDockBottom();

private:
    QRect allowedZone(const QRect &exposedContentRect, M::Orientation orientation) const;
    qreal verticalAnchorPosition(M::Orientation) const;
    qreal topNoGoMargin(M::Orientation orientation) const;
    qreal bottomNoGoMargin(M::Orientation orientation) const;
    void moveWidgetToZone(InputWidget *widget, Zone zone,
                          const QRect &exposedContentRect, M::Orientation orientation) const;
    void updateStubbedStyleValues(M::Orientation orientation = M::Landscape) const;

private:
    MApplication *app;
    MInputWidgetRelocator *subject;
    MSceneManager *sceneManager;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsWidget *rootElement;
    QPointer<ScrollableWidget> parentScrollableWidget;
    QPointer<ScrollableWidget> childScrollableWidget;
    QPointer<InputWidget> inputWidget;
};

class InputWidget : public QGraphicsWidget
{
public:
    InputWidget(QGraphicsItem *parent = 0)
        :QGraphicsWidget(parent)
    {
        setFlag(QGraphicsItem::ItemIsFocusable, true);
        setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);
        resize(100, 20);
    }

    virtual ~InputWidget()
    {
    }

    QRect microFocusRect() const
    {
        return rect().toRect(); // only height is important at the moment
    }

protected:
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const
    {
        if (query == Qt::ImMicroFocus) {
            return microFocusRect();
        }
        return QGraphicsWidget::inputMethodQuery(query);
    }
};

#endif // UT_MINPUTWIDGETRELOCATOR_H
