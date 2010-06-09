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

#ifndef QTMAEMO6DIALOGPROXY_H
#define QTMAEMO6DIALOGPROXY_H

#include <QGraphicsView>
#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include "qtmaemo6window.h"

class QtMaemo6DialogTitle;

/*!
 * This class emulates the windowdecoration, for dialogs. It draws the the
 * titlebar and cares for the composition modes.
 */
class QtMaemo6DialogProxy : public QtMaemo6Window
{
    Q_OBJECT
public:
    explicit QtMaemo6DialogProxy(QWidget *mw, QWidget *parent);
    ~QtMaemo6DialogProxy();

    /*!
     * sets the title of the dialog
    */
    void setTitle(const QString &text);

    /*!
      * sets the icon shown in the dialog's titlebar
    */
    void setPixmap(const QPixmap &icon);

protected:
    QtMaemo6DialogProxy() {};
    void init();

    /*! \reimp */
    //bool eventFilter(QObject* watched, QEvent* ev);
    //void closeEvent(QCloseEvent* event);
    virtual void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
    /*! \reimp_end */

private:
    QtMaemo6DialogTitle *m_dialogTitle;
};

#endif
