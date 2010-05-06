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

#include "mcheckboxview.h"
#include "mcheckboxview_p.h"

#include "mscalableimage.h"

#include <QPainter>

#include "mdebug.h"

MCheckboxViewPrivate::MCheckboxViewPrivate()
    : MButtonViewPrivate()
{
}

MCheckboxViewPrivate::~MCheckboxViewPrivate()
{
}

MCheckboxView::MCheckboxView(MButton *controller) :
    MButtonView(* new MCheckboxViewPrivate, controller)
{
}

MCheckboxView::~MCheckboxView()
{
}

void MCheckboxView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    if (style()->checkmarkImage() && model()->checked()) {
        QSizeF pos = (size() / 2) - (style()->checkmarkImage()->size() / 2);
        painter->drawPixmap(pos.width(), pos.height(), *style()->checkmarkImage());
    }
}

M_REGISTER_VIEW_NEW(MCheckboxView, MButton)
