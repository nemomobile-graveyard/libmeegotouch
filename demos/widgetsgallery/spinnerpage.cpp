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

#include "spinnerpage.h"
#include "utils.h"
#include <QTimer>
#include <MButton>
#include <MLabel>
#include <MTheme>
#include <MLocale>
#include <MAction>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MFlowLayoutPolicy>
#include <MProgressIndicator>
#include <MDialog>
#include <MContainer>
#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <MImageWidget>
#include <QDir>
#include <MDebug>

static const int ImageSize = 64;

SpinnerPage::SpinnerPage() :
    TemplatePage(),
    container(NULL),
    header(NULL),
    description(NULL),
    view(Unknown)
{
    gid = TemplatePage::IndicatorsAndNotifications;
}

SpinnerPage::~SpinnerPage()
{
}

QString SpinnerPage::timedemoTitle()
{
    return "Spinner";
}

void SpinnerPage::createContent()
{
    TemplatePage::createContent();

    //% "In container main area"
    MAction *action = new MAction(qtTrId("xx_spinner_page_container_main_area"), this);
    action->setLocation(MAction::ApplicationMenuLocation);
    connect(action, SIGNAL(triggered()), this, SLOT(inContainerMainArea()));
    addAction(action);

    //% "In container header"
    action = new MAction(qtTrId("xx_spinner_page_container_header"), this);
    action->setLocation(MAction::ApplicationMenuLocation);
    connect(action, SIGNAL(triggered()), this, SLOT(inContainerHeader()));
    addAction(action);

    //% "In view menu"
    action = new MAction(qtTrId("xx_spinner_page_menu"), this);
    action->setLocation(MAction::ApplicationMenuLocation);
    connect(action, SIGNAL(triggered()), this, SLOT(inViewmenu()));
    addAction(action);

    //% "In dialog"
    action = new MAction(qtTrId("xx_spinner_page_dialog"), this);
    action->setLocation(MAction::ApplicationMenuLocation);
    connect(action, SIGNAL(triggered()), this, SLOT(inDialog()));
    addAction(action);

    inContainerMainArea();

    retranslateUi();
}

void SpinnerPage::retranslateUi()
{
    //% "Spinner"
    setTitle(qtTrId("xx_spinner_page_title"));
    if (!isContentCreated())
        return;

    //% "Spinner is used to indicate that a progress is ongoing "
    //% "and the duration is unknown.\n\n"
    //% "Spinners can also be used in situations when duration is known, "
    //% "but available space on screen does not permit using a Progress bar.\n\n"
    //% "See the menu for examples of different use cases."
    infoLabel->setText("<a></a>" + qtTrId("xx_spinner_page_info_label"));
}

void SpinnerPage::inContainerMainArea()
{
    reset();
    view = ContainerMainArea;

    header = new MLabel(centralWidget());
    //% "From: John Doe\nDate: Today\nSubject: This is funny stuff!"
    header->setText("<a></a>" + qtTrId("xx_spinner_page_email_header"));
    containerPolicy->addItem(header);

    container = new MContainer(centralWidget());
    container->setHeaderVisible(false);
    containerPolicy->addItem(container);

    MProgressIndicator *spinner = new MProgressIndicator(container, MProgressIndicator::spinnerType);
    spinner->setUnknownDuration(true);
    container->setCentralWidget(spinner);

    description = new MLabel(centralWidget());
    //% "Spinner can be used in container while e.g. content is being fetched."
    description->setText("<a></a>" + qtTrId("xx_spinner_page_container_area_description"));
    description->setWordWrap(true);
    containerPolicy->addItem(description);
}

