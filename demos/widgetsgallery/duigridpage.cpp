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

#include <QDir>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>

#include <DuiContentItem>
#include <DuiImageWidget>
#include <DuiAbstractCellCreator>
#include <DuiLabel>

#include "utils.h"
#include "duigridpage.h"
#include "gridmodel.h"

class ContentItemCreator : public DuiAbstractCellCreator<DuiImageWidget>
{
public:
    void updateCell(const QModelIndex &index, DuiWidget *cell) const;
    QSizeF cellSize() const;
};

void ContentItemCreator::updateCell(const QModelIndex &index, DuiWidget *cell) const
{
    if(!cell)
        return;

    DuiImageWidget *imageWidget = qobject_cast<DuiImageWidget*>(cell);

    DuiImageWidget *badge = new DuiImageWidget(imageWidget);
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

DuiGridPage::DuiGridPage()
{
    setPannableAreaInteractive(true);
    gid = TemplatePage::ListsGridsAndMenus;
}

DuiGridPage::~DuiGridPage()
{
}

void DuiGridPage::createContent()
{
    DuiApplicationPage::createContent();

    DuiTheme::addPixmapDirectory(QDir(MEDIA_ART_DIR).canonicalPath());
    DuiWidget * panel = centralWidget();
    QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(layout);

    list = new DuiList(panel);
    list->setColumns(3);
    list->setObjectName("wgGrid");
    layout->addItem(list);

    ContentItemCreator * cellCreator = new ContentItemCreator();
    list->setCellCreator(cellCreator);

    GridModel * model = new GridModel(QSize(250, 250), Utils::mediaArtDir());
    list->setItemModel(model);
}

void DuiGridPage::retranslateUi()
{
    setTitle(qtTrId("xx_gridpage_title"));
}

QString DuiGridPage::timedemoTitle()
{
    return "Grid";
}
