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

#include "mapplicationmenubutton.h"
#include "mbutton_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MApplicationMenuButton)

MApplicationMenuButton::MApplicationMenuButton(QGraphicsItem *parent, MApplicationMenuButtonModel *model)
    : MButton(new MButtonPrivate, model == NULL ? new MApplicationMenuButtonModel : model, parent)
{
}

MApplicationMenuButton::MApplicationMenuButton(MButtonPrivate *dd, MApplicationMenuButtonModel *model, QGraphicsItem *parent)
    : MButton(dd, model, parent)
{
}

MApplicationMenuButton::~MApplicationMenuButton()
{
}

bool MApplicationMenuButton::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

bool MApplicationMenuButton::isArrowIconVisible() const
{
    return model()->arrowIconVisible();
}

void MApplicationMenuButton::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

void MApplicationMenuButton::setArrowIconVisible(bool visible)
{
    model()->setArrowIconVisible(visible);
}

