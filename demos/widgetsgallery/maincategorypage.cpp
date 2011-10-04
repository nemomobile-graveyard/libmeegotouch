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
#include "maincategorypage.h"
#include "mainpage.h"

#include "drilldownlistitem.h"

#include <MApplication>
#include <MAbstractCellCreator>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include <MPannableViewport>

class WidgetsGalleryCategoryDataModel : public QAbstractListModel {
public:
    WidgetsGalleryCategoryDataModel(QObject *parent, QAbstractItemModel *parentModel, const QModelIndex &parentIndex)
        : QAbstractListModel(parent),
          widgetsGalleryModel(parentModel),
          categoryIndex(parentIndex) {

    }

    QModelIndex parent(const QModelIndex &child) const {
        Q_UNUSED(child);

        return categoryIndex;
    }

    int rowCount(const QModelIndex &index) const {
        Q_UNUSED(index);

        return widgetsGalleryModel->rowCount(categoryIndex);
    }

    QVariant data(const QModelIndex &index, int role) const {
        return widgetsGalleryModel->data(index, role);
    }
private:
    QAbstractItemModel *widgetsGalleryModel;
    QModelIndex categoryIndex;
};

class WidgetGalleryCellCreator : public MAbstractCellCreator<DrillDownListItem>
{
public:
    WidgetGalleryCellCreator(MList *parent)
        : MAbstractCellCreator<DrillDownListItem>(),
          parent(parent)
    {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        DrillDownListItem *cell = dynamic_cast<DrillDownListItem *>(recycler.take(DrillDownListItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new DrillDownListItem(parent);
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        DrillDownListItem *item = qobject_cast<DrillDownListItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());
    }

private:
    MList *parent;
};

MainCategoryPage::MainCategoryPage(QAbstractItemModel *demosDataModel, const QModelIndex &parentIndex) :
        TimedemoPage(),
        dataModel(new WidgetsGalleryCategoryDataModel(this, demosDataModel, parentIndex)),
        list(0),
        policy(0)
{
}

QString MainCategoryPage::timedemoTitle()
{
    return "MainCategoryPage";
}

void MainCategoryPage::createContent()
{
    TimedemoPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setSpacing(0);

    populateLayout();
}

void MainCategoryPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setObjectName("list");
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        setStyleName("CommonApplicationPage");
        list->setStyleName("CommonList");
    } else {
        setStyleName("CommonApplicationPageInverted");
        list->setStyleName("CommonListInverted");
    }
    list->setCellCreator(new WidgetGalleryCellCreator(list));
    list->setItemModel(dataModel);

    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(galleryPageItemClicked(QModelIndex)));
}

void MainCategoryPage::galleryPageItemClicked(const QModelIndex &index)
{
    TemplatePage *page = static_cast<TemplatePage *>(index.data(MainPage::Page).value<void *>());
    page->setParent(this);
    page->appear(scene());
}
