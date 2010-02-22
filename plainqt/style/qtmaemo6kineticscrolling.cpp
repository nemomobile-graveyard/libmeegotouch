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

#include "qtmaemo6kineticscrolling.h"

#include <QAbstractItemView>
#include <QAbstractScrollArea>
#include <QApplication>
#include <QList>
#include <QMouseEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>

QtMaemo6KineticScrolling::QtMaemo6KineticScrolling(QObject *parent)
  : QObject(parent),
    m_scrollStartDelay(50),
    m_scrollStartOffset(5),
    m_deaccelerationInterval(20),
    m_deaccelerationStrength(2),
    m_maxKineticScrollSpeed(64)
{
}

QtMaemo6KineticScrolling::~QtMaemo6KineticScrolling()
{
}

void QtMaemo6KineticScrolling::enableOn(QAbstractScrollArea *scrollArea)
{
    QWidget *viewport = scrollArea->viewport();
    viewport->installEventFilter(this);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->installEventFilter(this);

    m_kineticData.remove(viewport);
    m_kineticData[viewport] = new KineticData;
    m_kineticData[viewport]->widget = scrollArea;
    m_kineticData[viewport]->state = KineticData::Waiting;

    if(QAbstractItemView* itemView = qobject_cast<QAbstractItemView*>(scrollArea)) {
        //FIXME: public API usage... scrollPerItem prevents the kinetic from working
        // proberly
        itemView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        itemView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    }
    return;
}

static QPoint scrollOffset(QWidget *widget)
{
    QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea*>(widget);
    if (scrollArea) {
        return QPoint(scrollArea->horizontalScrollBar()->value(),
                      scrollArea->verticalScrollBar()->value());
    }
    return QPoint();
}

static void setScrollOffset(QWidget *widget, const QPoint &p)
{
    QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea*>(widget);
    if (scrollArea) {
        scrollArea->horizontalScrollBar()->setValue(p.x());
        scrollArea->verticalScrollBar()->setValue(p.y());
    }
}

static QPoint deaccelerate(const QPoint &speed, int a, int max)
{
    int x = qBound(-max, speed.x(), max);
    int y = qBound(-max, speed.y(), max);
    if(x > 0)
        x = qMax(0, x - a);
    else
        x = qMin(0, x + a);

    if(y > 0)
        y = qMax(0, y - a);
    else
        y = qMin(0, y + a);
    //x = (x == 0) ? x : (x > 0) ? qMax(0, x - a) : qMin(0, x + a);
    //y = (y == 0) ? y : (y > 0) ? qMax(0, y - a) : qMin(0, y + a);
    return QPoint(x, y);
}

