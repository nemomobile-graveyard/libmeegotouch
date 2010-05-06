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

#include "qtmaemo6kineticscrolling.h"

#include <QAbstractItemView>
#include <QScrollArea>
#include <QApplication>
#include <QList>
#include <QMouseEvent>
#include <QEvent>
#include <QScrollBar>
#include <QDebug>

void QtMaemo6KineticScrolling::KineticData::setState(State state) {
    m_state = state;
}

QtMaemo6KineticScrolling::QtMaemo6KineticScrolling(QObject *parent)
  : QObject(parent),
    m_scrollStartDelay(50),
    m_scrollStartOffset(5),
    m_deaccelerationInterval(20),
    m_deaccelerationStrength(2),
    m_maxKineticScrollSpeed(64),
    m_layoutDirection(Qt::LeftToRight)
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
    m_kineticData[viewport]->scrollArea = scrollArea;
    m_kineticData[viewport]->setState(KineticData::Waiting);

    if(QAbstractItemView* itemView = qobject_cast<QAbstractItemView*>(scrollArea)) {
        //FIXME: public API usage... scrollPerItem prevents the kinetic from working
        // proberly
        itemView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        itemView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    }
    return;
}

static QPoint scrollOffset(QAbstractScrollArea *scrollArea, bool rightToLeft)
{
    if(rightToLeft)
        return QPoint(scrollArea->horizontalScrollBar()->maximum() - scrollArea->horizontalScrollBar()->value(),
                      scrollArea->verticalScrollBar()->value());
    else
        return QPoint(scrollArea->horizontalScrollBar()->value(),
                      scrollArea->verticalScrollBar()->value());
}

static int scrollRange(QScrollBar* scrollBar)
{
    return scrollBar->maximum() - scrollBar->minimum();
}

