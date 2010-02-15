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

#include "lifecycleapplication.h"
#include <QTimer>
#include <DuiInfoBanner>

LifeCycleApplication::LifeCycleApplication(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service)
    : DuiApplication(argc, argv, appIdentifier, service), m_bHandleSignal(true)
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
    DuiInfoBanner *infoBanner = new DuiInfoBanner(DuiInfoBanner::Information);
    infoBanner->setBodyText(message);
    infoBanner->setIconID("Icon-close");
    infoBanner->appear(DuiSceneWindow::DestroyWhenDone);
    QTimer::singleShot(5000, infoBanner, SLOT(disappear()));
}


void LifeCycleApplication::releaseMemoryHandler()
{
    if (m_bHandleSignal) {
        showReleaseMemory(QString("<b>System memory low, signal handler </b>"));
        m_bHandleSignal = false;
    }
}
