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

#include "mgridlayoutpolicy_p.h"

#include <QtGui/QGraphicsGridLayout>
#include <QtGui/QWidget>
#include <QtGui/QGraphicsWidget>
#include "mlayout.h"
#include "mlayouthelper_p.h"
MGridLayoutPolicyPrivate::MGridLayoutPolicyPrivate(MLayout *l) :
    MAbstractLayoutPolicyPrivate(l), engineWidget(new QGraphicsWidget), engine(new QGraphicsGridLayout(engineWidget))
{
}

MGridLayoutPolicyPrivate::~MGridLayoutPolicyPrivate()
{
    delete engineWidget; //This deletes the engine, which in turn deletes all of its children
}

void MGridLayoutPolicyPrivate::refreshEngine()
{
    engineWidget->setLayoutDirection(layout->layoutDirection());

    //To properly relayout or get the size hint, we need to invalidate the size hints
    //of all the proxy items and set the geometry of the proxy layout (d->engine) to be the same
    for (int i = engine->count() - 1; i >= 0; --i) {
        ProxyItem *item = static_cast<ProxyItem *>(engine->itemAt(i));
        item->refresh();
    }
    qreal left, top, right, bottom;
    layout->getContentsMargins(&left, &top, &right, &bottom);
    engine->setContentsMargins(left, top, right, bottom);

    engine->updateGeometry();
}

void MGridLayoutPolicyPrivate::refreshWidget()
{
    //We need to make engine->geometry() equal the layout->geometry() so that the items are in the right place
    qreal topMargin = layout->geometry().top();
    qreal leftMargin = layout->geometry().left();
    qreal width = layout->geometry().right();
    qreal height = layout->geometry().bottom();
    engineWidget->setContentsMargins(leftMargin, topMargin, 0, 0);
    engineWidget->resize(width, height);
    engine->setGeometry(engine->geometry()); //We need to call this to make sure the layout actually updates contents margins and RTL
}