static bool setScrollOffset(QAbstractScrollArea *scrollArea, const QPoint &p, Qt::LayoutDirection direction)
{
    bool ret = true;
    bool hasHorizontalScrolling = scrollRange(scrollArea->horizontalScrollBar()) > 0;
    bool hasVerticalScrolling = scrollRange(scrollArea->verticalScrollBar()) > 0;

    bool xOffsetInRange = p.x() >= scrollArea->horizontalScrollBar()->minimum()
                          && p.x() <= scrollArea->horizontalScrollBar()->maximum();
    bool yOffsetInRange = p.y() >= scrollArea->verticalScrollBar()->minimum()
                          && p.y() <= scrollArea->verticalScrollBar()->maximum();

    if(hasHorizontalScrolling && !xOffsetInRange)
        ret = false;
    if(hasVerticalScrolling && !yOffsetInRange)
        ret = false;

    //...but scroll at least to the end of the scrollbar anyway
    QPoint realScroll = p;
    if(realScroll.x() < scrollArea->horizontalScrollBar()->minimum())
        realScroll.setX(scrollArea->horizontalScrollBar()->minimum());
    if(realScroll.x() > scrollArea->horizontalScrollBar()->maximum())
        realScroll.setX(scrollArea->horizontalScrollBar()->maximum());
    if(realScroll.y() < scrollArea->verticalScrollBar()->minimum())
        realScroll.setY(scrollArea->verticalScrollBar()->minimum());
    if(realScroll.y() > scrollArea->verticalScrollBar()->maximum())
        realScroll.setY(scrollArea->verticalScrollBar()->maximum());

    if(direction == Qt::RightToLeft)
        scrollArea->horizontalScrollBar()->setValue(scrollArea->horizontalScrollBar()->maximum() - p.x());
    else
        scrollArea->horizontalScrollBar()->setValue(p.x());

    scrollArea->verticalScrollBar()->setValue(p.y());

    return ret;
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

    switch (data->state()) {
    case KineticData::Waiting:
        if (mouseEvent->type() == QEvent::MouseButtonPress)
            if (mouseEvent->buttons() == Qt::LeftButton) {
                consumed = true;
                data->setState(KineticData::Pressed);
                data->pressPos = mouseEvent->pos();
                data->pressedWidget = qobject_cast<QWidget*>(object);
                data->offset = scrollOffset(data->scrollArea, m_layoutDirection);
                if (!m_ticker.isActive())
                    m_ticker.start(m_scrollStartDelay, this);
            }
        break;

    case KineticData::Pressed:
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->setState(KineticData::Waiting);

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
                data->setState(KineticData::Panning);
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
            setScrollOffset(data->scrollArea, data->offset - delta, m_layoutDirection);
        }
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->setState(KineticData::KineticScroll);
            if (!m_ticker.isActive())
                m_ticker.start(m_deaccelerationInterval, this);
        }
        break;

    case KineticData::KineticScroll:
        if (mouseEvent->type() == QEvent::MouseButtonPress) {
            consumed = true;
            data->setState(KineticData::Stop);
            if (!m_ticker.isActive())
                m_ticker.start(m_scrollStartDelay, this);
        }
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->setState(KineticData::Waiting);
            data->speed = QPoint(0, 0);
        }
        break;

    case KineticData::Stop:
        if (mouseEvent->type() == QEvent::MouseButtonRelease) {
            consumed = true;
            data->setState(KineticData::Waiting);
        }
        if (mouseEvent->type() == QEvent::MouseMove) {
            consumed = true;
            data->setState(KineticData::Panning);
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
        if( data->state() == KineticData::Pressed) {
            m_ticker.stop();
            QMouseEvent *newPressEvent = new QMouseEvent(QEvent::MouseButtonPress,
                                                  data->pressPos, Qt::LeftButton,
                                                  Qt::LeftButton, Qt::NoModifier);
            data->ignored << newPressEvent;
            data->setState(KineticData::Waiting);
            QApplication::postEvent(data->pressedWidget, newPressEvent);
        }
        if (data->state() == KineticData::KineticScroll) {
            data->speed = deaccelerate(data->speed, m_deaccelerationStrength, m_maxKineticScrollSpeed);
            QPoint p = scrollOffset(data->scrollArea, m_layoutDirection);
            bool scrollOffsetSet = setScrollOffset(data->scrollArea, p - data->speed, m_layoutDirection);

            if(!scrollOffsetSet && data->speed.manhattanLength() > 0) {
                data->viewportOrigPos = data->scrollArea->viewport()->pos();
                data->setState(KineticData::Bounce);
            } else if(data->speed.manhattanLength() == 0) {
                data->setState(KineticData::Waiting);
                m_ticker.stop();
            }
        }
        if(data->state() == KineticData::Bounce) {
            QPoint speed = data->speed;
            //only bounce on axis that have a valid scrollrange
            if(scrollRange(data->scrollArea->horizontalScrollBar()) == 0)
                speed.setX(0);
            if(scrollRange(data->scrollArea->verticalScrollBar()) == 0)
                speed.setY(0);
            data->scrollArea->viewport()->move(data->scrollArea->viewport()->pos() + speed);

            //deaccelerate here by halveing the speed in each step, but care for the speed to stay bigger than 0
            int deaccelerationStrength = qMax(qAbs(data->speed.x()), qAbs(data->speed.y())) / 2;
            if( deaccelerationStrength == 0)
                deaccelerationStrength = 1;

            data->speed = deaccelerate(data->speed, deaccelerationStrength, m_maxKineticScrollSpeed);
            if(data->speed == QPoint(0,0)) {
                data->setState(KineticData::BounceBack);
            }
        }
        if (data->state() == KineticData::BounceBack) {
            QPoint pos = data->scrollArea->viewport()->pos();
            //bounce back to orignal position, this is required if e.g. headers are enabled
            pos -= data->viewportOrigPos;
            QPoint bounceStep;

            //limit the last bounceStep to, to bounce back to original position
            int stepX = 10;
            if(qAbs(pos.x()) - stepX < 0)
                stepX = qAbs(pos.x());
            int stepY = 10;
            if(qAbs(pos.y()) - stepY < 0)
                stepY = qAbs(pos.y());

            //invert the step if bouncing positions are negative
            if(pos.x() < 0)
                stepX = -stepX;
            if(pos.y() < 0)
                stepY = -stepY;

            bounceStep = QPoint(stepX, stepY);

            data->scrollArea->viewport()->move(data->scrollArea->viewport()->pos() - bounceStep);
            if(bounceStep.manhattanLength() == 0) {
                data->setState(KineticData::Waiting);
                m_ticker.stop();
            }
        }
        if (data->state() == KineticData::Stop) {
            data->setState(KineticData::Waiting);
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
