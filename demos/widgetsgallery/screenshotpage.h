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

#ifndef SCREENSHOTPAGE_H
#define SCREENSHOTPAGE_H

#include "templatepage.h"
#include <mgconfitem.h>
#include <MTextEdit>

class MGConfItem;

class ScreenShotPage: public TemplatePage
{
    Q_OBJECT

public:
    ScreenShotPage();
    virtual ~ScreenShotPage();

    virtual void createContent();
    virtual QString timedemoTitle();

private slots:

    void takeScreenshot();
    void screenshotDelay();

protected:

    virtual void retranslateUi();

private:

    void playScreenshotEffect();

    MLinearLayoutPolicy *layoutPolicy;
    MButton *buttonScreenshot;
    MLabel *label1;
    MLabel * secsLabel;
    MLabel *labelDescription;
    MTextEdit * entrySeconds;
};

#endif
