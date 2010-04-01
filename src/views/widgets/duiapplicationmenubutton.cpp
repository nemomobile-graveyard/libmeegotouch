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

#include "duiapplicationmenubutton.h"
#include "duibutton_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiApplicationMenuButton)

DuiApplicationMenuButton::DuiApplicationMenuButton(QGraphicsItem *parent, DuiApplicationMenuButtonModel *model)
    : DuiButton(new DuiButtonPrivate, model == NULL ? new DuiApplicationMenuButtonModel : model, parent)
{
}

DuiApplicationMenuButton::DuiApplicationMenuButton(DuiButtonPrivate *dd, DuiApplicationMenuButtonModel *model, QGraphicsItem *parent)
    : DuiButton(dd, model, parent)
{
}

DuiApplicationMenuButton::~DuiApplicationMenuButton()
{
}

bool DuiApplicationMenuButton::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

bool DuiApplicationMenuButton::isArrowIconVisible() const
{
    return model()->arrowIconVisible();
}

void DuiApplicationMenuButton::setProgressIndicatorVisible(bool visible)
{
    model()->setProgressIndicatorVisible(visible);
}

void DuiApplicationMenuButton::setArrowIconVisible(bool visible)
{
    model()->setArrowIconVisible(visible);
}

