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

#include "mseparatorview.h"
#include "mseparatorview_p.h"
#include "mseparator.h"
#include "mtheme.h"
#include "mviewcreator.h"

#include <QDebug>

MSeparatorViewPrivate::MSeparatorViewPrivate() :
    q_ptr(0), controller(0)
{
}

MSeparatorViewPrivate::~MSeparatorViewPrivate()
{
}

MSeparatorView::MSeparatorView(MSeparator *controller) :
    MWidgetView(controller),
    d_ptr(new MSeparatorViewPrivate)
{
    Q_D(MSeparatorView);

    d->q_ptr = this;
    d->controller = controller;
}

MSeparatorView::~MSeparatorView()
{
    delete d_ptr;
}

void MSeparatorView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == MSeparatorModel::Orientation) {
            updateGeometry();
            applyStyle();
        }
    }
    update();
}

void MSeparatorView::applyStyle()
{
    MWidgetView::applyStyle();

    if ( model()->orientation() == Qt::Horizontal ) {
        style().setModeHorizontal();
    } else {
        style().setModeVertical();
    }
    update();
}

QSizeF MSeparatorView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint);
    if (which == Qt::MinimumSize)
        return QSizeF(-1,-1);
    else if (model()->orientation() == Qt::Horizontal)
        return QSizeF(-1, style()->span());
    else
        return QSizeF(style()->span(), -1);
}

M_REGISTER_VIEW_NEW(MSeparatorView, MSeparator)

