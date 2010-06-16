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

#include "mbanner.h"
#include "mbannermodel.h"
#include "mdebug.h"
#include "mtheme.h"

#include "mscenewindow_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MBanner)

MBanner::MBanner() :
        MSceneWindow(new MSceneWindowPrivate, new MBannerModel, MSceneWindow::NotificationEvent)
{
}

MBanner::~MBanner()
{
}

void MBanner::setIconID(const QString &iconId)
{
    model()->setIconID(iconId);
}

QString MBanner::iconID() const
{
    return model()->iconID();
}

void MBanner::click()
{
    emit clicked();
}

QString MBanner::title() const
{
    return model()->title();
}

QString MBanner::subtitle() const
{
    return model()->subtitle();
}

void MBanner::setTitle(const QString &text)
{
    model()->setTitle(text);
}

void MBanner::setSubtitle(const QString &text)
{
    model()->setSubtitle(text);
}
