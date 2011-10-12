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

#include <MApplication>
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
#include <MPannableViewport>

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
#include "listsheet.h"
#include "systemwidesheetpage.h"
#include "drilldownlistitem.h"
#include "transparentheadersheet.h"

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

SheetsPage::SheetsPage()
    : TemplatePage(TemplatePage::DialogsSheetsAndBanners),
      photoSheetFullScreenTransition(0)
{
}


QString SheetsPage::timedemoTitle()
{
    return "SheetsPage";
}

void SheetsPage::createContent()
{
    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    setStyleName(inv("CommonApplicationPage"));

    QGraphicsWidget *panel = centralWidget();

    mainLayout = new QGraphicsLinearLayout(Qt::Vertical, panel);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    panel->setLayout(mainLayout);

    populateLayout();

    retranslateUi();
}

MBasicListItem *SheetsPage::createListItemAndAddToLayout()
{
    MBasicListItem *listItem = new MBasicListItem(MBasicListItem::SingleTitle, centralWidget());
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        listItem->setStyleName("CommonBasicListItem");
    } else {
        listItem->setStyleName("CommonBasicListItemInverted");
    }
    listItem->setLayoutPosition(M::CenterPosition);
    mainLayout->addItem(listItem);
    return listItem;
}

void SheetsPage::populateLayout()
{
    bool ok;

    loginSheetItem = createListItemAndAddToLayout();
    ok = connect(loginSheetItem, SIGNAL(clicked()), SLOT(openLoginSheet()));
    if (!ok) qFatal("signal connection failed");

    systemWideItem = new DrillDownListItem(centralWidget());
    mainLayout->addItem(systemWideItem);
    ok = connect(systemWideItem, SIGNAL(clicked()), SLOT(showSystemwideSheetPage()));
    if (!ok) qFatal("signal connection failed");

    sheetFromPhotoItem = createListItemAndAddToLayout();
    ok = connect(sheetFromPhotoItem, SIGNAL(clicked()), SLOT(openPhotoSheet()));
    if (!ok) qFatal("signal connection failed");

    longSheetItem = createListItemAndAddToLayout();
    ok = connect(longSheetItem, SIGNAL(clicked()), SLOT(openLongSheet()));
    if (!ok) qFatal("signal connection failed");

    listSheetItem = createListItemAndAddToLayout();
    ok = connect(listSheetItem, SIGNAL(clicked()), SLOT(openListSheet()));
    if (!ok) qFatal("signal connection failed");

    transparentHeaderSheetItem = createListItemAndAddToLayout();
    ok = connect(transparentHeaderSheetItem, SIGNAL(clicked()), SLOT(openTransparentHeaderSheet()));
    if (!ok) qFatal("signal connection failed");
}

void SheetsPage::retranslateUi()
{
    //% "Sheets"
    setTitle(qtTrId("xx_sheet_title"));
    if (!isContentCreated())
        return;

    //% "Login sheet"
    loginSheetItem->setTitle(qtTrId("xx_wg_sheets_login_sheet"));
    //% "Systemwide sheet"
    systemWideItem->setTitle(qtTrId("xx_wg_sheets_systemwide_sheet"));
    //% "Sheet from photo"
    sheetFromPhotoItem->setTitle(qtTrId("xx_wg_sheets_from_photo_sheet"));
    //% "Long sheet"
    longSheetItem->setTitle(qtTrId("xx_wg_sheets_long_sheet"));
    //% "List sheet"
    listSheetItem->setTitle(qtTrId("xx_wg_sheets_list_sheet"));
    //% "Sheet with transparent header"
    transparentHeaderSheetItem->setTitle(qtTrId("xx_wg_sheets_sheet_with_transparent_header"));
}

