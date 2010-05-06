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

#include <MApplicationPage>
#include <MLocale>
#include <MWidgetFactory>
#include <MImage>
#include <MGrid>

#include "trackergridpage.h"

// Implementation for custom grid widget factory plugin
MWidget *GridWidgetFactoryPlugin::onBuild(QMap<QString, QVariant>* data)
{
    qDebug() << "GridWidgetFactoryPlugin::onBuild()";

    // Check that Mime type is image
    if (data->value("?GridItem_Mime").toString().contains("image/")) {
        // Make full path to image file
        QString fullpath = data->value("?GridItem_Path").toString() +
                           QString("/") +
                           data->value("?GridItem_Name").toString();

        // nfo::Image ontology presents paths like this: "file:///home/user"
        // so strip the prefix
        if (fullpath.startsWith(QString("file:/")))
            fullpath = fullpath.right(fullpath.length() - 6);

        // Construct MImage with the full path to image
        qDebug() << "...creating MImage from " << fullpath;
        MImage *widget = new MImage(fullpath);

        // Set widget size
        QSize size(128, 128);
        widget->setMinimumSize(size);
        widget->setMaximumSize(size);
        widget->setPreferredSize(size);

        return (MWidget *)widget;
    }
    return 0;
}

QStringList GridWidgetFactoryPlugin::dataFields()
{
    qDebug() << "GridWidgetFactoryPlugin::dataFields()";

    static QStringList list;

    if (list.empty()) {
        list << "?GridItem_Name";
        list << "?GridItem_Path";
        list << "?GridItem_Mime";
    }

    return list;
}



// Implementation for tracker grid page
TrackerGridPage::TrackerGridPage()
{
    qDebug() << "TrackerGridPage::TrackerGridPage()";

    //% "TrackerGridPage"
    setTitle(qtTrId("xx_tracker_grid_page_title"));
    setObjectName("trackergridpage");
}

TrackerGridPage::~TrackerGridPage()
{
}

void TrackerGridPage::createContent()
{
    qDebug() << "TrackerGridPage::createContent()";

    MApplicationPage::createContent();

    // Grid for presenting tracker items
    grid = new MGrid();
    grid->resize(800, 450);
    grid->setSpacing(10);
    grid->setLayoutMode(MGridModel::SinglePass);
    grid->setBatchSize(100);
    setCentralWidget(grid);

    // Attach a custom grid plugin to widget factory:
    // default plugins cannot handle tracker data format
    MWidgetFactory *pFactory = MWidgetFactory::instance();
    pFactory->attachPlugin(new GridWidgetFactoryPlugin());

    // Query 20 images from tracker
    SopranoLive::RDFSelect query;
    makeQueryForImages(query, 20);
    imageNodes = tracker()->modelQuery(query);

    // Connect signals to get live data updates
    connectUpdateSignals(imageNodes);

    // Pass the abstract item model to grid
    grid->setItemModel(imageNodes.model());
}

// SELECT ?__GridItem_MimeObjOfnie_mimeType ?__GridItem_PathObjOfnfo_belongsToContainer ?__GridItem_NameObjOfnfo_fileName
// WHERE {
//   {
//     ?__1 <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image> .
//     ?__1 <http://www.semanticdesktop.org/ontologies/2007/01/19/nie#mimeType> ?__GridItem_MimeObjOfnie_mimeType .
//     ?__1 <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#belongsToContainer> ?__GridItem_PathObjOfnfo_belongsToContainer .
//     ?__1 <http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#fileName> ?__GridItem_NameObjOfnfo_fileName .
//   }
// }
// LIMIT 20
void TrackerGridPage::makeQueryForImages(SopranoLive::RDFSelect &query, int limit)
{
    qDebug() << "TrackerGridPage::makeQueryForImages()";

    SopranoLive::RDFVariable image = SopranoLive::RDFVariable::fromType<SopranoLive::nfo::Image>();
    query.limit(limit);
    query.addColumn("GridItem_Mime", image.property<SopranoLive::nie::mimeType>());
    query.addColumn("GridItem_Path", image.property<SopranoLive::nfo::belongsToContainer>());
    query.addColumn("GridItem_Name", image.property<SopranoLive::nfo::fileName>());
    qDebug() << query.getQuery();
}

void TrackerGridPage::connectUpdateSignals(const SopranoLive::LiveNodes &nodes)
{
    qDebug() << "TrackerGridPage::connectUpdateSignals()";

    QObject::connect(nodes.model(), SIGNAL(rowsInserted(QModelIndex, int, int))
                     , this, SLOT(updateGrid()));
    QObject::connect(nodes.model(), SIGNAL(rowsRemoved(QModelIndex, int, int))
                     , this, SLOT(updateGrid()));
    QObject::connect(nodes.model(), SIGNAL(dataChanged(QModelIndex, QModelIndex))
                     , this, SLOT(updateGrid()));
}

void TrackerGridPage::updateGrid()
{
    qDebug() << "TrackerGridPage::updateGrid()";
}

// End of file
