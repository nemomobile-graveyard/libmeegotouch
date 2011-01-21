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
 
#include "drilldownlistitem.h"

#include <QGraphicsGridLayout>

#include <MImageWidget>
#include <MLabel>

DrillDownListItem::DrillDownListItem(QGraphicsItem *parent)
    : MBasicListItem(MBasicListItem::SingleTitle, parent)
{

}

DrillDownListItem::~DrillDownListItem()
{

}

QGraphicsLayout *DrillDownListItem::createLayout()
{
    QGraphicsGridLayout *layout = new QGraphicsGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    titleLabelWidget()->setStyleName("CommonSingleTitle");
    layout->addItem(titleLabelWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    imageWidget()->setStyleName("CommonDrillDownIcon");
    imageWidget()->setImage("icon-m-common-drilldown-arrow");
    layout->addItem(imageWidget(), 0, 1, Qt::AlignCenter);

    return layout;
}

void DrillDownListItem::setImageWidget(MImageWidget *imageWidget)
{
    MBasicListItem::setImageWidget(imageWidget);
}

MImageWidget *DrillDownListItem::imageWidget()
{
    return MBasicListItem::imageWidget();
}
