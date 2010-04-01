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

#include "duiinfobanner.h"
#include "duiinfobanner_p.h"
#include <duidebug.h>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiInfoBanner)

const DuiTheme::ViewType DuiInfoBanner::informationType  = "information";
const DuiTheme::ViewType DuiInfoBanner::eventType        = "event";

DuiInfoBanner::DuiInfoBanner(BannerType type) :
    DuiSceneWindow(new DuiInfoBannerPrivate, new DuiInfoBannerModel,
                   (type == Information) ? DuiSceneWindow::NotificationInformation : DuiSceneWindow::NotificationEvent,
                   (type == Information) ? informationType : eventType)
{
    Q_D(DuiInfoBanner);

    d->bannerType = type;
}

DuiInfoBanner::~DuiInfoBanner()
{
}

void DuiInfoBanner::setImageID(const QString &image)
{
    model()->setImageID(image);
}

QString DuiInfoBanner::imageID() const
{
    return model()->imageID();
}

void DuiInfoBanner::setPixmap(const QPixmap &pixmap)
{
    model()->setPixmap(pixmap);
}

QPixmap DuiInfoBanner::pixmap() const
{
    return model()->pixmap();
}

void DuiInfoBanner::setBodyText(const QString &text)
{
    model()->setBodyText(text);
}

QString DuiInfoBanner::bodyText() const
{
    return model()->bodyText();
}

void DuiInfoBanner::setIconID(const QString &iconId)
{
    model()->setIconID(iconId);
}

QString DuiInfoBanner::iconID() const
{
    return model()->iconID();
}

DuiInfoBanner::BannerType DuiInfoBanner::bannerType()
{
    Q_D(DuiInfoBanner);

    return d->bannerType;
}

void DuiInfoBanner::setButtonText(const QString &text)
{
    model()->setButtonText(text);
}

QString DuiInfoBanner::buttonText() const
{
    return model()->buttonText();
}

void DuiInfoBanner::click()
{
    emit clicked();
}

