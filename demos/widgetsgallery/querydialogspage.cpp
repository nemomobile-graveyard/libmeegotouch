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

#include "querydialogspage.h"

#include <MApplication>
#include <MAbstractCellCreator>
#include <MButton>
#include <MBasicListItem>
#include <MDialog>
#include <MLabel>
#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>
#include <MMessageBox>
#include <MPannableViewport>

#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <QTimer>

#ifdef HAVE_LIBNGF
#   include <dbus/dbus-glib-lowlevel.h>
#   include <QDBusMessage>
#   include <QDBusConnection>
#endif

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
            if (MApplication::instance()->objectName() == "widgetsgallery") {
                cell->setStyleName("CommonBasicListItem");
            } else {
                cell->setStyleName("CommonBasicListItemInverted");
            }
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
#ifdef HAVE_LIBNGF
    connection = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    dbus_connection_setup_with_g_main(connection, NULL);

    ngfClient = ngf_client_create(NGF_TRANSPORT_DBUS, connection);
#endif //HAVE_LIBNGF
}


QueryDialogsPage::~QueryDialogsPage()
{
#ifdef HAVE_LIBNGF
    ngf_client_destroy(ngfClient);
    dbus_connection_unref(connection);
#endif
}


QString QueryDialogsPage::timedemoTitle()
{
    return "QueryDialogsPage";
}

void QueryDialogsPage::createContent()
{
    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    setStyleName(inv("CommonApplicationPage"));

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
    list->setObjectName("list");
    list->setStyleName(inv("CommonList"));
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
    case 4:
        openMessageBox(NgfEffect);
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
    messageBox->setObjectName("messageBox");

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

#ifdef HAVE_LIBNGF
    if (options & NgfEffect) {
        // play non graphical feedback effect
        NgfProplist *p = NULL;
        const gchar *event = "query";

        ngf_client_play_event(ngfClient, event, p);

        // OBS: A more encapsulated way of doing it would be to
        // play it from within your MMessageBox specialized class
        // when it leaves the MSceneWindow::Disappeared state
        // (see MSceneWindow::sceneWindowStateChanged())
    }
#endif

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
    //% "Query Dialog with NGF effect"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_query_dialog_with_ngf_effect");


    static_cast<QStringListModel *>(list->itemModel())->setStringList(queryDialogTypes);
}
