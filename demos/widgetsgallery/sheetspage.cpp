/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "sheetspage.h"

#include <MAbstractCellCreator>
#include <MBasicListItem>
#include <MButton>
#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>
#include <MBasicSheetHeader>
#include <MSheet>
#include <MLabel>
#include <MTextEdit>
#include <MImageWidget>
#include <MMessageBox>

#include <QStringListModel>
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QDir>
#include <QPropertyAnimation>
#include <QTimer>
#include <QAction>

#include "utils.h"
#include "loginsheet.h"
#include "longsheet.h"

ClickableImageWidget::ClickableImageWidget(QGraphicsItem *parent)
    : MImageWidget(parent),
    down(false)
{

}

ClickableImageWidget::~ClickableImageWidget()
{
}

void ClickableImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    down = true;
}

void ClickableImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (down)
        emit clicked();

    down = false;
}

void ClickableImageWidget::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    down = false;
}

class SheetsPageCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    SheetsPageCellCreator() : MAbstractCellCreator<MBasicListItem>() {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MBasicListItem *cell = dynamic_cast<MBasicListItem *>(
                recycler.take(MBasicListItem::staticMetaObject.className()));

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

SheetsPage::SheetsPage()
    : TemplatePage(TemplatePage::DialogsSheetsAndBanners),
      policy(0),
      list(0)
{
}


QString SheetsPage::timedemoTitle()
{
    return "SheetsPage";
}

void SheetsPage::createContent()
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

void SheetsPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setCellCreator(new SheetsPageCellCreator());
    list->setItemModel(new QStringListModel(list));
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));
}

void SheetsPage::itemClicked(const QModelIndex &index)
{
    switch (index.row()) {
    case 0:
        openLoginSheet();
        break;
    case 1:
        openSystemwideSheet();
        break;
    case 2:
        openPhotoSheet();
        break;
    case 3:
        openLongSheet();
        break;
    default:
        break;
    }
}

void SheetsPage::retranslateUi()
{
    //% "Sheets"
    setTitle(qtTrId("xx_sheet_title"));
    if (!isContentCreated())
        return;

    QStringList sheetExamples;
    //% "Login sheet"
    sheetExamples << qtTrId("xx_wg_sheets_login_sheet");
    //% "Systemwide sheet"
    sheetExamples << qtTrId("xx_wg_sheets_systemwide_sheet");
    //% "Sheet from photo"
    sheetExamples << qtTrId("xx_wg_sheets_from_photo_sheet");
    //% "Long sheet"
    sheetExamples << qtTrId("xx_wg_sheets_long_sheet");

    static_cast<QStringListModel *>(list->itemModel())->setStringList(sheetExamples);
}

