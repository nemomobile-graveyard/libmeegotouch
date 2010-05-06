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
#ifndef MRESOURCEMANAGER_H
#define MRESOURCEMANAGER_H

#include <QObject>
#include <QTime>
#include <QMap>
#include <QTimer>

class QSvgRenderer;

//! \internal

class MThemeResourceManager : public QObject
{
    Q_OBJECT
public:
    static MThemeResourceManager& instance();
    ~MThemeResourceManager();

    QSvgRenderer* svgRenderer(const QString& absoluteFilePath);

    void themeChanged();

private Q_SLOTS:
    void freeUnusedResources();

private:
    MThemeResourceManager();

    struct SvgRendererInfo
    {
        SvgRendererInfo(QSvgRenderer* renderer, const QTime& timestamp) 
            : renderer(renderer), timestamp(timestamp) {}
        QSvgRenderer*   renderer;
        QTime           timestamp;
    };
    QMap<QString, SvgRendererInfo*> svgRendererInfos;
    QTimer timer;
};

//! \internal_end

#endif
