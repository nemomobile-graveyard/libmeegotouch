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

MButtonIconView::MButtonIconView(MButton *controller) :
    MButtonView(controller)
{
}

MButtonIconView::~MButtonIconView()
{
}

void MButtonIconView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    MButtonView::drawContents(painter, option);
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
