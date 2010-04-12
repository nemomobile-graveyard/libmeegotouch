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

#include "lifecycleapplication.h"
#include <QTimer>
#include <MInfoBanner>

LifeCycleApplication::LifeCycleApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
    : MApplication(argc, argv, appIdentifier, service), m_bHandleSignal(true)
{
    connect(this, SIGNAL(memoryLow()), SLOT(releaseMemoryHandler()));
}

LifeCycleApplication::~LifeCycleApplication()
{}

void LifeCycleApplication::releaseMemory()
{
    if (!m_bHandleSignal) {
        showReleaseMemory(QString("<b>System memory low, virtual method handler</b>"));
    }
}

void LifeCycleApplication::showReleaseMemory(QString message)
{
    MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Information);
    infoBanner->setBodyText(message);
    infoBanner->setIconID("Icon-close");
    infoBanner->appear(MSceneWindow::DestroyWhenDone);
    QTimer::singleShot(5000, infoBanner, SLOT(disappear()));
}


void LifeCycleApplication::releaseMemoryHandler()
{
    if (m_bHandleSignal) {
        showReleaseMemory(QString("<b>System memory low, signal handler </b>"));
        m_bHandleSignal = false;
    }
}
