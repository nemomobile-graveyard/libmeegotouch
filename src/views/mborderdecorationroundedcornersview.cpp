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

#include "mborderdecorationroundedcornersview.h"
#include <mborderdecoration.h>
#include "mscenewindowview_p.h"
#include <mviewcreator.h>

#include <QGraphicsAnchorLayout>

MRoundedCorner::MRoundedCorner(QGraphicsItem *parent) : MStylableWidget(parent)
{
}

MRoundedCorner::~MRoundedCorner()
{
}

MBorderDecorationRoundedCornersView::MBorderDecorationRoundedCornersView(MBorderDecoration *controller)
    : MSceneWindowView(*(new MSceneWindowViewPrivate), controller)
{
    controller->setFlag(QGraphicsItem::ItemHasNoContents);

    QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout(controller);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    leftCornerWidget = new MRoundedCorner(controller);
    leftCornerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addCornerAnchors(leftCornerWidget, Qt::TopLeftCorner, layout, Qt::TopLeftCorner);

    rightCornerWidget = new MRoundedCorner(controller);
    rightCornerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addCornerAnchors(rightCornerWidget, Qt::TopRightCorner, layout, Qt::TopRightCorner);

    // we need to lock the layout direction to LeftToRight here,
    // because otherwise we will have the wrong rounded corners
    // on the left and right sides of the window.
    controller->setLayoutDirection(Qt::LeftToRight);
}

MBorderDecorationRoundedCornersView::~MBorderDecorationRoundedCornersView()
{
    delete leftCornerWidget;
    leftCornerWidget = 0;

    delete rightCornerWidget;
    rightCornerWidget = 0;
}

void MBorderDecorationRoundedCornersView::setupModel()
{
}

void MBorderDecorationRoundedCornersView::updateData(const QList<const char *> &modifications)
{
    Q_UNUSED(modifications)
}

void MBorderDecorationRoundedCornersView::applyStyle()
{
    MSceneWindowView::applyStyle();
    leftCornerWidget->setStyleName(style()->leftCornerWidgetStyleName());
    rightCornerWidget->setStyleName(style()->rightCornerWidgetStyleName());
}

M_REGISTER_VIEW_NEW(MBorderDecorationRoundedCornersView, MBorderDecoration)
