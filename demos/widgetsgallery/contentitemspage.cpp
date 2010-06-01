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
#include <MImageWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include <MListItem>

#include <MBasicListItem>
#include <MDetailedListItem>

class BasicListItemCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    BasicListItemCreator(MBasicListItem::ItemStyle style)
        : itemStyle(style),
        defaultImage(Utils::imagesDir() + "DefaultAvatar.png")
    {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);
        MBasicListItem *cell = dynamic_cast<MBasicListItem*>(recycler.take(MBasicListItem::staticMetaObject.className()));
        
        if (cell == NULL) {
            cell = new MBasicListItem(itemStyle);
            cell->initLayout();
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MBasicListItem *item = dynamic_cast<MBasicListItem*>(cell);
        if (!item)
            return;

        switch (itemStyle) {
        case MBasicListItem::SingleTitle: {
            
            item->setTitle(index.data().toString());
            break;
        }
        case MBasicListItem::TitleWithSubtitle: {
            MBasicListItem *item = dynamic_cast<MBasicListItem*>(cell);
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            break;
        }
        case MBasicListItem::IconWithTitle: {
            item->setTitle(index.data().toString());
            item->icon()->setImage(defaultImage);
            break;    
        }
        case MBasicListItem::IconWithTitleAndSubtitle: {
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            item->icon()->setImage(defaultImage);
            break;    
        }
        default:
            break;
        };

    }

private:
    MBasicListItem::ItemStyle itemStyle;
    QImage defaultImage;
};


class DetailedListItemCreator : public MAbstractCellCreator<MDetailedListItem>
{
public:
    DetailedListItemCreator(MDetailedListItem::ItemStyle style)
        : itemStyle(style),
        defaultImage(Utils::imagesDir() + "DefaultAvatar.png")
    {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);
        MDetailedListItem *cell = dynamic_cast<MDetailedListItem*>(recycler.take(MDetailedListItem::staticMetaObject.className()));
        
        if (cell == NULL) {
            cell = new MDetailedListItem(itemStyle);
            cell->initLayout();
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MDetailedListItem *item = dynamic_cast<MDetailedListItem*>(cell);
        if (!item)
            return;

        switch (itemStyle) {
        case MDetailedListItem::IconTitleSubtitleAndTwoSideIcons: {
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            item->icon()->setImage(defaultImage);
            item->sideTopImageWidget()->setImage(defaultImage);
            item->sideBottomImageWidget()->setImage(defaultImage);
            break;    
        }
        case MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel: {
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            item->icon()->setImage(defaultImage);
            item->sideTopImageWidget()->setImage(defaultImage);
            //% "Side"
            item->setSideBottomTitle(qtTrId("xx_wg_contentitemspage_sidetitle"));
            break;    
        }
        case MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons: {
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            item->thumbnail()->setImage(defaultImage);
            item->sideTopImageWidget()->setImage(defaultImage);
            item->sideBottomImageWidget()->setImage(defaultImage);
            break;    
        }
        case MDetailedListItem::ThumbnailTitleAndTwoSideIcons: {
            item->setTitle(index.data().toString());
            item->icon()->setImage(defaultImage);
            item->sideTopImageWidget()->setImage(defaultImage);
            item->sideBottomImageWidget()->setImage(defaultImage);
            break;    
        }
        default:
            break;
        };

    }

private:
    MDetailedListItem::ItemStyle itemStyle;
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
    
    retranslateUi();
}

void ContentItemsPage::populateLayout()
{
    //Basic list items
    contentItemLists.append(createList("Single Title", new BasicListItemCreator(MBasicListItem::SingleTitle)));
    contentItemLists.append(createList("Title with Subtitle", new BasicListItemCreator(MBasicListItem::TitleWithSubtitle)));
    contentItemLists.append(createList("Icon with Title",  new BasicListItemCreator(MBasicListItem::IconWithTitle)));
    contentItemLists.append(createList("Icon with Title and Subtitle",  new BasicListItemCreator(MBasicListItem::IconWithTitleAndSubtitle)));
    
    //Detailed list items
    contentItemLists.append(createList("Icon with Title Subtitle and Two Side Icons", 
                                       new DetailedListItemCreator(MDetailedListItem::IconTitleSubtitleAndTwoSideIcons)));
    contentItemLists.append(createList("Icon with Title Subtitle and Side Icon with Label", 
                                       new DetailedListItemCreator(MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel)));
    contentItemLists.append(createList("Thumbnail with Title Subtitle and Two Side Icons", 
                                       new DetailedListItemCreator(MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons)));
    contentItemLists.append(createList("Thumbnail with Title and Two Side Icons",
                                       new DetailedListItemCreator(MDetailedListItem::ThumbnailTitleAndTwoSideIcons)));
    
    /*contentItemLists.append(createList("Single Icon", MContentItem::SingleIcon));
    contentItemLists.append(createList("Icon and Single Text Label Vertical", MContentItem::IconAndSingleTextLabelVertical));
    contentItemLists.append(createList("Icon and Two Text Labels Vertical", MContentItem::IconAndTwoTextLabelsVertical));
    contentItemLists.append(createList("Two Icons Two Widgets", MContentItem::TwoIconsTwoWidgets));*/
}

MList *ContentItemsPage::createList(const QString &title, MCellCreator *creator)
{
    QAbstractItemModel *model = new ContentItemsPageModel(title);

    MList *list = new MList(centralWidget());
    list->setObjectName("wgList");
    list->setCellCreator(creator);
    list->setItemModel(model);
    list->setShowGroups(true);
    policy->addItem(list, Qt::AlignCenter);

    return list;
}

void ContentItemsPage::retranslateUi()
{
    //% "Content Items"
    setTitle(qtTrId("xx_contentitemspage_title"));
    if (!isContentCreated())
        return;
    // this file has no other calls to qtTrId() except for the title
    // at the moment. If more qtTrId() calls are needed they should
    // be added here.
}
