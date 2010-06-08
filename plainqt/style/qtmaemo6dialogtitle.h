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

#ifndef QTMAEMO6DIALOGTITLE_H
#define QTMAEMO6DIALOGTITLE_H

#include "qtmaemo6clicklabel.h"
#include <QHBoxLayout>


/*!
 * This class emulates the title bar of a dialog
 */
class QtMaemo6DialogTitle : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
public:
    explicit QtMaemo6DialogTitle(QWidget *parent);
    virtual ~QtMaemo6DialogTitle();

    /*!
     * returns the dialog's title
     */
    QString title() const;

public Q_SLOTS:
    /*!
     * sets the title of the dialog
     */
    void setTitle(const QString &title);

    /*!
     * sets the icon shown in the title bar
     */
    void setPixmap(const QPixmap &icon);

Q_SIGNALS:
    /*!
     * this signal is emitted, if the title bar's close button was clicked
     */
    void closeRequest();

protected:
    /*! \reimp */
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *event);
    /*! \reimp_end */
private:
    QtMaemo6ClickLabel *m_closeButton;
    QLabel *m_titleLabel;
    QHBoxLayout *m_titleBarLayout;
};

#endif
