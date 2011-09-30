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
#include "samplepage.h"

#include <MSceneManager>
#include <MButton>

SamplePage::SamplePage(int level) : MApplicationPage(0), level(level)
{
    QString title;

    if (level == 1) {
        title = "Root level";
    } else {
        title = QString("Level %1").arg(level);
    }

    setTitle(title);

    QString buttonTitle = QString("Open page level %1").arg(level + 1);
    MButton *button = new MButton(buttonTitle);
    connect(button, SIGNAL(clicked()), SLOT(openNextPage()));

    setCentralWidget(button);
}

void SamplePage::openNextPage()
{
    SamplePage *nextPage = new SamplePage(level + 1);
    nextPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}
