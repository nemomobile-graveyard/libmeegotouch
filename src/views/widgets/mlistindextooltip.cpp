/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
 

#include "mlistindextooltip.h"
#include "mlistindextooltip_p.h"

#include <MLabel>
#include <MSeparator>

#include <QGraphicsAnchorLayout>

MListIndexTooltipPrivate::MListIndexTooltipPrivate()
    : q_ptr(0),
      layout(0),
      titleLabelWidget(0)
{
}

MListIndexTooltipPrivate::~MListIndexTooltipPrivate()
{
}

void MListIndexTooltipPrivate::init()
{
    Q_Q(MListIndexTooltip);

    if (layout)
        return;

    layout = new QGraphicsAnchorLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    MLabel *titleLabelWidget = titleLabel();
    layout->addAnchors(titleLabelWidget, layout);
}

void MListIndexTooltipPrivate::setTitle(const QString &title)
{
    titleLabel()->setText(title);

    qreal leftMargin = 0;
    if (!title.isEmpty()) {
        // Assure that texts with a negative left bearing
        // don't exceed the left border.
        const QFontMetricsF fontMetrics(titleLabel()->font());
        const qreal leftBearing = fontMetrics.leftBearing(title[0]);
        if (leftBearing < 0) {
            leftMargin = -leftBearing;
        }
    }
    layout->setContentsMargins(leftMargin, 0, 0, 0);
    layout->activate();
}

MLabel *MListIndexTooltipPrivate::titleLabel()
{
    if (!titleLabelWidget) {
        Q_Q(MListIndexTooltip);
        titleLabelWidget = new MLabel(q);
        titleLabelWidget->setWordWrap(false);
    }
    return titleLabelWidget;
}

MListIndexTooltip::MListIndexTooltip(QGraphicsItem *parent)
    : MStylableWidget(parent),
    d_ptr(new MListIndexTooltipPrivate)
{
    Q_D(MListIndexTooltip);
    d->q_ptr = this;

    d->init();
}

MListIndexTooltip::~MListIndexTooltip()
{
    delete d_ptr;
}

void MListIndexTooltip::setTitle(const QString &title)
{
    Q_D(MListIndexTooltip);
    d->setTitle(title);
}

#include "moc_mlistindextooltip.cpp"
