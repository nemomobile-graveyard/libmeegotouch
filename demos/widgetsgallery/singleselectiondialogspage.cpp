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
#include <MTextEdit>

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
    : TemplatePage(TemplatePage::DialogsSheetsAndBanners),
      policy(0),
      list(0),
      dialog(),
      nestedDialog(),
      nestedMessageBox()
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
    list->setCellCreator(new SingleSelectionDialogsPageCellCreator());
    list->setItemModel(new QStringListModel(list));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void SingleSelectionDialogsPage::itemClicked(const QModelIndex &index)
{
    switch (index.row()) {
    case 0:
        openLongDialog();
        break;
    case 1:
        openSystemDialog();
        break;
    case 2:
        openSystemModalDialog();
        break;
    case 3:
        openDialogWithProgressIndicator();
        break;
    case 4:
        openStackedDialogs();
        break;
    case 5:
        openDialogWithIcon();
        break;
    default:
        break;
    }
}

void SingleSelectionDialogsPage::openStackedDialogs()
{
    if (dialog)
        return;

    QGraphicsWidget *alignContainer = new QGraphicsWidget();
    QGraphicsWidget *leftSpacer = createSpacer();
    QGraphicsWidget *rightSpacer = createSpacer();
    QGraphicsLinearLayout  *alignLayout = new QGraphicsLinearLayout(Qt::Horizontal, alignContainer);
    alignLayout->addItem(leftSpacer);

    //% "Click to spawn a nested dialog"
    MButton *button = new MButton(qtTrId("xx_dialogs_and_notifications_stacked_dialog_button"));
    button->setStyleName("CommonSingleButtonInverted");
    //% "Stacked dialogs"
    dialog = new MDialog(qtTrId("xx_dialogs_and_notifications_stacked_dialog_title"), M::NoStandardButton);
    alignLayout->addItem(button);
    alignLayout->addItem(rightSpacer);
    dialog->setCentralWidget(alignContainer);

    connect(button, SIGNAL(clicked()), SLOT(openNestedDialog()));

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openNestedDialog()
{
    if (nestedDialog)
        return;

    QGraphicsWidget *alignContainer = new QGraphicsWidget();
    QGraphicsWidget *leftSpacer = createSpacer();
    QGraphicsWidget *rightSpacer = createSpacer();
    QGraphicsLinearLayout  *alignLayout = new QGraphicsLinearLayout(Qt::Horizontal, alignContainer);
    alignLayout->addItem(leftSpacer);

    //% "Click to open a nested message box"
    MButton *button = new MButton(qtTrId("xx_dialogs_and_notifications_stacked_dialog_open_nested_messagebox"));
    button->setStyleName("CommonSingleButtonInverted");
    //% "This is a nested dialog"
    nestedDialog = new MDialog(qtTrId("xx_dialogs_and_notifications_stacked_dialog_nested_dialog_title"), M::NoStandardButton);
    alignLayout->addItem(button);
    alignLayout->addItem(rightSpacer);
    nestedDialog->setCentralWidget(alignContainer);
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

void SingleSelectionDialogsPage::openSystemDialog()
{
    if (dialog)
        return;

    dialog = new MDialog(
        //% "System Dialog"
        qtTrId("xx_dialogs_and_notifications_system_dialog_title"), M::NoStandardButton);
    //% "I'm a system dialog.<br>"
    //% "You can skip me with the home button.<br>"
    //% "I'll be minimised to the task switcher<br>"
    //% "but I'll remain alive until you make a selection."
    MLabel *label = new MLabel(qtTrId("xx_dialogs_and_notifications_system_dialog_label"));
    label->setStyleName("CommonBodyTextInverted");
    label->setWordWrap(true);
    dialog->setCentralWidget(label);

    dialog->setSystem(true);
    dialog->setModal(false);

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openSystemModalDialog()
{
    if (dialog)
        return;

    dialog = new MDialog(
        //% "System Modal Dialog"
        qtTrId("xx_dialogs_and_notifications_system_modal_dialog_title"), M::OkButton);
    //% "I'm a system modal dialog.<br>"
    //% "You can't skip me as I'm designed for<br>"
    //% "use cases that require immediate user attention."

    MLabel *textSystemModal= new MLabel(qtTrId("xx_dialogs_and_notifications_system_modal_dialog_label"));
    textSystemModal->setStyleName("CommonBodyTextInverted");
    textSystemModal->setAlignment(Qt::AlignCenter);
    textSystemModal->setWordWrap(true);
    textSystemModal->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->addStretch();
    layout->addItem(textSystemModal);
    layout->addStretch();
    dialog->centralWidget()->setLayout(layout);

    dialog->setSystem(true);

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openDialogWithProgressIndicator()
{
    if (dialog)
        return;

    MButton *button = new MButton();
    button->setViewType(MButton::switchType);
    button->setStyleName("CommonSwitchInverted");
    button->setCheckable(true);
    button->setChecked(true);
    connect(button, SIGNAL(toggled(bool)), this, SLOT(setDialogProgressIndicatorVisible(bool)));

    //% "Progress Indicator"
    MLabel *label = new MLabel(qtTrId("xx_dialogs_and_notifications_progress_indicator"));
    label->setStyleName("CommonTitleInverted");

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

void SingleSelectionDialogsPage::openDialogWithIcon()
{
    if (dialog)
        return;

    MButton *button = new MButton();
    button->setViewType(MButton::switchType);
    button->setStyleName("CommonSwitchInverted");
    button->setCheckable(true);
    button->setChecked(true);
    connect(button, SIGNAL(toggled(bool)), this, SLOT(setDialogIconVisible(bool)));

    //% "Title Bar Icon"
    MLabel *label = new MLabel(qtTrId("xx_dialogs_and_notifications_icon"));
    label->setStyleName("CommonTitleInverted");

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->addStretch();
    layout->addItem(label);
    layout->addItem(button);
    layout->addStretch();

    dialog = new MDialog("Lorem ipsum", M::NoStandardButton);
    dialog->centralWidget()->setLayout(layout);
    dialog->setTitleBarIconId("icon-l-default-application");
    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::openLongDialog()
{
    if (dialog)
        return;

    MWidget *centralWidget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    //% "Select printer"
    dialog = new MDialog(qtTrId("xx_dialogs_and_notifications_long_dialog_title"), M::NoStandardButton);
    dialog->setCentralWidget(centralWidget);

    centralWidget->setLayout(layout);

    const char * printers[] = {"Lexmark A", "Lexmark A", "Lexmark B", "Lexmark C", "Lexmark D", "Canon Alpha", "Canon Beta", "Canon Gama",
                          "Canon Zeta", "Brother 1", "Brother 2", "Brother 3", "Brother 4", "Xerox I", "Xerox II", "Xerox III",
                          "Xerox IV", "Dell Roger", "Dell Charlie", "Dell Bravo", "Dell Tango", "HP X", "HP Y", "HP Z", "HP Plus", "Epson Stylus",
                      "Epson Pro", "Epson Office", "Epson Extra", NULL};


    for(int i = 0; printers[i] != NULL; i++) {
        MBasicListItem *printerItem = new MBasicListItem(MBasicListItem::SingleTitle);
        printerItem->setStyleName("CommonSmallPanelInverted");
        printerItem->setTitle(printers[i]);
        dialog->connect(printerItem, SIGNAL(clicked()), SLOT(accept()));
        layout->addItem(printerItem);
    }

    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void SingleSelectionDialogsPage::setDialogProgressIndicatorVisible(bool visible)
{
    if (dialog) {
        dialog->setProgressIndicatorVisible(visible);
    }
}


void SingleSelectionDialogsPage::setDialogIconVisible(bool visible)
{
    if (dialog) {
        if(visible)
            dialog->setTitleBarIconId("icon-l-default-application");
        else
            dialog->setTitleBarIconId("");
    }
}

void SingleSelectionDialogsPage::retranslateUi()
{
    //% "Single Selection Dialogs"
    setTitle(qtTrId("xx_single_selection_dialog_title"));
    if (!isContentCreated())
        return;

    QStringList singleSelectionDialogTypes;
    //% "Long Dialog"
    singleSelectionDialogTypes << qtTrId("xx_wg_query_dialogs_page_long_dialog");
    //% "System Dialog"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_system_dialog");
    //% "System Modal Dialog"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_system_modal_dialog");
    //% "Dialog with Progress Indicator"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_dialog_with_progress_indicator");
    //% "Stacked Dialogs"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_stacked_dialogs");
    //% "Dialog with Icon"
    singleSelectionDialogTypes << qtTrId("xx_wg_single_selection_dialogs_page_dialog_with_icon");

    static_cast<QStringListModel *>(list->itemModel())->setStringList(singleSelectionDialogTypes);
}

QGraphicsWidget *SingleSelectionDialogsPage::createSpacer()
{
    QGraphicsWidget *spacer = new QGraphicsWidget();
    spacer->hide();
    spacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    spacer->setMinimumSize(0, 0);
    spacer->setPreferredSize(0, 0);
    spacer->setFlag(QGraphicsItem::ItemHasNoContents, true);

    return spacer;

}