bool QtMaemo6KineticScrolling::eventFilter(QObject *object, QEvent *event)
{
    if (!object->isWidgetType())
        return false;

    QEvent::Type type = event->type();
    if (type != QEvent::MouseButtonPress &&
        type != QEvent::MouseButtonRelease &&
        type != QEvent::MouseMove &&
        type != QEvent::Show)
        return false;

    QWidget *viewport = qobject_cast<QWidget*>(object);
    //if object is not the viewport itself, search if one of the parents is the viewport
    if(!qobject_cast<QAbstractScrollArea*>(viewport->parentWidget())) {
        QWidget* parentViewPort = viewport->parentWidget();
        QAbstractScrollArea* scrollArea = NULL;
        while ( parentViewPort ) {
            if((scrollArea = qobject_cast<QAbstractScrollArea*>(parentViewPort))){
                viewport = scrollArea->viewport();
                break;
            } else
                parentViewPort = parentViewPort->parentWidget();
        }
    }
    KineticData *data = m_kineticData.value(viewport);

    if(viewport && data && type == QEvent::Show) {
        //we have the viewport here
        if(data) {
            installEventFilter(viewport->children());
        }
    }

    //skip if the event is on ignoreList
    if (!viewport || !data || data->ignored.removeAll(event))
        return false;

    QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
    if (!mouseEvent || mouseEvent->modifiers() != Qt::NoModifier)
        return false;

    bool consumed = false;

    switch (data->state) {
    case KineticData::Waiting:
        if (mouseEvent->type() == QEvent::MouseButtonPress)
            if (mouseEvent->buttons() == Qt::LeftButton) {
                consumed = true;
                data->state = KineticData::Pressed;
                data->pressPos = mouseEvent->pos();
                data->pressedWidget = qobject_cast<QWidget*>(object);
                data->offset = scrollOffset(data->widget);
                if (!m_ticker.isActive())
                    m_ticker.start(m_scrollStartDelay, this);
            }
        break;

    case KineticData::Pressed:
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->state = KineticData::Waiting;

            QMouseEvent *event1 = new QMouseEvent(QEvent::MouseButtonPress,
                                                  data->pressPos, Qt::LeftButton,
                                                  Qt::LeftButton, Qt::NoModifier);
            QMouseEvent *event2 = new QMouseEvent(*mouseEvent);

            data->ignored << event1;
            data->ignored << event2;
            QApplication::postEvent(object, event1);
            QApplication::postEvent(object, event2);
        }
        if (mouseEvent->type() == QEvent::MouseMove) {
            consumed = true;
            QPoint offset = mouseEvent->pos() - data->pressPos;
            if(offset.manhattanLength() > m_scrollStartOffset) {
                data->state = KineticData::Panning;
                data->dragPos = QCursor::pos();
                if(m_ticker.isActive())
                    m_ticker.stop();
            }
        }
        break;

    case KineticData::Panning:
        if (mouseEvent->type() == QEvent::MouseMove) {
            consumed = true;
            QPoint delta = mouseEvent->pos() - data->pressPos;
            //it occurs, that the last move event before release is a (0,0)
            // move. In this case keep the last speed setting.
            QPoint speed = QCursor::pos() - data->dragPos;
            if(speed != QPoint(0,0))
                data->speed = speed;
            data->dragPos = QCursor::pos();
            setScrollOffset(data->widget, data->offset - delta);
        }
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->state = KineticData::KineticScroll;
            if (!m_ticker.isActive())
                m_ticker.start(m_deaccelerationInterval, this);
        }
        break;

    case KineticData::KineticScroll:
        if (mouseEvent->type() == QEvent::MouseButtonPress) {
            consumed = true;
            data->state = KineticData::Stop;
            if (!m_ticker.isActive())
                m_ticker.start(m_scrollStartDelay, this);
        }
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->state = KineticData::Waiting;
            data->speed = QPoint(0, 0);
        }
        break;

    case KineticData::Stop:
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->state = KineticData::Waiting;
        }
        if (mouseEvent->type() == QEvent::MouseMove) {
            consumed = true;
            data->state = KineticData::Panning;
            data->dragPos = QCursor::pos();
            if (!m_ticker.isActive())
                m_ticker.start(m_deaccelerationInterval, this);
        }
        break;

    default:
        break;
    }

    return consumed;
}

void QtMaemo6KineticScrolling::timerEvent(QTimerEvent *event)
{
    QHashIterator<QWidget*, KineticData*> item(m_kineticData);
    while (item.hasNext()) {
        item.next();
        KineticData *data = item.value();
        if( data->state == KineticData::Pressed) {
            m_ticker.stop();
            QMouseEvent *newPressEvent = new QMouseEvent(QEvent::MouseButtonPress,
                                                  data->pressPos, Qt::LeftButton,
                                                  Qt::LeftButton, Qt::NoModifier);
            data->ignored << newPressEvent;
            data->state = KineticData::Waiting;
            QApplication::postEvent(data->pressedWidget, newPressEvent);
        }
        if (data->state == KineticData::KineticScroll) {
            data->speed = deaccelerate(data->speed, m_deaccelerationStrength, m_maxKineticScrollSpeed);
            QPoint p = scrollOffset(data->widget);
            setScrollOffset(data->widget, p - data->speed);
            if (data->speed == QPoint(0, 0)) {
                data->state = KineticData::Waiting;
                m_ticker.stop();
            }
        }
        if (data->state == KineticData::Stop) {
            data->state = KineticData::Waiting;
            data->speed = QPoint(0,0);
            m_ticker.stop();
        }
    }
    QObject::timerEvent(event);
}

void QtMaemo6KineticScrolling::installEventFilter(QObjectList list) {
    foreach(QObject* obj, list) {
        QWidget* w = qobject_cast<QWidget*>(obj);
        if(w) {
            w->installEventFilter(this);
            installEventFilter(w->children());
        }
    }
}
