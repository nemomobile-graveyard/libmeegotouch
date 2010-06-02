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

#include "bannerspage.h"

#include <MAbstractCellCreator>
#include <MContentItem>
#include <MInfoBanner>
#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

#include <QStringListModel>
#include <QTimer>

class BannersPageCellCreator : public MAbstractCellCreator<MContentItem>
{
public:
    BannersPageCellCreator() : MAbstractCellCreator<MContentItem>() {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MContentItem *cell = dynamic_cast<MContentItem *>(recycler.take(MContentItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new MContentItem(MContentItem::SingleTextLabel);
            cell->setObjectName("wgMainPageCategoryItem");
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MContentItem *item = qobject_cast<MContentItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());
    }
};

BannersPage::BannersPage()
    : TemplatePage(TemplatePage::DialogsAndBanners)
{
}

QString BannersPage::timedemoTitle()
{
    return "BannersPage";
}

void BannersPage::createContent()
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

void BannersPage::populateLayout()
{
    QStringList bannerTypes;
    //% "Event Banner"
    bannerTypes << qtTrId("xx_wg_banners_page_event_banner");
    //% "Information Banner"
    bannerTypes << qtTrId("xx_wg_banners_page_information_banner");
    //% "System Information Banner"
    bannerTypes << qtTrId("xx_wg_banners_page_system_information_banner");

    list = new MList(centralWidget());
    list->setObjectName("wgList");
    list->setCellCreator(new BannersPageCellCreator());
    list->setItemModel(new QStringListModel(bannerTypes));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void BannersPage::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0)
        showEventBanner();
    else if (index.row() == 1)
        showInformationBanner();
    else if (index.row() == 2)
        showSystemInformationBanner();
}

void BannersPage::showEventBanner()
{
    MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Event);
    infoBanner->setImageID("icon-m-content-email");
    infoBanner->setBodyText(
        //% "<b>Ida Taipale</b><br/>Have you seen my dog?"
        qtTrId("xx_dialogs_and_notifications_event_banner"));
    infoBanner->setIconID("icon-m-content-mms");
    connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
    infoBanner->appear(MSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}

void BannersPage::showInformationBanner()
{
    MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Information);
    infoBanner->setImageID("icon-m-startup-help");
    infoBanner->setBodyText(
        //% "<b>Battery is running low</b>"
        qtTrId("xx_dialogs_and_notifications_information_banner"));
    infoBanner->appear(MSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}

void BannersPage::showSystemInformationBanner()
{
    MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Information);
    infoBanner->setImageID("icon-m-telephony-call-answer");
    infoBanner->setBodyText(
        //% "<b>Incoming call</b>"
        qtTrId("xx_dialogs_and_notifications_system_information_banner"));
    //% "Accept"
    infoBanner->setButtonText(qtTrId("xx_dialogs_and_notifications_system_information_banner_accept"));
    connect(infoBanner, SIGNAL(buttonClicked()), this, SLOT(openMessageBox()));
    infoBanner->appear(MSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}
