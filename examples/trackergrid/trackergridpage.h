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

#ifndef TRACKERGRIDPAGE_H
#define TRACKERGRIDPAGE_H

#include <QtTracker/Tracker>
#include <QtTracker/ontologies/nie.h>
#include <QtTracker/ontologies/nfo.h>

#include <MApplicationPage>
#include <MWidgetFactoryPluginInterface>

// Forward class declarations
class MGrid;



// Class declaration for custom grid widget factory plugin
class GridWidgetFactoryPlugin: public MWidgetFactoryPluginInterface
{
    Q_OBJECT

public:
    MWidget *onBuild(QMap<QString, QVariant>* data);
    QStringList dataFields();
};



// Class declaration for tracker grid page
class TrackerGridPage : public MApplicationPage
{
    Q_OBJECT

public:
    TrackerGridPage();
    virtual ~TrackerGridPage();
    virtual void createContent();

public slots:
    void updateGrid();

private:
    void makeQueryForImages(SopranoLive::RDFSelect &query, int limit);
    void connectUpdateSignals(const SopranoLive::LiveNodes &nodes);

private:
    SopranoLive::LiveNodes imageNodes;
    MGrid *grid;
};

#endif // TRACKERGRIDPAGE_H
