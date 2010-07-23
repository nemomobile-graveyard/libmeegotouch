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
#include <MProgressIndicator>

#include <MBasicListItem>
#include <MDetailedListItem>
#include <MAdvancedListItem>

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
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            break;
        }
        case MBasicListItem::IconWithTitle: {
            item->setTitle(index.data().toString());
            item->imageWidget()->setImage(defaultImage);
            break;    
        }
        case MBasicListItem::IconWithTitleAndSubtitle: {
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            item->imageWidget()->setImage(defaultImage);
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
            item->imageWidget()->setImage(defaultImage);
            item->sideTopImageWidget()->setImage(defaultImage);
            item->sideBottomImageWidget()->setImage(defaultImage);
            break;    
        }
        case MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel: {
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            item->imageWidget()->setImage(defaultImage);
            item->sideTopImageWidget()->setImage(defaultImage);
            //% "Side"
            item->setSideBottomTitle(qtTrId("xx_wg_contentitemspage_sidetitle"));
            break;    
        }
        case MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons: {
            item->setTitle(index.data().toString());
            //% "Subtitle"
            item->setSubtitle(qtTrId("xx_wg_contentitemspage_subtitle"));
            item->imageWidget()->setImage(defaultImage);
            item->sideTopImageWidget()->setImage(defaultImage);
            item->sideBottomImageWidget()->setImage(defaultImage);
            break;    
        }
        case MDetailedListItem::ThumbnailTitleAndTwoSideIcons: {
            item->setTitle(index.data().toString());
            item->imageWidget()->setImage(defaultImage);
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

class AdvancedListItemCreator : public MAbstractCellCreator<MAdvancedListItem>
{
public:
    AdvancedListItemCreator(MAdvancedListItem::ItemStyle style)
        : itemStyle(style),
        defaultImage(Utils::imagesDir() + "DefaultAvatar.png")
    {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);
        MAdvancedListItem *cell = dynamic_cast<MAdvancedListItem*>(recycler.take(MAdvancedListItem::staticMetaObject.className()));

        if (cell == NULL) {
            cell = new MAdvancedListItem(itemStyle);
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MAdvancedListItem *item = dynamic_cast<MAdvancedListItem*>(cell);
        if (!item)
            return;

        switch (itemStyle) {
        case MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons: {
                item->setTitle(index.data().toString());
                item->imageWidget()->setImage(defaultImage);
                item->sideTopImageWidget()->setImage(defaultImage);
                item->sideBottomImageWidget()->setImage(defaultImage);
                item->progressIndicator()->setValue(55);
                break;
            }
        case MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon: {
                item->setTitle(index.data().toString());
                item->imageWidget()->setImage(defaultImage);
                item->sideTopImageWidget()->setImage(defaultImage);
                item->progressIndicator()->setUnknownDuration(true);
                break;
            }
        default:
            break;
        };

    }

private:
    MAdvancedListItem::ItemStyle itemStyle;
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
    : TemplatePage(TemplatePage::ListsGridsAndPopups),
      contentItemLists(),
      policy(0)
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
{    //Basic list items
    //% "Single Title"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_single_title"), new BasicListItemCreator(MBasicListItem::SingleTitle)));
    //% "Title with Subtitle"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_title_with_subtitle"), new BasicListItemCreator(MBasicListItem::TitleWithSubtitle)));
    //% "Icon with Title"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_icon_with_title"),  new BasicListItemCreator(MBasicListItem::IconWithTitle)));
    //% "Icon with Title and Subtitle"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_icon_with_title_and_subtitle"),  new BasicListItemCreator(MBasicListItem::IconWithTitleAndSubtitle)));
    
    //Detailed list items
    //% "Icon with Title Subtitle and Two Side Icons"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_icon_with_title_subtitle_and_two_side_icons"),
                                       new DetailedListItemCreator(MDetailedListItem::IconTitleSubtitleAndTwoSideIcons)));
    //% "Icon with Title Subtitle and Side Icon with Label"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_icon_with_title_subtitle_and_side_icon_with_label"),
                                       new DetailedListItemCreator(MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel)));
    //% "Thumbnail with Title Subtitle and Two Side Icons"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_thumbnail_with_title_subtitle_and_two_side_icons"),
                                       new DetailedListItemCreator(MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons)));
    //% "Thumbnail with Title and Two Side Icons"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_thumbnail_with_title_and_two_side_icons"),
                                       new DetailedListItemCreator(MDetailedListItem::ThumbnailTitleAndTwoSideIcons)));
    
    //Advanced list items
    //% "Icon with Title Progress Indicator and Two Side Icons"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_icon_with_title_progress_indicator_and_two_side_icons"),
                                       new AdvancedListItemCreator(MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons)));
    //% "Icon with Title Progress Indicator and Top Side Icon"
    contentItemLists.append(createList(qtTrId("xx_wg_contentitemspage_icon_with_title_progress_indicator_and_top_side_icon"),
                                       new AdvancedListItemCreator(MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon)));
}

MList *ContentItemsPage::createList(const QString &title, MCellCreator *creator)
{
    QAbstractItemModel *model = new ContentItemsPageModel(title);

    MList *list = new MList(centralWidget());
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
