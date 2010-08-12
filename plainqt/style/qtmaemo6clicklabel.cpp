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

#include <QPainter>
#include <QPixmap>
#include <qtmaemo6clicklabel.h>
#include <qtmaemo6style.h>

QtMaemo6ClickLabel::QtMaemo6ClickLabel(QWidget *parent)
    : QLabel(parent)
    , m_orientationAngle(M::Angle0)
{
    setMargin(0);
    setIndent(0);
    setContentsMargins(0,0,0,0);
}

QSize QtMaemo6ClickLabel::minimumSizeHint () const {
    return getSize(QLabel::minimumSizeHint());
}

QSize QtMaemo6ClickLabel::sizeHint () const {
    return getSize(QLabel::sizeHint());
}

QSize QtMaemo6ClickLabel::getSize(const QSize origSize) const {
    if(m_orientationAngle == M::Angle0 || m_orientationAngle == M::Angle180)
        return origSize;
    else
        return QSize(origSize.height(), origSize.width());
}

void QtMaemo6ClickLabel::setOrientation(M::OrientationAngle angle) {
    m_orientationAngle = angle;
    updateGeometry();
}

void QtMaemo6ClickLabel::paintEvent(QPaintEvent *ev) {
    Q_UNUSED(ev);

    QPainter p(this);
    QRect bRect = rect();

    QTransform t;
    QPoint center = bRect.center();
    t.translate(center.x(), center.y());
    t.rotate(m_orientationAngle);
    t.translate(-center.x(), -center.y());

    bRect = t.mapRect(bRect);
    p.setTransform(t);
    p.fillRect(bRect, Qt::transparent);
    style()->drawItemText(&p, bRect, Qt::AlignLeft | Qt::AlignVCenter, palette(), true, text(), QPalette::Foreground);
    if(pixmap())
        style()->drawItemPixmap(&p, bRect, Qt::AlignCenter, *pixmap());
}

void QtMaemo6ClickLabel::mousePressEvent(QMouseEvent *ev) {
    Q_UNUSED(ev);
    emit clicked();
}
