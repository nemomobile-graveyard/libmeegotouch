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

#include <QTimer>
#include <DuiLocale>
#include <DuiLayout>
#include <DuiButton>
#include <DuiLabel>
#include <DuiGridLayoutPolicy>
#include <DuiFlowLayoutPolicy>
#include <DuiProgressIndicator>
#include <QDebug>

#include "mainpage.h"
#include "anotherpage.h"

MainPage::MainPage() :
    bar0(NULL),
    bar1(NULL),
    anotherPage(new AnotherPage()),
    canvasPage(new ContainerPage()),
    m_shownPage(NULL),
    m_pTimer(new QTimer(this))
{
    setTitle("MainPage");

    m_pbarValue = 1;

    m_pTimer->setInterval(100);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateProgressBars()));
}

MainPage::~MainPage()
{
    delete anotherPage;
    delete canvasPage;
}

void MainPage::createContent()
{
    DuiLayout *layout = new DuiLayout(centralWidget());
    DuiGridLayoutPolicy *policy = new DuiGridLayoutPolicy(layout);
    policy->setSpacing(20.0);

    bar0 = new DuiProgressIndicator(this, "spinner");
    bar0->setRange(0, 12);
    bar0->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar0->setUnknownDuration(false);
    bar0->setObjectName("bar0");
    policy->addItem(bar0, 0, 0, 1, 3);

    DuiLabel *label1 = new DuiLabel("Application Lifecycle Demo");
    label1->setAlignment(Qt::AlignCenter);
    policy->addItem(label1, 1, 1);

    bar1 = new DuiProgressIndicator(this, "spinner");
    bar1->setRange(0, 120);
    bar1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar1->setUnknownDuration(false);
    bar1->setObjectName("bar1");
    policy->addItem(bar1, 4, 0, 1, 3);

    connect(bar0, SIGNAL(displayEntered()), this, SLOT(activateWidgets()));
    connect(bar1, SIGNAL(displayEntered()), this, SLOT(activateWidgets()));
    connect(bar0, SIGNAL(displayExited()),  this, SLOT(pauseWidgets()));
    connect(bar1, SIGNAL(displayExited()),  this, SLOT(pauseWidgets()));

    DuiButton *button = new DuiButton("Visibility Information");
    connect(button, SIGNAL(clicked()), this, SLOT(buttonPress()));
    policy->addItem(button, 5, 1);

    DuiButton *button2 = new DuiButton("Applet Stuff");
    connect(button2, SIGNAL(clicked()), this, SLOT(button2Press()));
    policy->addItem(button2, 6, 1);

    layout->setLandscapePolicy(policy);
}

void MainPage::indicatorVisible()
{
    qDebug() << qobject_cast<DuiProgressIndicator *>(sender())->objectName() << " visible";

}

void MainPage::indicatorHidden()
{
    qDebug() << qobject_cast<DuiProgressIndicator *>(sender())->objectName() << " hidden";
}

void MainPage::buttonPress()
{
    anotherPage->setEscapeMode(DuiApplicationPageModel::EscapeManualBack);
    connect(anotherPage, SIGNAL(backButtonClicked()), this, SLOT(handleBackItemClick()));
    m_shownPage = anotherPage;
    anotherPage->appear();
}

void MainPage::button2Press()
{
    canvasPage->setEscapeMode(DuiApplicationPageModel::EscapeManualBack);
    connect(canvasPage, SIGNAL(backButtonClicked()), this, SLOT(handleBackItemClick()));
    m_shownPage = canvasPage;
    canvasPage->appear();
}

void MainPage::handleBackItemClick()
{
    m_shownPage->disconnect(this, SLOT(handleBackItemClick()));
    appear();
}

void MainPage::updateProgressBars()
{
    bar0->setValue(m_pbarValue % (bar0->maximum() + 1));
    bar1->setValue(m_pbarValue % (bar1->maximum() + 1));
    m_pbarValue++;
}

void MainPage::deactivateWidgets()
{
    qDebug() << "lifecycle: prestart restored";
    m_pTimer->stop();
    bar0->reset();
    bar1->reset();
    m_pbarValue = 0;
}

void MainPage::pauseWidgets()
{
    qDebug() << "pauseWidgets";
    m_pTimer->stop();
}

void MainPage::activateWidgets()
{
    qDebug() << "activateWidgets";
    m_pTimer->start();
}

void MainPage::exitDisplayEvent()
{
    qDebug() << "lifecycle: MainPage hidden";
}

void MainPage::enterDisplayEvent()
{
    qDebug() << "lifecycle: MainPage visible";
}

