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

#include "singleselectiondialogspage.h"

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

class SingleSelectionDialogsPageCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    SingleSelectionDialogsPageCellCreator() : MAbstractCellCreator<MBasicListItem>() {
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

SingleSelectionDialogsPage::SingleSelectionDialogsPage()
    : TemplatePage(TemplatePage::DialogsAndBanners),
      policy(0),
      list(0)
{
}

QString SingleSelectionDialogsPage::timedemoTitle()
{
    return "SingleSelectionDialogsPage";
}

void SingleSelectionDialogsPage::createContent()
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

void SingleSelectionDialogsPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setObjectName("wgList");
    list->setCellCreator(new SingleSelectionDialogsPageCellCreator());
    list->setItemModel(new QStringListModel(list));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void SingleSelectionDialogsPage::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0)
        openStackedDialogs();
    else if (index.row() == 1)
        openSystemModalDialog();
    else if (index.row() == 2)
        openDialogWithProgressIndicator();
    else if (index.row() == 3)
        openMessageBox();
}

void SingleSelectionDialogsPage::openStackedDialogs()
{
    if (dialog)
        return;

    //% "Click to spawn a nested dialog"
    MButton *button = new MButton(qtTrId("xx_dialogs_and_notifications_stacked_dialog_button"));
    //% "Stacked dialogs"
    dialog = new MDialog(qtTrId("xx_dialogs_and_notifications_stacked_dialog_title"), M::CancelButton);
    dialog->setCentralWidget(button);

    connect(button, SIGNAL(clicked()), SLOT(openNestedDialog()));

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openNestedDialog()
{
    if (nestedDialog)
        return;

    //% "Click to open a nested message box"
    MButton *button = new MButton(qtTrId("xx_dialogs_and_notifications_stacked_dialog_open_nested_messagebox"));
    //% "This is a nested dialog"
    nestedDialog = new MDialog(qtTrId("xx_dialogs_and_notifications_stacked_dialog_nested_dialog_title"), M::CancelButton);
    nestedDialog->setCentralWidget(button);
    connect(button, SIGNAL(clicked()), SLOT(openNestedMessageBox()));

    nestedDialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openNestedMessageBox()
{
    if (nestedMessageBox)
        return;

    //% "I'm a nested message box"
    nestedMessageBox = new MMessageBox(qtTrId("xx_dialogs_and_notifications_stacked_dialog_messagebox_text"));
    nestedMessageBox->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openSystemModalDialog()
{
    if (dialog)
        return;

    dialog = new MDialog(
        //% "System Modal Dialog"
        qtTrId("xx_dialogs_and_notifications_system_modal_dialog_title"),
        M::OkButton);

    dialog->setCentralWidget(
        //% "I'm a window modal dialog.\n"
        //% "There's no way around me!\n"
        //% "Muwhahaha... [evil laugh]"
        new MLabel(qtTrId("xx_dialogs_and_notifications_system_modal_dialog_label")));

    dialog->setSystemModal(true);

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openDialogWithProgressIndicator()
{
    if (dialog)
        return;

    MButton *button = new MButton();
    button->setViewType(MButton::switchType);
    button->setCheckable(true);
    button->setChecked(true);
    connect(button, SIGNAL(toggled(bool)), this, SLOT(setDialogProgressIndicatorVisible(bool)));

    //% "Progress Indicator"
    MLabel *label = new MLabel(qtTrId("xx_dialogs_and_notifications_progress_indicator"));

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->addStretch();
    layout->addItem(label);
    layout->addItem(button);
    layout->addStretch();

    dialog = new MDialog("Lorem ipsum", M::NoStandardButton);
    dialog->centralWidget()->setLayout(layout);
    dialog->setProgressIndicatorVisible(true);
    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openMessageBox()
{
    if (dialog)
        return;

    //% "Hello World!"
    dialog = new MMessageBox(qtTrId("xx_dialogs_and_notifications_message_box_text"), M::OkButton);
    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::setDialogProgressIndicatorVisible(bool visible)
{
    if (dialog) {
        dialog->setProgressIndicatorVisible(visible);
    }
}

void SingleSelectionDialogsPage::retranslateUi()
{
    //% "Single Selection Dialogs"
    setTitle(qtTrId("xx_single_selection_dialog_title"));
    if (!isContentCreated())
        return;

    QStringList singleSelectionDialogTypes;
    //% "Stacked Dialogs"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_stacked_dialogs");
    //% "System Modal Dialog"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_system_modal_dialog");
    //% "Dialog with Progress Indicator"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_dialog_with_progress_indicator");
    //% "Message Box"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_message_box");

    static_cast<QStringListModel *>(list->itemModel())->setStringList(singleSelectionDialogTypes);
}
