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

#ifndef QTMAEMO6WINDOWDECORATION_H
#define QTMAEMO6WINDOWDECORATION_H

#include <QGraphicsView>
#include <QWidget>
#include <QScrollArea>
#include <QPointer>

#include "qtmaemo6window.h"

class QStatusBar;
class QtMaemo6TitleBar;
class QVBoxLayout;
class QMenuBar;

/*!
 * this class emulates the m windowdecoration
 * it adds a titlebar and places an statusbar on the bottom of the screen,
 * if available
 */
class QtMaemo6WindowDecoration : public QtMaemo6Window
{
    Q_OBJECT
public:
    explicit QtMaemo6WindowDecoration(QWidget *mw, QWidget *parent = NULL);
    ~QtMaemo6WindowDecoration();

    /*!
     * sets the given statusbar as the applications status bar
     * this one stays on the bottom of the screen
     * it takes the ownership of the menuBar
     */
    void setStatusBar(QStatusBar *statusBar);

    /*!
     * sets the menuBar for this windowDecoration
     * it takes the ownership of the menuBar
     */
    void setMenuBar(QMenuBar *menuBar);
protected Q_SLOTS:
    void showMenuBar();
protected:
    QtMaemo6WindowDecoration() {};

    /*! \reimp */
    bool eventFilter(QObject *obj, QEvent *event);
    /*! \reimp_end */
private:
    QtMaemo6TitleBar *m_titleBar;
    QMenuBar *m_menuBar;
    QStatusBar *m_statusBar;
    QWidget *m_statusBarParent;
};

#endif
