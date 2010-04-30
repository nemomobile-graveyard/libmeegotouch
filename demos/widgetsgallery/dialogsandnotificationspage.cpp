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

#include "dialogsandnotificationspage.h"

#include "listpage.h"
#include <QTimer>
#include <MDialog>
#include <MLayout>
#include <MLocale>
#include <MButton>
#include <MMessageBox>
#include <MInfoBanner>
#include <MLabel>
#include <MTextEdit>
#include <QDebug>
#include <MContainer>
#include <MLinearLayoutPolicy>
#include <MWidget>
#include <QGraphicsLinearLayout>

DialogsAndNotificationsPage::DialogsAndNotificationsPage()
    : TemplatePage(),
      dialog(0),
      nestedDialog(0),
      nestedMessageBox(0),
      label1(0),
      label2(0),
      label3(0),
      button1(0),
      button2(0),
      button3(0),
      button4(0),
      button5(0),
      button6(0),
      button7(0),
      buttonStackedDialogs(0),
      buttonSystemModalDialog(0)
{
    gid = TemplatePage::ViewsAndDialogs;
}

DialogsAndNotificationsPage::~DialogsAndNotificationsPage()
{
}

QString DialogsAndNotificationsPage::timedemoTitle()
{
    return "DialogsAndNotifications";
}

void DialogsAndNotificationsPage::createContent()
{
    MApplicationPage::createContent();

    MLayout *layout = new MLayout(centralWidget());
    MLinearLayoutPolicy *layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    populateLayout(layoutPolicy);

    retranslateUi();
}

void DialogsAndNotificationsPage::openQuestionDialog()
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

void DialogsAndNotificationsPage::openEntryDialog()
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

void DialogsAndNotificationsPage::openLongDialog()
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

void DialogsAndNotificationsPage::openStackedDialogs()
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

void DialogsAndNotificationsPage::openNestedDialog()
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

void DialogsAndNotificationsPage::openNestedMessageBox()
{
    if (nestedMessageBox)
        return;

    //% "I'm a nested message box"
    nestedMessageBox = new MMessageBox(qtTrId("xx_dialogs_and_notifications_stacked_dialog_messagebox_text"));
    nestedMessageBox->appear(MSceneWindow::DestroyWhenDone);
}

void DialogsAndNotificationsPage::openSystemModalDialog()
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

void DialogsAndNotificationsPage::openDialogWithProgressIndicator()
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

void DialogsAndNotificationsPage::openMessageBox()
{
    if (dialog)
        return;

    //% "Hello World!"
    dialog = new MMessageBox(qtTrId("xx_dialogs_and_notifications_message_box_text"), M::OkButton);
    dialog->appear(MSceneWindow::DestroyWhenDone);
}

void DialogsAndNotificationsPage::showEventBanner()
{
    MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Event);
    infoBanner->setImageID("icon-m-content-email");
    infoBanner->setBodyText(
        //% "<b>Ida Taipale</b><br/>Have you seen my dog?"
        qtTrId("xx_dialogs_and_notifications_event_banner"));
    infoBanner->setIconID("Icon-new-SMS");
    connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
    infoBanner->appear(MSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}

void DialogsAndNotificationsPage::showInformationBanner()
{
    MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Information);
    infoBanner->setImageID("icon-m-startup-help");
    infoBanner->setBodyText(
        //% "<b>Battery is running low</b>"
        qtTrId("xx_dialogs_and_notifications_information_banner"));
    infoBanner->appear(MSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}

void DialogsAndNotificationsPage::showSystemInformationBanner()
{
    MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Information);
    infoBanner->setImageID("icon-m-telephony-call-answer");
    infoBanner->setBodyText(
        //% "<b>Incoming call</b>"
        qtTrId("xx_dialogs_and_notifications_system_information_banner"));
    //% "Accept"
    infoBanner->setButtonText(qtTrId("xx_dialogs_and_notifications_system_information_banner_accept"));
    connect(infoBanner, SIGNAL(buttonClicked()), this, SLOT(openMessageBox()));
    infoBanner->appear(MSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}



