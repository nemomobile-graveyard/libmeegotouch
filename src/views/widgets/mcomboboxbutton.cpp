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

#include "mcomboboxbutton.h"

#include "mimagewidget.h"
#include "mlabel.h"
#include "mprogressindicator.h"
#include "mlayout.h"
#include "mgridlayoutpolicy.h"

MComboBoxButton::MComboBoxButton(QGraphicsItem *parent)
    : MListItem(parent),
      icon(0),
      title(0),
      subtitle(0),
      indicator(0),
      progress(0),
      flags(TitleShown)
{
    memset(policy, 0, sizeof(policy));

    setLayout(new MLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);
    updateLayout();
}

void MComboBoxButton::updateLayout()
{
    static_cast<MLayout*>(layout())->setPolicy(layoutPolicy(flags));
}

MGridLayoutPolicy* MComboBoxButton::layoutPolicy(int f)
{
    if (!policy[f]) {
        MLayout* mlayout = static_cast<MLayout*>(layout());
        if ((f&IconShown) && (f&TitleShown) && !(f&ProgressShown)) {
            policy[f] = new MGridLayoutPolicy(mlayout);
            policy[f]->addItem(iconWidget(), 0, 0, 3, 1);
            policy[f]->addItem(titleWidget(), 0, 1);
            policy[f]->addItem(subtitleWidget(), 1, 1);
            policy[f]->addItem(indicatorWidget(), 0, 2, 3, 1, Qt::AlignVCenter);
            policy[f]->addItem(new QGraphicsWidget(this), 2, 1);

        } else if (!(f&IconShown) && (f&TitleShown) && !(f&ProgressShown)) {
            policy[f] = new MGridLayoutPolicy(mlayout);
            policy[f]->addItem(titleWidget(), 0, 0);
            policy[f]->addItem(subtitleWidget(), 1, 0);
            policy[f]->addItem(indicatorWidget(), 0, 1, 3, 1, Qt::AlignVCenter);
            policy[f]->addItem(new QGraphicsWidget(this), 2, 0);

        } else if ((f&IconShown) && (f&TitleShown) && (f&ProgressShown)) {
            policy[f] = new MGridLayoutPolicy(mlayout);
            policy[f]->addItem(iconWidget(), 0, 0, 3, 1);
            policy[f]->addItem(titleWidget(), 0, 1);
            policy[f]->addItem(progressWidget(), 1, 1);
            policy[f]->addItem(indicatorWidget(), 0, 2, 3, 1, Qt::AlignVCenter);
            policy[f]->addItem(new QGraphicsWidget(this), 2, 1);

        } else if (!(f&IconShown) && (f&TitleShown) && (f&ProgressShown)) {
            policy[f] = new MGridLayoutPolicy(mlayout);
            policy[f]->addItem(titleWidget(), 0, 0);
            policy[f]->addItem(progressWidget(), 1, 0);
            policy[f]->addItem(indicatorWidget(), 0, 1, 3, 1, Qt::AlignVCenter);
            policy[f]->addItem(new QGraphicsWidget(this), 2, 0);

        } else if (!(f&TitleShown) && !(f&ProgressShown)) {
            policy[f] = new MGridLayoutPolicy(mlayout);
            policy[f]->addItem(subtitleWidget(), 0, 0, Qt::AlignVCenter);
            policy[f]->addItem(indicatorWidget(), 0, 1, Qt::AlignVCenter);

        } else if (!(f&TitleShown) && (f&ProgressShown)) {
            policy[f] = new MGridLayoutPolicy(mlayout);
            policy[f]->addItem(progressWidget(), 0, 0);
            policy[f]->addItem(indicatorWidget(), 0, 1, 2, 1, Qt::AlignVCenter);
       }
    }
    return policy[f];
}

void MComboBoxButton::setProgressIndicatorVisible(bool visible)
{
    flags = visible ? flags | ProgressShown : flags & ~ProgressShown;
    updateLayout();
}

void MComboBoxButton::setIconVisible(bool visible)
{
    flags = visible ? flags | IconShown : flags & ~IconShown;
    updateLayout();
}

void MComboBoxButton::setTitleVisible(bool visible)
{
    flags = visible ? flags | TitleShown : flags & ~TitleShown;
    updateLayout();
}

void MComboBoxButton::setIconID(const QString& id)
{
    iconImageID = id;
    if (icon)
        icon->setImage(iconImageID);
}

void MComboBoxButton::setTitle(const QString &newTitle)
{
    titleText = newTitle;
    if (title)
        title->setText(titleText);
}

void MComboBoxButton::setSubtitle(const QString &subtitle)
{
    subtitleWidget()->setText(subtitle);
}

void MComboBoxButton::setIconObjectName(const QString& name)
{
    iconObjectName = name;
    if (icon)
        icon->setObjectName(iconObjectName);
}

void MComboBoxButton::setTitleObjectName(const QString& name)
{
    titleObjectName = name;
    if (title)
        title->setObjectName(titleObjectName);
}

void MComboBoxButton::setSubTitleObjectName(const QString& name)
{
    subTitleObjectName = name;
    if (subtitle)
        subtitle->setObjectName(subTitleObjectName);
}

void MComboBoxButton::setProgressIndicatorObjectName(const QString& name)
{
    progressIndicatorObjectName = name;
    if (progress)
        progress->setObjectName(progressIndicatorObjectName);
}

void MComboBoxButton::setIndicatorObjectName(const QString& name)
{
    indicatorObjectName = name;
    if (indicator)
        indicator->setObjectName(indicatorObjectName);
}

void MComboBoxButton::setIndicatorImage(const QString& image)
{
    indicatorImage = image;
    if (indicator)
        indicator->setImage(indicatorImage);
}

MImageWidget * MComboBoxButton::iconWidget()
{
    if (!icon) {
        icon = new MImageWidget(this);
        icon->setImage(iconImageID);
        icon->setObjectName(iconObjectName);
    }
    return icon;
}

MLabel * MComboBoxButton::titleWidget()
{
    if (!title) {
        title = new MLabel(this);
        title->setTextElide(true);
        title->setText(titleText);
        title->setObjectName(titleObjectName);
    }
    return title;
}

MLabel * MComboBoxButton::subtitleWidget()
{
    if (!subtitle) {
        subtitle = new MLabel(this);
        subtitle->setTextElide(true);
        subtitle->setObjectName(subTitleObjectName);
    }
    return subtitle;
}

MProgressIndicator* MComboBoxButton::progressWidget()
{
    if (!progress) {
        progress = new MProgressIndicator(this, MProgressIndicator::barType);
        progress->setUnknownDuration(true);
        progress->setObjectName(progressIndicatorObjectName);
    }
    return progress;
}

MImageWidget* MComboBoxButton::indicatorWidget()
{
    if (!indicator) {
        indicator = new MImageWidget(this);
        indicator->setImage(indicatorImage);
        indicator->setObjectName(indicatorObjectName);
    }
    return indicator;
}
