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

#include "qtmaemo6pangesture.h"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QDebug>

void QtMaemo6PanGesture::enableOn(QWidget *w)
{
    if (w) {
        w->installEventFilter(this);
        ScrollState *state = new ScrollState;
        state->widget = w;
        state->panState = ScrollState::None;
        m_activeWidgets.insert(w, state);
    }
}

bool QtMaemo6PanGesture::eventFilter(QObject *object, QEvent *event)
{
    if (!qobject_cast<QWidget *>(object))
        return false;

    QEvent::Type type = event->type();
    if (type != QEvent::MouseButtonPress &&
            type != QEvent::MouseButtonRelease &&
            type != QEvent::MouseMove)
        return false;

    QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    if (!mouseEvent || (mouseEvent->modifiers() != Qt::NoModifier))
        return false;

    QAbstractScrollArea *scrollArea = qobject_cast<QAbstractScrollArea *>(object);
    ScrollState *scrollState = m_activeWidgets.value(scrollArea);
    if (scrollArea && scrollState) {
        //return if the event is on the ignore list
        if (scrollState->ignoreEvents.removeAll(event))
            return false;

        switch (scrollState->panState) {
        case ScrollState::None:
            if (mouseEvent->type() == QEvent::MouseButtonPress
                    && mouseEvent->buttons() == Qt::LeftButton) {
                qDebug() << "Gesture State None Press Event";
                scrollState->pressPos = mouseEvent->pos();
                scrollState->panState = ScrollState::Pressed;
                scrollState->scrollOffset = QPoint(scrollArea->horizontalScrollBar()->value(),
                                                   scrollArea->verticalScrollBar()->value());
            }
            break;
        case ScrollState::Pressed:
            if (mouseEvent->type() == QEvent::MouseButtonRelease) {
                qDebug() << "Gesture State Pressed Release Event";
                //generate new press and release event and
                scrollState->panState = ScrollState::None;

                QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress,
                        scrollState->pressPos, Qt::LeftButton,
                        Qt::LeftButton, Qt::NoModifier);
                QMouseEvent *releaseEvent = new QMouseEvent(*mouseEvent);

                //these generated events must not be handled by this filter again!
                scrollState->ignoreEvents << pressEvent << releaseEvent;

                QApplication::postEvent(object, pressEvent);
                QApplication::postEvent(object, releaseEvent);
            }
            if (mouseEvent->type() == QEvent::MouseMove) {
                qDebug() << "Gesture State Pressed Move Event";
                scrollState->panState = ScrollState::Scroll;
                QPoint delta = mouseEvent->pos() - scrollState->pressPos;

                scrollArea->horizontalScrollBar()->setValue(scrollState->scrollOffset.x() - delta.x());
                scrollArea->verticalScrollBar()->setValue(scrollState->scrollOffset.y() - delta.y());
            }
            break;
        case ScrollState::Scroll:
            if (mouseEvent->type() == QEvent::MouseMove) {
                qDebug() << "Gesture State Scroll Move Event";
                QPoint delta = mouseEvent->pos() - scrollState->pressPos;

                scrollArea->horizontalScrollBar()->setValue(scrollState->scrollOffset.x() - delta.x());
                scrollArea->verticalScrollBar()->setValue(scrollState->scrollOffset.y() - delta.y());

                qDebug() << "Gesture delta" << delta;
                qDebug() << "ScrollBars Value" << scrollArea->horizontalScrollBar()->value() << scrollArea->verticalScrollBar()->value();
            }
            if (mouseEvent->type() == QEvent::MouseButtonRelease) {
                qDebug() << "Gesture State Scroll Release Event";
                scrollState->panState = ScrollState::None;
            }
            break;
        default:
            return false;
        }
        return true;
    } else
        return false;
}
