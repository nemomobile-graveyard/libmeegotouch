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

#include "mlistheader.h"
#include "mlistheader_p.h"

#include <MLabel>
#include <MSeparator>

#include <QGraphicsLinearLayout>

MListHeaderPrivate::MListHeaderPrivate()
    : q_ptr(0),
    layout(0),
    titleLabel(0),
    separator(0)
{
}

MListHeaderPrivate::~MListHeaderPrivate()
{
}

void MListHeaderPrivate::init()
{
    Q_Q(MListHeader);
    if (!layout) {
        layout = new QGraphicsLinearLayout(Qt::Horizontal, q);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addItem(separatorWidget());
        layout->addItem(titleLabelWidget());
    }
}

MLabel *MListHeaderPrivate::titleLabelWidget()
{
    Q_Q(MListHeader);
    if (!titleLabel) {
        titleLabel = new MLabel(q);
        titleLabel->setTextElide(true);
        titleLabel->setStyleName("CommonGroupHeader");
    }
    return titleLabel;
}

MSeparator *MListHeaderPrivate::separatorWidget()
{
    Q_Q(MListHeader);
    if (!separator) {
        separator = new MSeparator(q);
        separator->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        separator->setStyleName("CommonGroupHeaderDivider");
    }
    return separator;
}

MListHeader::MListHeader(QGraphicsItem *parent)
    : MStylableWidget(parent),
    d_ptr(new MListHeaderPrivate)
{
    Q_D(MListHeader);
    d->q_ptr = this;
    d->init();
}

MListHeader::~MListHeader()
{
    delete d_ptr;
}

void MListHeader::setText(const QString &text)
{
    Q_D(MListHeader);
    d->titleLabelWidget()->setText(text);
}

QString MListHeader::text() const
{
    Q_D(const MListHeader);
    return const_cast<MListHeaderPrivate*>(d)->titleLabelWidget()->text();
}

void MListHeader::applyStyle()
{
    Q_D(MListHeader);
    d->titleLabelWidget()->setStyleName(style()->titleStyleName());
    d->separatorWidget()->setStyleName(style()->separatorStyleName());
}