void SheetsPage::openLoginSheet()
{
    MSheet *loginSheet = new LoginSheet;
    loginSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::openSystemwideSheet()
{
    MSheet *loginSheet = new LoginSheet;
    loginSheet->appearSystemwide(MSceneWindow::DestroyWhenDone);
}

void SheetsPage::openLongSheet()
{
    MSheet *longSheet = new LongSheet;
    longSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::openPhotoSheet()
{
    photoSheet = new MSheet;

    populatePhotoSheetCentralWidget(photoSheet->centralWidget());
    populatePhotoSheetHeader(photoSheet);

    photoSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::populatePhotoSheetCentralWidget(QGraphicsWidget *centralWidget)
{
    sheetPhoto = new ClickableImageWidget(centralWidget);
    sheetPhoto->setPixmap(QPixmap(Utils::mediaArtDir() + QDir::separator() + "photo_450x450_021.jpg"));
    sheetPhoto->setGeometry(QRectF(100, 50, 300, 300));

    connect(sheetPhoto, SIGNAL(clicked()), SLOT(openFullscreenPhotoSheet()));
}

void SheetsPage::populatePhotoSheetHeader(MSheet *sheet)
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(sheet);

    //% "Cancel"
    basicHeader->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_cancel"), basicHeader));
    basicHeader->connect(basicHeader->negativeAction(), SIGNAL(triggered()), sheet, SLOT(dismiss()));

    sheet->setHeaderWidget(basicHeader);
}

void SheetsPage::openFullscreenPhotoSheet()
{
    fullScreenPhotoSheet = new MSheet;
    fullScreenPhotoSheet->setStyleName("PhotoViewer");

    populateFullscreenPhotoSheetCentralWidget(fullScreenPhotoSheet->centralWidget());
    populateFullscreenPhotoSheetHeader(fullScreenPhotoSheet);

    connect(fullScreenPhotoSheet, SIGNAL(disappearing()), this, SLOT(reparentPhotoImage()));
    fullScreenPhotoSheet->appear(scene(), MSceneWindow::DestroyWhenDone);


    QPropertyAnimation *animation = new QPropertyAnimation(sheetPhoto, "geometry", sheetPhoto);
    animation->setStartValue(sheetPhoto->geometry());
    animation->setEndValue(fullScreenPhotoSheet->centralWidget()->geometry());
    animation->setDuration(350);
    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->start();
}

void SheetsPage::populateFullscreenPhotoSheetCentralWidget(QGraphicsWidget *centralWidget)
{
    sheetPhoto->setParentItem(centralWidget);
    sheetPhoto->disconnect(this);

    connect(sheetPhoto, SIGNAL(clicked()), this, SLOT(triggerHeaderVisibility()));
}

void SheetsPage::populateFullscreenPhotoSheetHeader(MSheet *sheet)
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(sheet);
    basicHeader->setStyleName("Inverted");

    //% "Done"
    basicHeader->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_done"), basicHeader));
    //% "Save
    basicHeader->setPositiveAction(new QAction(qtTrId("xx_wg_sheets_save"), basicHeader));
    basicHeader->connect(basicHeader->negativeAction(), SIGNAL(triggered()), sheet, SLOT(dismiss()));
    basicHeader->connect(basicHeader->positiveAction(), SIGNAL(triggered()), this, SLOT(processFullscreenSave()));

    sheet->setHeaderWidget(basicHeader);
}

void SheetsPage::reparentPhotoImage()
{
    sheetPhoto->setParentItem(photoSheet->centralWidget());
    sheetPhoto->disconnect(this);
    connect(sheetPhoto, SIGNAL(clicked()), SLOT(openFullscreenPhotoSheet()));

    QPropertyAnimation *animation = new QPropertyAnimation(sheetPhoto, "geometry", sheetPhoto);
    animation->setStartValue(sheetPhoto->geometry());
    animation->setEndValue(QRectF(100, 50, 300, 300));
    animation->setDuration(350);
    animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->start();
}

void SheetsPage::triggerHeaderVisibility()
{
    fullScreenPhotoSheet->setHeaderVisible(!fullScreenPhotoSheet->isHeaderVisible());
}

void SheetsPage::processFullscreenSave()
{
    MBasicSheetHeader *header = qobject_cast<MBasicSheetHeader*>(fullScreenPhotoSheet->headerWidget());
    if (header) {
        header->setProgressIndicatorVisible(true);
        header->positiveAction()->setEnabled(false);
        QTimer::singleShot(2000, this, SLOT(processFullscreenSaveSuccess()));
    }
}

void SheetsPage::processFullscreenSaveSuccess()
{
    if (!fullScreenPhotoSheet)
        return;

    MBasicSheetHeader *header = qobject_cast<MBasicSheetHeader*>(fullScreenPhotoSheet->headerWidget());
    if (header) {
        header->setProgressIndicatorVisible(false);
    }
    MMessageBox *msg = new MMessageBox;
    //% "Save Completed!"
    msg->setTitle(qtTrId("xx_wg_sheets_save_completed"));
    //% "The photo was imported into your photo library, and is now accessible through the photo gallery viewer."
    msg->setText(qtTrId("xx_wg_sheets_save_completed_text"));

    connect(msg, SIGNAL(disappeared()), fullScreenPhotoSheet, SLOT(dismiss()));
    msg->appear(MSceneWindow::DestroyWhenDone);
}
