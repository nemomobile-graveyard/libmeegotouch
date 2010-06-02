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

#ifndef QTMAEMO6CLICKLABEL_H
#define QTMAEMO6CLICKLABEL_H

#include <QLabel>
#include <mnamespace.h>


/*!
 * this class extends QLabel to behave more like a button
 */
class QtMaemo6ClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QtMaemo6ClickLabel(QWidget *parent);

    virtual QSize minimumSizeHint () const;
    virtual QSize sizeHint () const;
protected:
    /*! \reimp */
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    /*! \reimp_end */
private:
    QSize getSize(const QSize origSize) const;
public Q_SLOTS:
    void setOrientation(M::OrientationAngle);
Q_SIGNALS:
    void clicked();
protected:
    M::OrientationAngle m_orientationAngle;
};

#endif // QTMAEMO6CLICKLABEL_H
