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

#ifndef DUILAYOUTHELPER_H
#define DUILAYOUTHELPER_H

#include <QtGui/QGraphicsLayout>
#include <QtGui/QGraphicsItem>

//! \internal

class ProxyItem : public QGraphicsLayoutItem
{
public:
    ProxyItem(QGraphicsLayoutItem *proxiedItem) {
        item = proxiedItem;
        setOwnedByLayout(true);
    }
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const {
        return item->effectiveSizeHint(which, constraint);
    }
    virtual void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const {
        item->getContentsMargins(left, top, right, bottom);
    }
    QGraphicsLayoutItem *proxiedItem() const {
        return item;
    }
    void refresh() {
        updateGeometry();
        setSizePolicy(item->sizePolicy());
    }
private:
    QGraphicsLayoutItem *item;
};

//! \internal_end

#endif // Header Guard
