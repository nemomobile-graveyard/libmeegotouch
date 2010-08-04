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

#ifndef MRESIZELISTENER_H
#define MRESIZELISTENER_H

#include <QObject>

class QGraphicsWidget;

//! \internal

class MResizeListener : public QObject
{
    Q_OBJECT
public:
    MResizeListener(QObject *parent = 0);
    virtual ~MResizeListener();

signals:
    void widgetResized(QGraphicsWidget *widget);

protected:
    virtual bool eventFilter(QObject *, QEvent *event);
};

//! \internal_end

#endif // MRESIZELISTENER_H
