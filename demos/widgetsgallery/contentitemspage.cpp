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

#include "contentitemspage.h"
#include "utils.h"

#include <MAbstractCellCreator>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>

#include <MDebug>

class ContentItemsPageCellCreator : public MAbstractCellCreator<MContentItem>
{
public:
    ContentItemsPageCellCreator(MContentItem::ContentItemStyle style)
        : itemStyle(style),
        defaultImage(Utils::imagesDir() + "DefaultAvatar.png")
    {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MContentItem *cell = dynamic_cast<MContentItem *>(recycler.take(MContentItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new MContentItem(itemStyle);
            cell->setObjectName("wgContentItemsPageItem");

        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MContentItem *item = qobject_cast<MContentItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());

        switch (itemStyle) {
        case MContentItem::IconAndTwoTextLabels:
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_content_items_subtitle"));
            item->setImage(defaultImage);
            break;
        case MContentItem::IconAndSingleTextLabel:
            item->setImage(defaultImage);
            break;
        case MContentItem::TwoTextLabels:
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_content_items_subtitle"));
            break;
        case MContentItem::SingleIcon:
            item->setImage(defaultImage);
            break;
        case MContentItem::IconAndSingleTextLabelVertical:
            item->setImage(defaultImage);
            break;
        case MContentItem::IconAndTwoTextLabelsVertical:
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_content_items_subtitle"));
            item->setImage(defaultImage);
            break;
        case MContentItem::TwoIconsTwoWidgets:
            item->setImage(defaultImage);
            item->setOptionalImage(defaultImage);
            item->enableProgressBar();
            dynamic_cast<MProgressIndicator*>(item->additionalItem())->setValue(25 * index.row());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_content_items_subtitle"));
            break;
        default:
            break;
        };

        const_cast<ContentItemsPageCellCreator*>(this)->updateCellSize(item);
    }

    void updateCellSize(const MWidget *cell) {
        size = cell->effectiveSizeHint(Qt::PreferredSize);
        mDebug("") << size;
    }

    QSizeF cellSize() const {
        return size;
    }

private:
    QSizeF size;
    MContentItem::ContentItemStyle itemStyle;
    QImage defaultImage;
};

class ContentItemsPageModel : public QAbstractItemModel {
public:
    ContentItemsPageModel(const QString &title)
        : QAbstractItemModel(),
        headerTitle(title)
    {
    }

    QModelIndex parent(const QModelIndex &child) const {
        Q_UNUSED(child);

        if (child.internalId() == 0)
            return index(0, 0);

        return QModelIndex();
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const {
        if (parent.isValid())
            return createIndex(row, column, 0);
        else
            return createIndex(row, column, 1);
    }

    int rowCount(const QModelIndex &parent) const {
        if (parent.isValid())
            return 2;

        return 1;
    }

    int columnCount(const QModelIndex &parent) const {
        Q_UNUSED(parent);

        return 1;
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (role == Qt::DisplayRole) {
            if (!index.parent().isValid())
                return QVariant::fromValue(headerTitle);
            else
                return QVariant::fromValue(QString(headerTitle + " %1").arg(index.row()));
        }

        return QVariant();
    }

private:
    QString headerTitle;
};

ContentItemsPage::ContentItemsPage()
    : TemplatePage(TemplatePage::ListsGridsAndPopups)
{
}

QString ContentItemsPage::timedemoTitle()
{
    return "ContentItems";
}

void ContentItemsPage::createContent()
{
    MApplicationPage::createContent();

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);

    populateLayout();
}

void ContentItemsPage::populateLayout()
{
    contentItemLists.append(createList("Icon and Two Text Labels", MContentItem::IconAndTwoTextLabels));
    contentItemLists.append(createList("Single Text Label", MContentItem::SingleTextLabel));
    contentItemLists.append(createList("Icon and Single Text Label", MContentItem::IconAndSingleTextLabel));
    contentItemLists.append(createList("Two Text Labels", MContentItem::TwoTextLabels));
    contentItemLists.append(createList("Single Icon", MContentItem::SingleIcon));
    contentItemLists.append(createList("Icon and Single Text Label Vertical", MContentItem::IconAndSingleTextLabelVertical));
    contentItemLists.append(createList("Icon and Two Text Labels Vertical", MContentItem::IconAndTwoTextLabelsVertical));
    contentItemLists.append(createList("Two Icons Two Widgets", MContentItem::TwoIconsTwoWidgets));
}

MList *ContentItemsPage::createList(const QString &title, MContentItem::ContentItemStyle style)
{
    QAbstractItemModel *model = new ContentItemsPageModel(title);

    MList *list = new MList(centralWidget());
    list->setObjectName("wgList");
    list->setCellCreator(new ContentItemsPageCellCreator(style));
    list->setItemModel(model);
    list->setShowGroups(true);
    policy->addItem(list, Qt::AlignCenter);

    return list;
}
