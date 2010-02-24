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
#include "duithemeresourcemanager.h"
#include <QSvgRenderer>

DuiThemeResourceManager& DuiThemeResourceManager::instance()
{
    static DuiThemeResourceManager themeResourceManager;
    return themeResourceManager;
}

DuiThemeResourceManager::DuiThemeResourceManager()
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), SLOT(freeUnusedResources()));
}

DuiThemeResourceManager::~DuiThemeResourceManager()
{
    foreach(SvgRendererInfo* info, svgRendererInfos) {
        delete info->renderer;
        delete info;
    }
}

QSvgRenderer* DuiThemeResourceManager::svgRenderer(const QString& absoluteFilePath)
{
    SvgRendererInfo* info = svgRendererInfos.value(absoluteFilePath, NULL);
    if(!info) {
        QSvgRenderer* renderer = new QSvgRenderer(absoluteFilePath);
        if(renderer->isValid()) {
            info = new SvgRendererInfo(renderer, QTime::currentTime());
            svgRendererInfos.insert(absoluteFilePath, info);
        } else {
            delete renderer;
            return NULL;
        }
    } else {
        info->timestamp = QTime::currentTime();
    }

    if(!timer.isActive()) {
        timer.start(3000);
    }
    return info->renderer;
}

void DuiThemeResourceManager::freeUnusedResources()
{
    QTime now = QTime::currentTime();

    // free unused svg resources which are kept open
    QList<QString> openedSvgRenderers = svgRendererInfos.keys();
    foreach(const QString& svg, openedSvgRenderers) {
        SvgRendererInfo* info = svgRendererInfos[svg];

        // if no-one accessed this svg for a period of time so we'll close it
        if(info->timestamp.secsTo(now) >= 3) {
            delete info->renderer;
            delete info;
            svgRendererInfos.remove(svg);
        }
    }

    // still open svg resources? 
    // we'll try to free them later
    if(svgRendererInfos.count() > 0) {
        timer.start(3000);
    }
}

void DuiThemeResourceManager::themeChanged()
{
    if(!timer.isActive()) {
        timer.stop();
    }

    qDeleteAll(svgRendererInfos);
    svgRendererInfos.clear();
}
