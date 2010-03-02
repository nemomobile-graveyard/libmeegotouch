/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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
#include <DuiDialog>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiButton>
#include <DuiMessageBox>
#include <DuiInfoBanner>
#include <DuiLabel>
#include <DuiTextEdit>
#include <QDebug>
#include <DuiContainer>
#include <DuiLinearLayoutPolicy>

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
      buttonWindowModalDialog(0)
{
    gid = TemplatePage::ViewsAndDialogs;
}

DialogsAndNotificationsPage::~DialogsAndNotificationsPage()
{
    delete dialog;
    delete nestedDialog;
    delete nestedMessageBox;
}

QString DialogsAndNotificationsPage::timedemoTitle()
{
    return "DialogsAndNotifications";
}

void DialogsAndNotificationsPage::createContent()
{
    DuiApplicationPage::createContent();

    DuiLayout *layout = new DuiLayout(centralWidget());
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    populateLayout(layoutPolicy);

    retranslateUi();
}

void DialogsAndNotificationsPage::openQuestionDialog()
{
    if (dialog != NULL)
        delete dialog;
    dialog = new DuiDialog(
        //%  "Question Dialog Title"
        qtTrId("xx_dialogs_and_notifications_question_dialog_title"),
        //% "Lorem ipsum dolor sit amet?"
        new DuiLabel(qtTrId("xx_dialogs_and_notifications_question_dialog_content")),
        Dui::YesButton | Dui::NoButton);
    dialog->exec();
}

void DialogsAndNotificationsPage::openEntryDialog()
{
    DuiWidget *centralWidget = new DuiWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    //% "Name"
    DuiLabel *label = new DuiLabel(qtTrId("xx_dialogs_and_notifications_entry_dialog_label"), centralWidget);
    DuiTextEdit *textEdit = new DuiTextEdit(DuiTextEditModel::SingleLine,
                                            QString(),
                                            centralWidget);
    centralWidget->setLayout(layout);

    layout->addItem(label);
    layout->addItem(textEdit);

    if (dialog != NULL)
        delete dialog;
    //% "Please enter your name"
    dialog = new DuiDialog(qtTrId("xx_dialogs_and_notifications_entry_dialog_title"),
                           centralWidget,
                           Dui::OkButton | Dui::ResetButton);

    dialog->exec();
}

void DialogsAndNotificationsPage::openLongDialog()
{
    DuiWidget *centralWidget = new DuiWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);

    if (dialog != NULL)
        delete dialog;
    //% "Select printer"
    dialog = new DuiDialog(qtTrId("xx_dialogs_and_notifications_long_dialog_title"), centralWidget, Dui::CancelButton);
    DuiButton *button = 0;

    centralWidget->setLayout(layout);

#define ADD_PRINTER_BUTTON(NAME) \
    button = new DuiButton(NAME, centralWidget); \
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

    dialog->exec();
}

void DialogsAndNotificationsPage::openStackedDialogs()
{
    if (dialog != NULL)
        delete dialog;

    //% "Click to spawn a nested dialog"
    DuiButton *button = new DuiButton(qtTrId("xx_dialogs_and_notifications_stacked_dialog_button"));
    //% "Stacked dialogs"
    dialog = new DuiDialog(qtTrId("xx_dialogs_and_notifications_stacked_dialog_title"), button, Dui::CancelButton);

    connect(button, SIGNAL(clicked()), SLOT(openNestedDialog()));
    dialog->exec();
}

void DialogsAndNotificationsPage::openNestedDialog()
{
    if (nestedDialog != NULL)
        delete nestedDialog;

    //% "Click to open a nested message box"
    DuiButton *button = new DuiButton(qtTrId("xx_dialogs_and_notifications_stacked_dialog_open_nested_messagebox"));
    //% "This is a nested dialog"
    nestedDialog = new DuiDialog(qtTrId("xx_dialogs_and_notifications_stacked_dialog_nested_dialog_title"), button, Dui::CancelButton);
    connect(button, SIGNAL(clicked()), SLOT(openNestedMessageBox()));

    nestedDialog->exec();
}

void DialogsAndNotificationsPage::openNestedMessageBox()
{
    if (nestedMessageBox != NULL)
        delete nestedMessageBox;

    //% "I'm a nested message box"
    nestedMessageBox = new DuiMessageBox(qtTrId("xx_dialogs_and_notifications_stacked_dialog_messagebox_text"));
    nestedMessageBox->exec();
}

void DialogsAndNotificationsPage::openWindowModalDialog()
{
    if (dialog != NULL)
        delete dialog;
    dialog = new DuiDialog(
        //% "Window Modal Dialog"
        qtTrId("xx_dialogs_and_notifications_window_modal_dialog_title"),
        //% "I'm a window modal dialog.\n"
        //% "There's no way around me!\n"
        //% "Muwhahaha... [evil laugh]"
        new DuiLabel(qtTrId("xx_dialogs_and_notifications_window_modal_dialog_label")),
        Dui::OkButton);
    dialog->setWindowModal(true);
    dialog->exec();
}

