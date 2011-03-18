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

#include "longsheet.h"

#include <MBasicSheetHeader>
#include <MLabel>
#include <MMessageBox>
#include <MTextEdit>
#include <MSlider>
#include <MPannableViewport>

#include <QAction>
#include <QGraphicsLinearLayout>

LongSheet::LongSheet()
{
    fakeAuthenticationTimer.setSingleShot(true);
    fakeAuthenticationTimer.setInterval(2000);
    connect(&fakeAuthenticationTimer, SIGNAL(timeout()), SLOT(showLoginSuccessfulAndDismiss()));

    createCentralWidget();
    createHeaderWidget();
}

void LongSheet::createCentralWidget()
{
    MPannableViewport *pannableViewport = new MPannableViewport;

    QGraphicsWidget *contentWidget = new QGraphicsWidget;
    pannableViewport->setWidget(contentWidget);
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical,
                                                                  contentWidget);
    setCentralWidget(pannableViewport);

    mainLayout->setContentsMargins(10.0f, 0.0f, 10.0f, 10.0f);
    mainLayout->setSpacing(0);

    //% "Connect to Service"
    MLabel *label = new MLabel(qtTrId("xx_wg_sheets_connect_service"));
    label->setStyleName("CommonTitle");
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addItem(label);

    mainLayout->addItem(createSpacer());

    //% "Username:"
    label = new MLabel(qtTrId("xx_wg_sheets_username"));
    label->setStyleName("CommonFieldLabel");

    mainLayout->addItem(label);

    MTextEdit *textEdit = new MTextEdit;
    textEdit->setStyleName("CommonSingleInputFieldLabeled");
    mainLayout->addItem(textEdit);

    mainLayout->addItem(createSpacer());

    //% "Password:"
    label = new MLabel(qtTrId("xx_wg_sheets_password"));
    label->setStyleName("CommonFieldLabel");
    mainLayout->addItem(label);

    textEdit = new MTextEdit;
    textEdit->setStyleName("CommonSingleInputFieldLabeled");
    textEdit->setEchoMode(MTextEditModel::Password);
    mainLayout->addItem(textEdit);

    //% "This sheet is so long that it needs a pannable viewport to make all its "
    //% "content available."
    label = new MLabel(qtTrId("xx_wg_sheets_long_notice"));
    label->setStyleName("CommonBodyText");
    label->setWordWrap(true);
    mainLayout->addItem(label);

    mainLayout->addItem(createSpacer());

    // OBS: No point in translating dummy text.
    label = new MLabel("Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim "
        "ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip "
        "ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
        "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat "
        "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id "
        "est laborum.");
    label->setStyleName("CommonBodyText");
    label->setWordWrap(true);
    mainLayout->addItem(label);

    mainLayout->addItem(createSpacer());

    MSlider *slider = new MSlider;
    slider->setStyleName("CommonSlider");
    slider->setRange(0, 100);
    slider->setMinLabelVisible(true);
    slider->setMaxLabelVisible(true);
    slider->setHandleLabelVisible(true);
    mainLayout->addItem(slider);

    mainLayout->addItem(createSpacer());

    // OBS: No point in translating dummy text.
    label = new MLabel("Foobar:");
    label->setStyleName("CommonFieldLabel");
    mainLayout->addItem(label);
    textEdit = new MTextEdit;
    textEdit->setStyleName("CommonSingleInputFieldLabeled");
    mainLayout->addItem(textEdit);
}

void LongSheet::createHeaderWidget()
{
    MBasicSheetHeader *basicHeader = new MBasicSheetHeader(this);

    //% "Cancel"
    basicHeader->setNegativeAction(new QAction(qtTrId("xx_wg_sheets_cancel"), basicHeader));
    //% "Connect"
    basicHeader->setPositiveAction(new QAction(qtTrId("xx_wg_sheets_connect"), basicHeader));

    connect(basicHeader->negativeAction(), SIGNAL(triggered()), SLOT(cancel()));
    connect(basicHeader->positiveAction(), SIGNAL(triggered()), SLOT(login()));

    setHeaderWidget(basicHeader);
}

void LongSheet::login()
{
    MBasicSheetHeader *header = static_cast<MBasicSheetHeader*>(headerWidget());

    header->setProgressIndicatorVisible(true);
    header->positiveAction()->setEnabled(false);
    centralWidget()->setEnabled(false);

    // fake that we're waiting for authentication via network
    Q_ASSERT(!fakeAuthenticationTimer.isActive());
    fakeAuthenticationTimer.start();
}

void LongSheet::cancel()
{
    if (fakeAuthenticationTimer.isActive()) {
        fakeAuthenticationTimer.stop();
    }
    dismiss();
}

void LongSheet::showLoginSuccessfulAndDismiss()
{
    MBasicSheetHeader *header = static_cast<MBasicSheetHeader*>(headerWidget());

    header->setProgressIndicatorVisible(false);

    MMessageBox *messageBox = new MMessageBox;

    //% "Access Granted!"
    messageBox->setTitle(qtTrId("xx_wg_sheets_connection_success"));
    //% "You are now able to use the requested services."
    messageBox->setText(qtTrId("xx_wg_sheets_connection_success_text"));

    connect(messageBox, SIGNAL(disappeared()), SLOT(dismiss()));
    messageBox->appear(MSceneWindow::DestroyWhenDone);
}

QGraphicsWidget *LongSheet::createSpacer() const
{
    QGraphicsWidget *spacer = new QGraphicsWidget;
    spacer->setFlag(QGraphicsItem::ItemHasNoContents);
    spacer->setPreferredHeight(24.0);
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    return spacer;
}
