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

#include "listsheet.h"

#include <QAction>
#include <QGraphicsLinearLayout>

#include <MAbstractCellCreator>
#include <MBasicSheetHeader>
#include <MList>
#include <MPannableViewport>

#include "phonebookmodel.h"
#include "phonebookcell.h"

class MListSheetContentItemCreator : public MAbstractCellCreator<PhoneBookCell>
{
public:
    MListSheetContentItemCreator() { }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
    {
        PhoneBookCell *cell = dynamic_cast<PhoneBookCell *>(recycler.take(PhoneBookCell::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new PhoneBookCell;
        }
        updateCell(index, cell);
        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        PhoneBookCell *listCell = qobject_cast<PhoneBookCell*>(cell);

        QVariant data = index.data(Qt::DisplayRole);
        PhoneBookEntry *entry = static_cast<PhoneBookEntry *>(data.value<void *>());

        listCell->setTitle(entry->fullName);
        listCell->setSubtitle(entry->phoneNumber);
        listCell->setImage(entry->thumbnail);
    }
};

ListSheet::ListSheet()
    : MSheet()
{
    createHeaderWidget();
    createCentralWidget();
}

ListSheet::~ListSheet()
{

}

void ListSheet::createHeaderWidget()
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(this);

    //% "Cancel"
    basicHeader->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_cancel"), basicHeader));

    connect(basicHeader->negativeAction(), SIGNAL(triggered()), SLOT(dismiss()));

    setHeaderWidget(basicHeader);
}

void ListSheet::createCentralWidget()
{
    MPannableViewport *pannableViewport = new MPannableViewport(this);
    setCentralWidget(pannableViewport);

    PhoneBookModel *model = new PhoneBookModel;
    model->clear();
    model->insertRows(0, 1000);
    model->setGrouped(true);

    MList *list = new MList(pannableViewport);
    list->setShowGroups(true);
    list->setItemModel(model);
    list->setCellCreator(new MListSheetContentItemCreator);
    list->setIndexDisplayMode(MList::Floating);

    model->setParent(list);
    pannableViewport->setWidget(list);
}
