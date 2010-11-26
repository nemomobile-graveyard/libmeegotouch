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

#include "mbuttongrouplayoutpolicy_p.h"
#include <QGraphicsLayoutItem>
#include <MButton>
#include <MLayout>
#include <MApplication>
#include <MDeviceProfile>

MButtonGroupLayoutPolicy::MButtonGroupLayoutPolicy(MLayout *layout, Qt::Orientation orientation)
    : MLinearLayoutPolicy(layout, orientation),
    buttonWidth(0)
{
}

MButtonGroupLayoutPolicy::~MButtonGroupLayoutPolicy()
{ }

void MButtonGroupLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item)
{
    MLinearLayoutPolicy::insertItem(index, item);

    updateButtonsViewTypes();
}

void MButtonGroupLayoutPolicy::removeAt(int index)
{
    MLinearLayoutPolicy::removeAt(index);

    updateButtonsViewTypes();
}

void MButtonGroupLayoutPolicy::updateButtonsViewTypes()
{
    const int buttonBoxSize = count();

    if (buttonBoxSize == 0) return;

    if (buttonBoxSize == 1) {
        setButtonViewType(0, "single");
    } else {
        int i = 0;
        if (layout()->layoutDirection() == Qt::RightToLeft && MApplication::activeWindow()->orientation() == M::Landscape) {
            setButtonViewType(i, "last");
        } else {
            setButtonViewType(i, "first");
        }
        while (++i < buttonBoxSize - 1) {
            setButtonViewType(i, "middle");
        }
        if (layout()->layoutDirection() == Qt::RightToLeft && MApplication::activeWindow()->orientation() == M::Landscape) {
            setButtonViewType(i, "first");
        }else{
            setButtonViewType(i, "last");
        }
    }
}

void MButtonGroupLayoutPolicy::setButtonViewType(int index, const MTheme::ViewType &viewType)
{
    MButton *button = dynamic_cast<MButton *>(itemAt(index));
    if (button) {
        button->setViewType(viewType);
        if (buttonWidth>0){
            button->setPreferredWidth(buttonWidth);
            button->setMaximumWidth(buttonWidth);
            button->setMinimumWidth(buttonWidth);
        } else {
            button->setMinimumWidth(-1);
            button->setMaximumWidth(qMax(MDeviceProfile::instance()->resolution().width(),
                                         MDeviceProfile::instance()->resolution().height()));
            button->setPreferredWidth(-1);

        }
    } else {
        qWarning("Only MButton should be added to MButtonGroupLayoutPolicy.");
    }
}

void MButtonGroupLayoutPolicy::setButtonWidth(qreal width){
    buttonWidth = width;
    updateButtonsViewTypes();
}
