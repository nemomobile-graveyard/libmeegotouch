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
    : TemplatePage(TemplatePage::DialogsAndBanners),
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
        openQuestionDialog();
        break;
    case 1:
        openEntryDialog();
        break;
    case 2:
        openLongDialog();
        break;
    case 3:
        openMessageBox();
        break;
    case 4:
        openMessageBox(Icon);
        break;
    case 5:
        openMessageBox(LargeText);
        break;
    case 6:
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

void QueryDialogsPage::openQuestionDialog()
{
    if (dialog)
        return;

    dialog = new MDialog(
        //%  "Question Dialog Title"
        qtTrId("xx_dialogs_and_notifications_question_dialog_title"),
        M::YesButton | M::NoButton);
    dialog->setCentralWidget(
        //% "Lorem ipsum dolor sit amet?"
        new MLabel(qtTrId("xx_dialogs_and_notifications_question_dialog_content")));

    dialog->appear(MSceneWindow::DestroyWhenDone);
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

void QueryDialogsPage::retranslateUi()
{
    //% "Query Dialogs"
    setTitle(qtTrId("xx_query_dialog_title"));
    if (!isContentCreated())
        return;

    QStringList queryDialogTypes;
    //% "Question Dialog"
    queryDialogTypes << qtTrId("xx_wg_multiple_selection_dialogs_page_question_dialog");
    //% "Entry Dialog"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_page_entry_dialog");
    //% "Long Dialog"
    queryDialogTypes << qtTrId("xx_wg_query_dialogs_page_long_dialog");
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
