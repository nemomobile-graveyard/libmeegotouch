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
#include <MBasicListItem>
#include <MBanner>
#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

#include <QStringListModel>
#include <QTimer>
#include <MDialog>
#include <QGraphicsLinearLayout>

class BannersPageCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    BannersPageCellCreator() : MAbstractCellCreator<MBasicListItem>() {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MBasicListItem *cell = dynamic_cast<MBasicListItem *>(recycler.take(MBasicListItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new MBasicListItem(MBasicListItem::SingleTitle);
            cell->initLayout();
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MBasicListItem *item = qobject_cast<MBasicListItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());
    }
};

BannersPage::BannersPage()
    : TemplatePage(TemplatePage::DialogsSheetsAndBanners),
      policy(0),
      list(0),
      banner(0)
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

    retranslateUi();
}

void BannersPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setCellCreator(new BannersPageCellCreator());
    list->setItemModel(new QStringListModel(list));
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
    MBanner *banner = new MBanner();
    banner->setStyleName("ShortEventBanner");
    banner->setIconID("icon-s-status-sms");
    banner->setTitle("John Milles");
    banner->setSubtitle("Voicemail");
    banner->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void BannersPage::showInformationBanner()
{
    MBanner *banner = new MBanner();
    banner->setStyleName("InformationBanner");
    banner->setTitle("You have 6 meetings in your Agenda");
    banner->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void BannersPage::showSystemInformationBanner()
{
    MBanner *systemBanner = new MBanner();
    systemBanner->setStyleName("SystemBanner");
    systemBanner->setIconID("icon-m-common-usb");
    systemBanner->setTitle("Unable to continue video editing. USB disconnected.");
    systemBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void BannersPage::retranslateUi()
{
    //% "Banners"
    setTitle(qtTrId("xx_bannerspage_title"));
    if (!isContentCreated())
        return;

    QStringList bannerTypes;
    //% "Event Banner"
    bannerTypes << qtTrId("xx_wg_banners_page_event_banner");
    //% "Information Banner"
    bannerTypes << qtTrId("xx_wg_banners_page_information_banner");
    //% "System Information Banner"
    bannerTypes << qtTrId("xx_wg_banners_page_system_information_banner");

    static_cast<QStringListModel *>(list->itemModel())->setStringList(bannerTypes);
}
