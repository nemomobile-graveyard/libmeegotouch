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

#include "multipleselectiondialogspage.h"

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

class MultipleSelectionDialogsPageCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    MultipleSelectionDialogsPageCellCreator() : MAbstractCellCreator<MBasicListItem>() {
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

MultipleSelectionDialogsPage::MultipleSelectionDialogsPage()
    : TemplatePage(TemplatePage::DialogsAndBanners)
{
}

QString MultipleSelectionDialogsPage::timedemoTitle()
{
    return "MultipleSelectionDialogsPage";
}

void MultipleSelectionDialogsPage::createContent()
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

void MultipleSelectionDialogsPage::populateLayout()
{
    QStringList multipleSelectionDialogTypes;
    //% "Question Dialog"
    multipleSelectionDialogTypes << qtTrId("xx_wg_multiple_selection_dialogs_page_question_dialog");

    list = new MList(centralWidget());
    list->setObjectName("wgList");
    list->setCellCreator(new MultipleSelectionDialogsPageCellCreator());
    list->setItemModel(new QStringListModel(multipleSelectionDialogTypes));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void MultipleSelectionDialogsPage::itemClicked(const QModelIndex &index)
{
    if (index.row() == 0)
        openQuestionDialog();
}

void MultipleSelectionDialogsPage::openQuestionDialog()
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