void SheetsPage::openLoginSheet()
{
    LoginSheet *loginSheet = new LoginSheet;
    loginSheet->setAutoFocusOnFirstTextEditEnabled(true);
    loginSheet->setObjectName("loginSheet");
    loginSheet->setStyleName(inv(""));
    loginSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::showSystemwideSheetPage()
{
    MApplicationPage *page = new SystemwideSheetPage;
    page->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::openLongSheet()
{
    MSheet *longSheet = new LongSheet;
    longSheet->setObjectName("longSheet");
    longSheet->setStyleName(inv(""));
    longSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::openListSheet()
{
    MSheet *listSheet = new ListSheet;
    listSheet->setObjectName("listSheet");
    listSheet->setStyleName(inv(""));
    listSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::openPhotoSheet()
{
    photoSheet = new MSheet;
    photoSheet->setObjectName("photoSheet");
    photoSheet->setStyleName(inv(""));

    populatePhotoSheetCentralWidget(photoSheet->centralWidget());
    populatePhotoSheetHeader(photoSheet);

    photoSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::openTransparentHeaderSheet()
{
    MSheet *transparentHeaderSheet = new TransparentHeaderSheet;
    transparentHeaderSheet->setObjectName("transparentHeaderSheet");
    transparentHeaderSheet->setStyleName("Overlay");
    transparentHeaderSheet->appear(scene(), MSceneWindow::DestroyWhenDone);
}

void SheetsPage::populatePhotoSheetCentralWidget(QGraphicsWidget *centralWidget)
{
    sheetPhoto = new ClickableImageWidget(centralWidget);
    sheetPhoto->setObjectName("sheetPhoto");
    sheetPhoto->setPixmap(QPixmap(Utils::mediaArtDir() + QDir::separator() + "photo_450x450_021.jpg"));
    sheetPhoto->setGeometry(QRectF(100, 50, 300, 300));

    connect(sheetPhoto, SIGNAL(clicked()), SLOT(openFullscreenPhotoSheet()));
}

void SheetsPage::populatePhotoSheetHeader(MSheet *sheet)
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(sheet);
    basicHeader->setObjectName("basicHeader");
    basicHeader->setStyleName(inv(""));

    //% "Cancel"
    basicHeader->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_cancel"), basicHeader));
    basicHeader->connect(basicHeader->negativeAction(), SIGNAL(triggered()), sheet, SLOT(dismiss()));

    sheet->setHeaderWidget(basicHeader);
}

void SheetsPage::openFullscreenPhotoSheet()
{
    fullScreenPhotoSheet = new MSheet;
    fullScreenPhotoSheet->setObjectName("fullScreenPhotoSheet");
    fullScreenPhotoSheet->setStyleName("PhotoViewer");

    populateFullscreenPhotoSheetCentralWidget(fullScreenPhotoSheet->centralWidget());
    populateFullscreenPhotoSheetHeader(fullScreenPhotoSheet);

    connect(fullScreenPhotoSheet, SIGNAL(disappearing()), this, SLOT(reparentPhotoImage()));
    fullScreenPhotoSheet->appear(scene(), MSceneWindow::DestroyWhenDone);

    if (!photoSheetFullScreenTransition)
        photoSheetFullScreenTransition = new QPropertyAnimation(sheetPhoto, "geometry", sheetPhoto);

    photoSheetFullScreenTransition->setStartValue(sheetPhoto->geometry());
    photoSheetFullScreenTransition->setEndValue(fullScreenPhotoSheet->centralWidget()->geometry());
    photoSheetFullScreenTransition->setDuration(350);
    photoSheetFullScreenTransition->setEasingCurve(QEasingCurve::InOutExpo);
    photoSheetFullScreenTransition->start();

    connect(fullScreenPhotoSheet->centralWidget(), SIGNAL(widthChanged()), this, SLOT(updateFullscreenPhotoSheetGeometry()));
    connect(fullScreenPhotoSheet->centralWidget(), SIGNAL(heightChanged()), this, SLOT(updateFullscreenPhotoSheetGeometry()));
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
    basicHeader->setObjectName("basicHeader");
    basicHeader->setStyleName("Inverted");

    //% "Done"
    basicHeader->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_done"), basicHeader));
    //% "Save"
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

    disconnect(fullScreenPhotoSheet->centralWidget(), SIGNAL(widthChanged()), this, SLOT(updateFullscreenPhotoSheetGeometry()));
    disconnect(fullScreenPhotoSheet->centralWidget(), SIGNAL(heightChanged()), this, SLOT(updateFullscreenPhotoSheetGeometry()));

    photoSheetFullScreenTransition->setStartValue(sheetPhoto->geometry());
    photoSheetFullScreenTransition->setEndValue(QRectF(100, 50, 300, 300));
    photoSheetFullScreenTransition->start();
}

void SheetsPage::updateFullscreenPhotoSheetGeometry()
{
    if (photoSheetFullScreenTransition && photoSheetFullScreenTransition->state() == QPropertyAnimation::Running)
        photoSheetFullScreenTransition->setEndValue(fullScreenPhotoSheet->centralWidget()->geometry());
    else
        sheetPhoto->setGeometry(fullScreenPhotoSheet->centralWidget()->geometry());
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
    msg->setObjectName("messageBox");
    //% "Save Completed!"
    msg->setTitle(qtTrId("xx_wg_sheets_save_completed"));
    //% "The photo was imported into your photo library, and is now accessible through the photo gallery viewer."
    msg->setText(qtTrId("xx_wg_sheets_save_completed_text"));

    connect(msg, SIGNAL(disappeared()), fullScreenPhotoSheet, SLOT(dismiss()));
    msg->appear(MSceneWindow::DestroyWhenDone);
}
