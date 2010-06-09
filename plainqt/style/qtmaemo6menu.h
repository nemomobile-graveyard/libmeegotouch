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

#ifndef QTMAEMO6MENU_H
#define QTMAEMO6MENU_H

#include <QWidget>
#include <QMenuBar>

/*!
 * This class emulates the title bar of a dialog
 */
class QtMaemo6Menu : public QWidget
{
    Q_OBJECT
public:
    explicit QtMaemo6Menu(QMenuBar *mb, QWidget *parent);
    virtual ~QtMaemo6Menu();
protected Q_SLOTS:
    void showSubMenu();
    void executeAction();
protected:
    /*! \reimp */
    void resizeEvent(QResizeEvent *);
    /*! \reimp_end */
};

#endif