void SpinnerPage::inContainerHeader()
{
    reset();
    view = ContainerHeader;

    container = new MContainer(centralWidget());
    container->setProgressIndicatorVisible(true);
    //% "Online albums"
    container->setTitle(qtTrId("xx_spinner_page_container_title"));
    containerPolicy->addItem(container);

    MLayout *layout = new MLayout(container->centralWidget());
    imageContainerPolicy = new MFlowLayoutPolicy(layout);

    description = new MLabel(centralWidget());
    //% "Spinner can be used in container header to indicate that the items inside the container "
    //% "are being updated, but visible items can be interacted with."
    description->setText("<a></a>" + qtTrId("xx_spinner_page_container_header_description"));
    description->setWordWrap(true);
    containerPolicy->addItem(description);

    connect(&timer, SIGNAL(timeout()), SLOT(timeout()));
    timer.setSingleShot(true);
    timer.start(2000);
}

void SpinnerPage::timeout()
{
    Q_ASSERT(imageContainerPolicy != NULL);

    if (imageContainerPolicy) {
        QString contactsDir = Utils::contactsDir();
        QDir imagesDir(contactsDir);
        imagesDir.setNameFilters(QStringList() << "*.png");

        QStringList imageContacts = imagesDir.entryList(QDir::Files);
        if(imageContacts.isEmpty()) {
            mWarning("SpinnerPage") << "SpinnerPage - Sample image directory does not contain any .png files:" << contactsDir;
            return;
        }

        MImageWidget *image = new MImageWidget(container->centralWidget());
        image->setPixmap(QPixmap(contactsDir + QDir::separator() + imageContacts[imageContainerPolicy->count() % imageContacts.size()]));
        image->setMinimumSize(ImageSize, ImageSize);
        image->setMaximumSize(ImageSize, ImageSize);
        imageContainerPolicy->addItem(image);

        if (imageContainerPolicy->count() < 12) {
            timer.setSingleShot(true);
            timer.start(2000);
        }
    }
}

void SpinnerPage::inViewmenu()
{
    reset();
    view = Menu;

    description = new MLabel(centralWidget());
    //% "Spinner in the application menu area indicates that the whole view is pending for update."
    description->setText("<a></a>" + qtTrId("xx_spinner_page_application_menu_description"));
    description->setWordWrap(true);
    containerPolicy->addItem(description);
    setProgressIndicatorVisible(true);

}

void SpinnerPage::inDialog()
{
    reset();
    view = Dialog;

    description = new MLabel(centralWidget());
    //% "Spinner can be placed in dialog header to indicate changing content."
    description->setText("<a></a>" + qtTrId("xx_spinner_page_dialog_header_description"));
    description->setWordWrap(true);
    containerPolicy->addItem(description);

    launchDialog();
}

void SpinnerPage::launchDialog()
{
    MWidget *container = new MWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    container->setLayout(layout);

    MButton *wifi = new MButton("Free Wifi", container);
    layout->addItem(wifi);
    MButton *wlan = new MButton("Public WLAN", container);
    layout->addItem(wlan);
    MButton *starbucks = new MButton("Starbucks", container);
    layout->addItem(starbucks);

    //% "Select Internet connection"
    MDialog dialog(qtTrId("xx_spinner_page_dialog_header"),
                     M::NoStandardButton);
    dialog.setCentralWidget(container);
    dialog.setProgressIndicatorVisible(true);
    dialog.exec();
}

void SpinnerPage::reset()
{
    switch (view) {
    case Unknown:
        break;

    case ContainerMainArea: {
        containerPolicy->removeItem(header);
        containerPolicy->removeItem(container);
        containerPolicy->removeItem(description);
        delete container;
        delete description;
        delete header;
        container = NULL;
        description = NULL;
        header = NULL;
    } break;

    case ContainerHeader: {
        disconnect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
        containerPolicy->removeItem(container);
        containerPolicy->removeItem(description);
        delete container;
        delete description;
        container = NULL;
        description = NULL;
    } break;

    case Menu: {
        setProgressIndicatorVisible(false);
        containerPolicy->removeItem(description);
        delete description;
        description = NULL;
    } break;

    case Dialog: {
        containerPolicy->removeItem(description);
        delete description;
        description = NULL;
    } break;
    }

    imageContainerPolicy = NULL;
    view = Unknown;
}

