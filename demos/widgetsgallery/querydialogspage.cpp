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

#include "querydialogspage.h"

#include <MAbstractCellCreator>
#include <MButton>
#include <MBasicListItem>
#include <MDialog>
#include <MLabel>
#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>
#include <MMessageBox>

#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <QTimer>

class QueryDialogsPageCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    QueryDialogsPageCellCreator() : MAbstractCellCreator<MBasicListItem>() {
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

QueryDialogsPage::QueryDialogsPage()
    : TemplatePage(TemplatePage::DialogsSheetsAndBanners),
      policy(0),
      list(0),
      dialog()
{
}

QString QueryDialogsPage::timedemoTitle()
{
    return "QueryDialogsPage";
}

void QueryDialogsPage::createContent()
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

void QueryDialogsPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setCellCreator(new QueryDialogsPageCellCreator());
    list->setItemModel(new QStringListModel(list));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void QueryDialogsPage::itemClicked(const QModelIndex &index)
{
    switch (index.row()) {
    case 0:
        openMessageBox();
        break;
    case 1:
        openMessageBox(Icon);
        break;
    case 2:
        openMessageBox(LargeText);
        break;
    case 3:
        openMessageBox(LargeText | Icon);
        break;
    default:
        break;
    }
}

void QueryDialogsPage::openMessageBox(MessageBoxOptions options)
{
    if (dialog)
        return;

    MMessageBox *messageBox = new MMessageBox("", M::YesButton|M::NoButton);

    //% "ARE YOU SURE?"
    messageBox->setTitle(qtTrId("xx_dialogs_and_notifications_are_you_sure_upcase"));

    if (options & LargeText) {
        //% "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus laoreet ornare elit ac tincidunt. Ut lectus enim, vestibulum congue aliquet vitae, sagittis nec enim."
        messageBox->setText(qtTrId("xx_dialogs_and_notifications_big_lorem_impsum"));
    } else {
        //% "Lorem ipsum dolor sit amet."
        messageBox->setText(qtTrId("xx_dialogs_and_notifications_short_lorem_impsum"));
    }

    if (options & Icon)
        messageBox->setIconId("icon-l-default-application");

    dialog = messageBox;
    dialog->appear(MSceneWindow::DestroyWhenDone);
}


void QueryDialogsPage::retranslateUi()
{
    //% "Query Dialogs"
    setTitle(qtTrId("xx_query_dialog_title"));
    if (!isContentCreated())
        return;

    QStringList queryDialogTypes;
    //% "Small Query Dialog without Icon"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_small_query_dialog_without_icon");
    //% "Small Query Dialog with Icon"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_small_query_dialog_with_icon");
    //% "Big Query Dialog without Icon"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_big_query_dialog_without_icon");
    //% "Big Query Dialog with Icon"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_big_query_dialog_with_icon");


    static_cast<QStringListModel *>(list->itemModel())->setStringList(queryDialogTypes);
}
