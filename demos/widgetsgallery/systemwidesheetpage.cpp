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

#include "systemwidesheetpage.h"
#include "labeledcheckbox.h"
#include "loginsheet.h"
#include <QGraphicsLinearLayout>
#include <MApplication>
#include <MComboBox>
#include <MMessageBox>

#ifdef HAVE_DBUS
#include "accessmanager.h"
#endif // HAVE_DBUS

SystemwideSheetPage::SystemwideSheetPage()
    : MApplicationPage()
{
}

void SystemwideSheetPage::createContent()
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical,
                                                                  centralWidget());

    if (MApplication::instance()->objectName() == "widgetsgallery") {
        setStyleName("CommonApplicationPage");
    } else {
        setStyleName("CommonApplicationPageInverted");
    }

    statusBarCheckbox = new LabeledCheckbox(this);
    //% "Status bar"
    statusBarCheckbox->label->setText(qtTrId("xx_statusbar"));
    statusBarCheckbox->button->setChecked(false);
    mainLayout->addItem(statusBarCheckbox);

    autoFocusCheckbox = new LabeledCheckbox(this);
    //% "Auto-focus on first text edit"
    autoFocusCheckbox->label->setText(qtTrId("xx_autofocus_textedit"));
    autoFocusCheckbox->button->setChecked(false);
    mainLayout->addItem(autoFocusCheckbox);

    createOrientationComboBox();
    mainLayout->addItem(orientationCombobox);

#ifdef HAVE_DBUS
    chainedCheckbox = new LabeledCheckbox(this);
    //% "Chained, via service"
    chainedCheckbox->label->setText(qtTrId("xx_chained_via_service"));
    chainedCheckbox->button->setChecked(false);
    mainLayout->addItem(chainedCheckbox);
#endif // HAVE_DBUS

    MButton *button = new MButton(this);
    //% "Open systemwide sheet"
    button->setText(qtTrId("xx_open_systemwide_sheet"));
    connect(button, SIGNAL(clicked()), SLOT(openSystemwideSheet()));
    mainLayout->addItem(button);
}

void SystemwideSheetPage::createOrientationComboBox()
{
    orientationCombobox = new MComboBox;
    orientationCombobox->setObjectName("orientationCombobox");
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        orientationCombobox->setStyleName("CommonComboBox");
    } else {
        orientationCombobox->setStyleName("CommonComboBoxInverted");
    }

    //%  "Orientation mode"
    orientationCombobox->setTitle(qtTrId("xx_systemwidesheet_orientation"));

    orientationCombobox->setIconVisible(false);

    //% "Follow device"
    orientationCombobox->insertItem(MSheet::FollowsDeviceOrientation,
        qtTrId("xx_followsdeviceorientation"));

    //% "Follows current app"
    orientationCombobox->insertItem(MSheet::FollowsCurrentAppWindowOrientation,
        qtTrId("xx_followscurrentappwindoworientation"));

    //% "Locked to portrait"
    orientationCombobox->insertItem(MSheet::LockedToPortraitOrientation,
        qtTrId("xx_lockedtoportraitorientation"));

    //% "Locked to landscape"
    orientationCombobox->insertItem(MSheet::LockedToLandscapeOrientation,
        qtTrId("xx_lockedtolandscapeorientation"));

    orientationCombobox->setCurrentIndex(0);
}

void SystemwideSheetPage::openSystemwideSheet()
{
#ifdef HAVE_DBUS
    if (chainedCheckbox->button->isChecked()) {
        openSystemwideSheetViaService();
        return;
    }
#endif // HAVE_DBUS

    LoginSheet *loginSheet = new LoginSheet;
    loginSheet->setObjectName("loginSheet");

    if (MApplication::instance()->objectName() == "widgetsgallery") {
        loginSheet->setStyleName("");
    } else {
        loginSheet->setStyleName("Inverted");
    }

    loginSheet->setStatusBarVisibleInSystemwide(
                statusBarCheckbox->button->isChecked());

    loginSheet->setAutoFocusOnFirstTextEditEnabled(
                autoFocusCheckbox->button->isChecked());

    loginSheet->setSystemwideModeOrientation(
        static_cast<MSheet::SystemwideModeOrientation>(
                    orientationCombobox->currentIndex()));

    loginSheet->appearSystemwide(MSceneWindow::DestroyWhenDone);
}

#ifdef HAVE_DBUS
void SystemwideSheetPage::openSystemwideSheetViaService()
{
    AccessManager manager("com.nokia.widgetsgallery.AccessManager");

    if (!manager.isValid()) {
        qWarning() << "Cannot connect to service";
        return;
    }

    bool res = manager.login(
                !statusBarCheckbox->button->isChecked(),
                autoFocusCheckbox->button->isChecked(),
                orientationCombobox->currentIndex()
                );

    if (res) {
        MMessageBox* messageBox = new MMessageBox;
        messageBox->setObjectName("messageBox");

        //% "Access Granted!"
        messageBox->setTitle(qtTrId("xx_wg_sheets_connection_success"));
        //% "You are now able to use the requested services."
        messageBox->setText(qtTrId("xx_wg_sheets_connection_success_text"));

        messageBox->appear(MSceneWindow::DestroyWhenDone);
    }
}
#endif // HAVE_DBUS
