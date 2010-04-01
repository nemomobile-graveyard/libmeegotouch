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

#include "duiseparatorview.h"
#include "duiseparatorview_p.h"
#include "duiseparator.h"
#include "duitheme.h"
#include "duiviewcreator.h"

#include <QDebug>

DuiSeparatorViewPrivate::DuiSeparatorViewPrivate() :
    q_ptr(0), controller(0)
{
}

DuiSeparatorViewPrivate::~DuiSeparatorViewPrivate()
{
}

DuiSeparatorView::DuiSeparatorView(DuiSeparator *controller) :
    DuiWidgetView(controller),
    d_ptr(new DuiSeparatorViewPrivate)
{
    Q_D(DuiSeparatorView);

    d->q_ptr = this;
    d->controller = controller;
}

DuiSeparatorView::~DuiSeparatorView()
{
    delete d_ptr;
}

void DuiSeparatorView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == DuiSeparatorModel::Orientation) {
            updateGeometry();
        }
    }
    update();
}

QSizeF DuiSeparatorView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QSizeF s = DuiWidgetView::sizeHint(which, constraint);

    if (which == Qt::PreferredSize || which == Qt::MaximumSize) {
        if (model()->orientation() == Qt::Horizontal)
            s.setHeight(style()->span());
        else
            s.setWidth(style()->span());
    }

    return s;
}

DUI_REGISTER_VIEW_NEW(DuiSeparatorView, DuiSeparator)

