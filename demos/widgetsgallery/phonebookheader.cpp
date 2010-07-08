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

#include "phonebookheader.h"

#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>

PhoneBookHeader::PhoneBookHeader(QGraphicsItem *parent)
    : MStylableWidget(parent),
    layout(NULL),
    groupTitleLabel(NULL),
    groupCountLabel(NULL)
{
}

void PhoneBookHeader::setGroupTitle(const QString &title)
{
    groupTitleLabelWidget()->setText(title);
}

void PhoneBookHeader::setGroupCount(int count)
{
    groupCountLabelWidget()->setText(QString::number(count));
}

MLabel *PhoneBookHeader::groupTitleLabelWidget()
{
    if (!groupTitleLabel) {
        groupTitleLabel = new MLabel(this);
        groupTitleLabel->setAlignment(Qt::AlignLeft);
    }

    return groupTitleLabel;
}

MLabel *PhoneBookHeader::groupCountLabelWidget()
{
    if (!groupCountLabel) {
        groupCountLabel = new MLabel(this);
        groupCountLabel->setAlignment(Qt::AlignRight);
    }

    return groupCountLabel;
}

void PhoneBookHeader::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetController::resizeEvent(event);

    if (layout == NULL)
        setLayout(createLayout());
}

MLayout *PhoneBookHeader::createLayout()
{
    if (layout)
        delete layout;

    layout = new MLayout(this);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);

    policy->addItem(groupTitleLabelWidget());
    policy->addItem(new QGraphicsWidget(this));
    policy->addItem(groupCountLabelWidget());

    layout->setPolicy(policy);

    return layout;
}