void DialogsAndNotificationsPage::openDialogWithProgressIndicator()
{
    if (dialog != NULL)
        delete dialog;

    DuiButton *button = new DuiButton();
    button->setViewType(DuiButton::switchType);
    button->setCheckable(true);
    button->setChecked(true);
    connect(button, SIGNAL(toggled(bool)), this, SLOT(setDialogProgressIndicatorVisible(bool)));

    //% "Progress Indicator"
    DuiLabel *label = new DuiLabel(qtTrId("xx_dialogs_and_notifications_progress_indicator"));

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->addStretch();
    layout->addItem(label);
    layout->addItem(button);
    layout->addStretch();

    dialog = new DuiDialog("Lorem ipsum", Dui::NoStandardButton);
    dialog->centralWidget()->setLayout(layout);
    dialog->setProgressIndicatorVisible(true);
    dialog->exec();
}

void DialogsAndNotificationsPage::openMessageBox()
{
    if (dialog != NULL)
        delete dialog;
    //% "Hello World!"
    dialog = new DuiMessageBox(qtTrId("xx_dialogs_and_notifications_message_box_text"), Dui::OkButton);
    dialog->exec();
}

void DialogsAndNotificationsPage::showEventBanner()
{
    DuiInfoBanner *infoBanner = new DuiInfoBanner(DuiInfoBanner::Event);
    infoBanner->setImageID("ebf7db7b6ab73c6cba8fc87da2d1fdcf");
    infoBanner->setBodyText(
        //% "<b>Ida Taipale</b><br/>Have you seen my dog?"
        qtTrId("xx_dialogs_and_notifications_event_banner"));
    infoBanner->setIconID("Icon-new-SMS");
    connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
    infoBanner->appear(DuiSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}

void DialogsAndNotificationsPage::showInformationBanner()
{
    DuiInfoBanner *infoBanner = new DuiInfoBanner(DuiInfoBanner::Information);
    infoBanner->setImageID("Icon-close");
    infoBanner->setBodyText(
        //% "<b>Battery is running low</b>"
        qtTrId("xx_dialogs_and_notifications_information_banner"));
    connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
    infoBanner->appear(DuiSceneWindow::DestroyWhenDone);
    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}

void DialogsAndNotificationsPage::showSystemInformationBanner()
{
    DuiInfoBanner *infoBanner = new DuiInfoBanner(DuiInfoBanner::Information);
    infoBanner->setImageID("icon-l-chat");
    infoBanner->setBodyText(
        //% "<b>Incoming call</b>"
        qtTrId("xx_dialogs_and_notifications_system_information_banner"));
    //% "Accept"
    infoBanner->setButtonText(qtTrId("xx_dialogs_and_notifications_system_information_banner_accept"));
    connect(infoBanner, SIGNAL(buttonClicked()), this, SLOT(openMessageBox()));
    infoBanner->appear(DuiSceneWindow::DestroyWhenDone);

    QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
}

void DialogsAndNotificationsPage::populateLayout(DuiLinearLayoutPolicy *layoutPolicy)
{
    // Dialogs

    dialogsContainer = new DuiContainer();
    QGraphicsLinearLayout *dialogsLayout = new QGraphicsLinearLayout(Qt::Vertical, dialogsContainer->centralWidget());

    layoutPolicy->addItem(dialogsContainer);

    button1 = new DuiButton(centralWidget());
    connect(button1, SIGNAL(clicked()), this, SLOT(openQuestionDialog()));
    dialogsLayout->addItem(button1);

    button2 = new DuiButton(centralWidget());
    connect(button2, SIGNAL(clicked()), this, SLOT(openEntryDialog()));
    dialogsLayout->addItem(button2);

    button3 = new DuiButton(centralWidget());
    connect(button3, SIGNAL(clicked()), this, SLOT(openLongDialog()));
    dialogsLayout->addItem(button3);

    buttonStackedDialogs = new DuiButton(centralWidget());
    connect(buttonStackedDialogs, SIGNAL(clicked()), this, SLOT(openStackedDialogs()));
    dialogsLayout->addItem(buttonStackedDialogs);

    buttonWindowModalDialog = new DuiButton(centralWidget());
    connect(buttonWindowModalDialog, SIGNAL(clicked()), this, SLOT(openWindowModalDialog()));
    dialogsLayout->addItem(buttonWindowModalDialog);

    buttonDialogWithProgressIndicator = new DuiButton(centralWidget());
    connect(buttonDialogWithProgressIndicator, SIGNAL(clicked()), this, SLOT(openDialogWithProgressIndicator()));
    dialogsLayout->addItem(buttonDialogWithProgressIndicator);

    button4 = new DuiButton(centralWidget());
    connect(button4, SIGNAL(clicked()), this, SLOT(openMessageBox()));
    dialogsLayout->addItem(button4);

    // Notifications

    notificationsContainer = new DuiContainer();
    QGraphicsLinearLayout *notificationsLayout = new QGraphicsLinearLayout(Qt::Vertical, notificationsContainer->centralWidget());

    layoutPolicy->addItem(notificationsContainer);

    button5 = new DuiButton(centralWidget());
    connect(button5, SIGNAL(clicked()), this, SLOT(showEventBanner()));
    notificationsLayout->addItem(button5);

    button6 = new DuiButton(centralWidget());
    connect(button6, SIGNAL(clicked()), this, SLOT(showInformationBanner()));
    notificationsLayout->addItem(button6);

    button7 = new DuiButton(centralWidget());
    button7->connect(button7, SIGNAL(clicked()), this, SLOT(showSystemInformationBanner()));
    notificationsLayout->addItem(button7);
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

    //% "Window Modal Dialog"
    buttonWindowModalDialog->setText(qtTrId("xx_dialogs_and_notifications_window_modal_dialog"));

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
