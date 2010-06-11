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
#include <MTextEdit>

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
    : TemplatePage(TemplatePage::DialogsAndBanners)
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
}

void QueryDialogsPage::populateLayout()
{
    QStringList queryDialogTypes;
    //% "Entry Dialog"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_page_entry_dialog");
    //% "Long Dialog"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_page_long_dialog");

    list = new MList(centralWidget());
    list->setObjectName("wgList");
    list->setCellCreator(new QueryDialogsPageCellCreator());
    list->setItemModel(new QStringListModel(queryDialogTypes));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void QueryDialogsPage::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0)
        openEntryDialog();
    else if (index.row() == 1)
        openLongDialog();
}

void QueryDialogsPage::openEntryDialog()
{
    if (dialog)
        return;

    MWidget *centralWidget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    //% "Name"
    MLabel *label = new MLabel(qtTrId("xx_dialogs_and_notifications_entry_dialog_label"), centralWidget);
    MTextEdit *textEdit = new MTextEdit(MTextEditModel::SingleLine,
                                            QString(),
                                            centralWidget);
    centralWidget->setLayout(layout);

    layout->addItem(label);
    layout->addItem(textEdit);

    //% "Please enter your name"
    dialog = new MDialog(qtTrId("xx_dialogs_and_notifications_entry_dialog_title"),
                           M::OkButton | M::ResetButton);
    dialog->setCentralWidget(centralWidget);

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void QueryDialogsPage::openLongDialog()
{
    if (dialog)
        return;

    MWidget *centralWidget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);

    //% "Select printer"
    dialog = new MDialog(qtTrId("xx_dialogs_and_notifications_long_dialog_title"), M::CancelButton);
    dialog->setCentralWidget(centralWidget);
    MButton *button = 0;

    centralWidget->setLayout(layout);

#define ADD_PRINTER_BUTTON(NAME) \
    button = new MButton(NAME, centralWidget); \
    connect(button, SIGNAL(clicked()), dialog, SLOT(accept())); \
    layout->addItem(button);

    ADD_PRINTER_BUTTON("Lexmark A");
    ADD_PRINTER_BUTTON("Lexmark B");
    ADD_PRINTER_BUTTON("Lexmark C");
    ADD_PRINTER_BUTTON("Lexmark D");
    ADD_PRINTER_BUTTON("Canon Alpha");
    ADD_PRINTER_BUTTON("Canon Beta");
    ADD_PRINTER_BUTTON("Canon Gama");
    ADD_PRINTER_BUTTON("Canon Zeta");
    ADD_PRINTER_BUTTON("Brother 1");
    ADD_PRINTER_BUTTON("Brother 2");
    ADD_PRINTER_BUTTON("Brother 3");
    ADD_PRINTER_BUTTON("Brother 4");
    ADD_PRINTER_BUTTON("Xerox I");
    ADD_PRINTER_BUTTON("Xerox II");
    ADD_PRINTER_BUTTON("Xerox III");
    ADD_PRINTER_BUTTON("Xerox IV");
    ADD_PRINTER_BUTTON("Dell Roger");
    ADD_PRINTER_BUTTON("Dell Charlie");
    ADD_PRINTER_BUTTON("Dell Bravo");
    ADD_PRINTER_BUTTON("Dell Tango");
    ADD_PRINTER_BUTTON("HP X");
    ADD_PRINTER_BUTTON("HP Y");
    ADD_PRINTER_BUTTON("HP Z");
    ADD_PRINTER_BUTTON("HP Plus");
    ADD_PRINTER_BUTTON("Epson Stylus");
    ADD_PRINTER_BUTTON("Epson Pro");
    ADD_PRINTER_BUTTON("Epson Office");
    ADD_PRINTER_BUTTON("Epson Extra");

#undef ADD_PRINTER_BUTTON

    dialog->appear(MSceneWindow::DestroyWhenDone);
}
