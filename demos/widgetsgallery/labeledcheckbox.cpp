/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "labeledcheckbox.h"
#include <QGraphicsLinearLayout>
#include <MApplication>

LabeledCheckbox::LabeledCheckbox(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags)
{
    setFlag(QGraphicsItem::ItemHasNoContents);

    button = new MButton(this);
    button->setObjectName("button");
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        button->setStyleName("CommonLeftCheckBox");
    } else {
        button->setStyleName("CommonLeftCheckBoxInverted");
    }
    button->setViewType(MButton::checkboxType);
    button->setCheckable(true);

    label = new MLabel(this);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    label->setObjectName("label");
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        label->setStyleName("CommonSingleTitle");
    } else {
        label->setStyleName("CommonSingleTitleInverted");
    }
    label->setWordWrap(true);
    label->setTextElide(true);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    layout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addItem(button);
    layout->addItem(label);
    layout->setAlignment(button, Qt::AlignCenter);
    layout->setAlignment(label, Qt::AlignCenter);
}

LabeledCheckbox::~LabeledCheckbox()
{
}
