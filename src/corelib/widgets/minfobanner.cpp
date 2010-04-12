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

#include "minfobanner.h"
#include "minfobanner_p.h"
#include <mdebug.h>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MInfoBanner)

const MTheme::ViewType MInfoBanner::informationType  = "information";
const MTheme::ViewType MInfoBanner::eventType        = "event";

MInfoBanner::MInfoBanner(BannerType type) :
    MSceneWindow(new MInfoBannerPrivate, new MInfoBannerModel,
                   (type == Information) ? MSceneWindow::NotificationInformation : MSceneWindow::NotificationEvent,
                   (type == Information) ? informationType : eventType)
{
    Q_D(MInfoBanner);

    d->bannerType = type;
}

MInfoBanner::~MInfoBanner()
{
}

void MInfoBanner::setImageID(const QString &image)
{
    model()->setImageID(image);
}

QString MInfoBanner::imageID() const
{
    return model()->imageID();
}

void MInfoBanner::setPixmap(const QPixmap &pixmap)
{
    model()->setPixmap(pixmap);
}

QPixmap MInfoBanner::pixmap() const
{
    return model()->pixmap();
}

void MInfoBanner::setBodyText(const QString &text)
{
    model()->setBodyText(text);
}

QString MInfoBanner::bodyText() const
{
    return model()->bodyText();
}

void MInfoBanner::setIconID(const QString &iconId)
{
    model()->setIconID(iconId);
}

QString MInfoBanner::iconID() const
{
    return model()->iconID();
}

MInfoBanner::BannerType MInfoBanner::bannerType()
{
    Q_D(MInfoBanner);

    return d->bannerType;
}

void MInfoBanner::setButtonText(const QString &text)
{
    model()->setButtonText(text);
}

QString MInfoBanner::buttonText() const
{
    return model()->buttonText();
}

void MInfoBanner::click()
{
    emit clicked();
}