void DialogsAndNotificationsPage::populateLayout(MLinearLayoutPolicy *layoutPolicy)
{
    // Dialogs

    dialogsContainer = new MContainer();
    QGraphicsLinearLayout *dialogsLayout = new QGraphicsLinearLayout(Qt::Vertical, dialogsContainer->centralWidget());

    layoutPolicy->addItem(dialogsContainer);



    button1 = new MButton(centralWidget());
    connect(button1, SIGNAL(clicked()), this, SLOT(openQuestionDialog()));
    dialogsLayout->addItem(button1);

    button2 = new MButton(centralWidget());
    connect(button2, SIGNAL(clicked()), this, SLOT(openEntryDialog()));
    dialogsLayout->addItem(button2);

    button3 = new MButton(centralWidget());
    connect(button3, SIGNAL(clicked()), this, SLOT(openLongDialog()));
    dialogsLayout->addItem(button3);

    buttonStackedDialogs = new MButton(centralWidget());
    connect(buttonStackedDialogs, SIGNAL(clicked()), this, SLOT(openStackedDialogs()));
    dialogsLayout->addItem(buttonStackedDialogs);

    buttonSystemModalDialog = new MButton(centralWidget());
    connect(buttonSystemModalDialog, SIGNAL(clicked()), this, SLOT(openSystemModalDialog()));
    dialogsLayout->addItem(buttonSystemModalDialog);

    buttonDialogWithProgressIndicator = new MButton(centralWidget());
    connect(buttonDialogWithProgressIndicator, SIGNAL(clicked()), this, SLOT(openDialogWithProgressIndicator()));
    dialogsLayout->addItem(buttonDialogWithProgressIndicator);

    button4 = new MButton(centralWidget());
    connect(button4, SIGNAL(clicked()), this, SLOT(openMessageBox()));
    dialogsLayout->addItem(button4);

    // Notifications

    notificationsContainer = new MContainer();
    QGraphicsLinearLayout *notificationsLayout = new QGraphicsLinearLayout(Qt::Vertical, notificationsContainer->centralWidget());

    layoutPolicy->addItem(notificationsContainer);

    button5 = new MButton(centralWidget());
    connect(button5, SIGNAL(clicked()), this, SLOT(showEventBanner()));
    notificationsLayout->addItem(button5);

    button6 = new MButton(centralWidget());
    connect(button6, SIGNAL(clicked()), this, SLOT(showInformationBanner()));
    notificationsLayout->addItem(button6);

    button7 = new MButton(centralWidget());
    button7->connect(button7, SIGNAL(clicked()), this, SLOT(showSystemInformationBanner()));
    notificationsLayout->addItem(button7);


    //notificationsLayout->addItem(button8);
}

void DialogsAndNotificationsPage::retranslateUi()
{
    //% "Dialogs and Notifications"
    setTitle(qtTrId("xx_dialogs_and_notifications_title"));
    if (!isContentCreated())
        return;
    //% "Dialog examples"
    dialogsContainer->setTitle(qtTrId("xx_dialogs_and_notifications_label_dialogs"));
    //% "Question Dialog"
    button1->setText(qtTrId("xx_dialogs_and_notifications_query_dialog"));
    //% "Entry Dialog"
    button2->setText(qtTrId("xx_dialogs_and_notifications_entry_dialog"));
    //% "A Long Dialog"
    button3->setText(qtTrId("xx_dialogs_and_notifications_long_dialog"));

    //% "Stacked Dialogs"
    buttonStackedDialogs->setText(qtTrId("xx_dialogs_and_notifications_stacked_dialogs"));

    //% "System Modal Dialog"
    buttonSystemModalDialog->setText(qtTrId("xx_dialogs_and_notifications_system_modal_dialog"));

    //% "Dialog With Progress Indicator"
    buttonDialogWithProgressIndicator->setText(qtTrId("xx_dialogs_and_notifications_dialog_with_progress_indicator"));

    //% "Notifications and messages"
    notificationsContainer->setTitle(qtTrId("xx_dialogs_and_notifications_label_notifications"));
    //% "Simple Message Box"
    button4->setText(qtTrId("xx_dialogs_and_notifications_messagebox"));
    //% "Event Banner"
    button5->setText(qtTrId("xx_dialogs_and_notifications_label_event_banner"));
    //% "Information Banner"
    button6->setText(qtTrId("xx_dialogs_and_notifications_label_information_banner"));
    //% "System Information Banner"
    button7->setText(qtTrId("xx_dialogs_and_notifications_label_system_information_banner"));

}

void DialogsAndNotificationsPage::setDialogProgressIndicatorVisible(bool visible)
{
    if (dialog) {
        dialog->setProgressIndicatorVisible(visible);
    }
}
