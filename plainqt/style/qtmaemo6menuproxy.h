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

#ifndef QTMAEMO6MENUPROXY_H
#define QTMAEMO6MENUPROXY_H

#include <QWidget>

#include "qtmaemo6window.h"

class QtMaemo6Menu;
class QDialog;
class QMenuBar;

/*!
 * This class emulates the windowdecoration, for dialogs. It draws the the
 * titlebar and cares for the composition modes.
 */
class QtMaemo6MenuProxy : public QtMaemo6Window
{
    Q_OBJECT
public:
    explicit QtMaemo6MenuProxy(QMenuBar *menu, QWidget *parent);
    ~QtMaemo6MenuProxy();

protected:
    QtMaemo6MenuProxy() {};
    void hideWindow();
    /*! \reimp */
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void showEvent(QShowEvent *event);
    /*! \reimp_end */
protected:
    QMenuBar *m_menuBar;
    QtMaemo6Menu *m_menu;
    QWidget* m_appArea;
};

#endif
