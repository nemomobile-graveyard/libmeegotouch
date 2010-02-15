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

#ifndef QTMAEMO6PANGESTURE_H
#define QTMAEMO6PANGESTURE_H

#include <QObject>
#include <QPoint>
#include <QList>
#include <QMap>

class QWidget;

/*!
 * this class implements a pan gesture recognition which can be enabled on
 * plain scrollareas.
 * this class may be deprecated in future, because dui gestures may be used
 */
class QtMaemo6PanGesture : public QObject
{
    Q_OBJECT
public:
    QtMaemo6PanGesture(QObject *parent) : QObject(parent) {};

    /*!
     * enables the pan gesture recognition onto the given widget
     */
    void enableOn(QWidget *w);

    /*! \reimp */
    /* !
     * the recognition is handled here
     */
    bool eventFilter(QObject *object, QEvent *event);
    /*! \reimp_end */
private:
    /*!
     * this struct holdes the different states when panning is active
     */
    struct ScrollState {
        enum PanState { None, Pressed, Scroll, PostScroll, Stop };
        PanState panState;
        QPoint pressPos;
        QPoint scrollOffset;
        QWidget *widget;
        QList<QEvent *> ignoreEvents;
    };
    QMap<QWidget *, ScrollState *> m_activeWidgets;
};

#endif
