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

#include "mbuttoniconview.h"
#include "mbuttoniconview_p.h"

#include <QPainter>

#include "mviewcreator.h"
#include "mbutton.h"
#include "mbutton_p.h"
#include "mlabel.h"
#include "mtheme.h"


MButtonIconViewPrivate::MButtonIconViewPrivate()
    : opacity(1.0)
{
}

MButtonIconViewPrivate::~MButtonIconViewPrivate()
{
}

void MButtonIconViewPrivate::updateItemsAfterModeChange()
{
    Q_Q(MButtonIconView);

    MButtonViewPrivate::updateItemsAfterModeChange();

    opacity = q->style()->contentOpacity();
    label->setOpacity(opacity);
}

MButtonIconView::MButtonIconView(MButton *controller) :
    MButtonView(* new MButtonIconViewPrivate, controller)
{
}

MButtonIconView::~MButtonIconView()
{
}

void MButtonIconView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MButtonIconView);

    qreal formerOpacity = painter->opacity();
    painter->setOpacity(d->opacity * d->controller->effectiveOpacity());
    MButtonView::drawContents(painter, option);
    painter->setOpacity(formerOpacity);
}

void MButtonIconView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    MButtonView::drawBackground(painter, option);
}

void MButtonIconView::applyStyle()
{
    MButtonView::applyStyle();
}

void MButtonIconView::updateData(const QList<const char *>& modifications)
{
    MButtonView::updateData(modifications);
}

M_REGISTER_VIEW_NEW(MButtonIconView, MButton)
