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

#include <QDir>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>

#include <MContentItem>
#include <MImageWidget>
#include <MAbstractCellCreator>
#include <MLabel>

#include "utils.h"
#include "mgridpage.h"
#include "gridmodel.h"

class ContentItemCreator : public MAbstractCellCreator<MImageWidget>
{
public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;
    QSizeF cellSize() const;
};

void ContentItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if(!cell)
        return;

    MImageWidget *imageWidget = qobject_cast<MImageWidget*>(cell);

    MImageWidget *badge = new MImageWidget(imageWidget);
    badge->setObjectName("badge");
    badge->setPixmap( QPixmap( QDir(IMAGES_DIR).canonicalPath() + QDir::separator() + "star.png") );

    QVariant data = index.data(Qt::DecorationRole);
    QPixmap pixmap = data.value<QPixmap>();

    imageWidget->setPixmap(pixmap);
}

QSizeF ContentItemCreator::cellSize() const
{
    return QSizeF(260, 260);
}

MGridPage::MGridPage()
{
    gid = TemplatePage::ListsGridsAndMenus;
}

MGridPage::~MGridPage()
{
}

void MGridPage::createContent()
{
    MApplicationPage::createContent();

    MTheme::addPixmapDirectory(QDir(CONTACTS_DIR).canonicalPath());
    QGraphicsWidget * panel = centralWidget();
    QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(layout);

    list = new MList(panel);
    list->setColumns(3);
    list->setObjectName("wgGrid");
    layout->addItem(list);

    ContentItemCreator * cellCreator = new ContentItemCreator();
    list->setCellCreator(cellCreator);

    GridModel * model = new GridModel(QSize(250, 250), Utils::mediaArtDir());
    list->setItemModel(model);
    retranslateUi();
}

void MGridPage::retranslateUi()
{
    //% "Grid page"
    setTitle(qtTrId("xx_gridpage_title"));
    if (!isContentCreated())
        return;
}

QString MGridPage::timedemoTitle()
{
    return "Grid";
}
