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

#ifndef MSCENEMANAGERSTYLE_H
#define MSCENEMANAGERSTYLE_H

#include "mstyle.h"

class MSceneManagerStyle : public MStyle
{
    Q_OBJECT
    M_STYLE(MSceneManagerStyle)

    M_STYLE_ATTRIBUTE(QString, pageSwitchAnimation, PageSwitchAnimation)
    M_STYLE_ATTRIBUTE(QString, dialogAnimation, DialogAnimation)
    M_STYLE_ATTRIBUTE(QString, notificationInformationAnimation, NotificationInformationAnimation)
    M_STYLE_ATTRIBUTE(QString, notificationEventAnimation, NotificationEventAnimation)
    M_STYLE_ATTRIBUTE(QString, applicationMenuAnimation, ApplicationMenuAnimation)
    M_STYLE_ATTRIBUTE(QString, popupListAnimation, PopupListAnimation)
    M_STYLE_ATTRIBUTE(QString, messageBoxAnimation, MessageBoxAnimation)
    M_STYLE_ATTRIBUTE(QString, statusBarAnimation, StatusBarAnimation)
    M_STYLE_ATTRIBUTE(QString, objectMenuAnimation, ObjectMenuAnimation)
    M_STYLE_ATTRIBUTE(QString, navigationBarAnimation, NavigationBarAnimation)
    M_STYLE_ATTRIBUTE(QString, systemBannerAnimation, SystemBannerAnimation)
    M_STYLE_ATTRIBUTE(QString, sheetAnimation, SheetAnimation)
};

class MSceneManagerStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER(MSceneManagerStyle)
};

#endif // MSCENEMANAGERSTYLE_H
