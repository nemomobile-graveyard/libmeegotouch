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

#include "duilinearlayoutpolicy_p.h"

#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QWidget>

#include "duilayout.h"
#include "duilayouthelper_p.h"
DuiLinearLayoutPolicyPrivate::DuiLinearLayoutPolicyPrivate(DuiLayout *layout, Qt::Orientation orientation) :
    DuiAbstractLayoutPolicyPrivate(layout), engineWidget(new QGraphicsWidget), engine(new QGraphicsLinearLayout(orientation, engineWidget))
{
    rowCount = 0;
    engineWidget->setContentsMargins(0, 0, 0, 0);
    engineWidget->setMinimumSize(0, 0);
    engineWidget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

DuiLinearLayoutPolicyPrivate::~DuiLinearLayoutPolicyPrivate()
{
    delete engineWidget; //This deletes the engine, which in turn deletes all of its children
}

void DuiLinearLayoutPolicyPrivate::fixIndex(int *index) const
{
    if (uint(*index) >= uint(rowCount))
        *index = rowCount;
}

void DuiLinearLayoutPolicyPrivate::refreshEngine()
{
    engineWidget->setLayoutDirection(layout->layoutDirection());   //Make sure that we have our RTL/LTR correct

    engine->invalidate();
    for (int i = engine->count() - 1; i >= 0; --i) {
        ProxyItem *item = static_cast<ProxyItem *>(engine->itemAt(i));
        item->refresh();
    }
    qreal left, top, right, bottom;
    layout->getContentsMargins(&left, &top, &right, &bottom);
    engine->setContentsMargins(left, top, right, bottom);

    engine->updateGeometry();

    //We need to make engine->geometry() equal the layout->geometry() so that the items are in the right place
    qreal topMargin = layout->geometry().top();
    qreal leftMargin = layout->geometry().left();
    qreal width = layout->geometry().right();
    qreal height = layout->geometry().bottom();
    engineWidget->setContentsMargins(leftMargin, topMargin, 0, 0);
    engineWidget->resize(width, height);
}

