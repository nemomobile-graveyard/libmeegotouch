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

#include "duibuttongrouplayoutpolicy_p.h"
#include <QGraphicsLayoutItem>
#include <DuiButton>

DuiButtonGroupLayoutPolicy::DuiButtonGroupLayoutPolicy(DuiLayout *layout, Qt::Orientation orientation)
    : DuiLinearLayoutPolicy(layout, orientation)
{
}

DuiButtonGroupLayoutPolicy::~DuiButtonGroupLayoutPolicy()
{ }

void DuiButtonGroupLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item)
{
    DuiLinearLayoutPolicy::insertItem(index, item);

    updateButtonsViewTypes();
}

void DuiButtonGroupLayoutPolicy::removeAt(int index)
{
    DuiLinearLayoutPolicy::removeAt(index);

    updateButtonsViewTypes();
}

void DuiButtonGroupLayoutPolicy::updateButtonsViewTypes()
{
    const int buttonBoxSize = count();

    if (buttonBoxSize == 0) return;

    if (buttonBoxSize == 1) {
        setButtonViewType(0, "single");
    } else {
        int i = 0;
        setButtonViewType(i, "first");
        while (++i < buttonBoxSize - 1) {
            setButtonViewType(i, "middle");
        }
        setButtonViewType(i, "last");
    }
}

void DuiButtonGroupLayoutPolicy::setButtonViewType(int index, const DuiTheme::ViewType &viewType)
{
    DuiButton *button = dynamic_cast<DuiButton *>(itemAt(index));
    if (button) {
        button->setViewType(viewType);
    } else {
        qWarning("Only DuiButton should be added to DuiButtonGroupLayoutPolicy.");
    }
}
