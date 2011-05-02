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

#ifndef NOTIFICATIONSPAGE_H
#define NOTIFICATIONSPAGE_H

#include "templatepage.h"

#include <MLabel>
#include <MTextEdit>
#include <MButton>

class MList;

class NotificationsPage : public TemplatePage
{
    Q_OBJECT

public:

    NotificationsPage();

    QString timedemoTitle();

    void createContent();

protected:

    void retranslateUi();

private Q_SLOTS:

    void sendNotification();
    void toggleNotificationPreviewEnabled();

private:

    MLinearLayoutPolicy *policy;
    MLabel * labelPageTitle;
    MLabel * labelTitleNotification;
    MLabel * labelSubtitleNotification;
    MTextEdit * textEditTitle;
    MTextEdit * textEditSubTitle;
    MLabel * labelIconEnable;
    MButton * checkBoxIconEnable;
    MLabel * labelPreviewEnable;
    MButton * checkBoxPreview;
    MLabel *footnote;
};

#endif
