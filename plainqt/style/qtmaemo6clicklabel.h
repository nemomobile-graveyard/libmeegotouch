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

#ifndef QTMAEMO6CLICKLABEL_H
#define QTMAEMO6CLICKLABEL_H

#include <QLabel>


/*!
 * this class extends QLabel to behave more like a button
 */
class QtMaemo6ClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QtMaemo6ClickLabel(QWidget *parent): QLabel(parent) {}

protected:
    void mousePressEvent(QMouseEvent *ev) {
        Q_UNUSED(ev);
        emit clicked();
    }

Q_SIGNALS:
    void clicked();
};

#endif // QTMAEMO6CLICKLABEL_H
