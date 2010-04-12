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

#include <QTimer>
#include <MLocale>
#include <MLayout>
#include <MButton>
#include <MLabel>
#include <MGridLayoutPolicy>
#include <MFlowLayoutPolicy>
#include <MProgressIndicator>
#include <QDebug>

#include "mainpage.h"
#include "anotherpage.h"

MainPage::MainPage() :
    bar0(NULL),
    bar1(NULL),
    anotherPage(new AnotherPage()),
    canvasPage(new ContainerPage()),
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
    MLayout *layout = new MLayout(centralWidget());
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    policy->setSpacing(20.0);

    bar0 = new MProgressIndicator(this, "spinner");
    bar0->setRange(0, 12);
    bar0->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar0->setUnknownDuration(false);
    bar0->setObjectName("bar0");
    policy->addItem(bar0, 0, 0, 1, 3);

    MLabel *label1 = new MLabel("Application Lifecycle Demo");
    label1->setAlignment(Qt::AlignCenter);
    policy->addItem(label1, 1, 1);

    bar1 = new MProgressIndicator(this, "spinner");
    bar1->setRange(0, 120);
    bar1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar1->setUnknownDuration(false);
    bar1->setObjectName("bar1");
    policy->addItem(bar1, 4, 0, 1, 3);

    connect(bar0, SIGNAL(displayEntered()), this, SLOT(activateWidgets()));
    connect(bar1, SIGNAL(displayEntered()), this, SLOT(activateWidgets()));
    connect(bar0, SIGNAL(displayExited()),  this, SLOT(pauseWidgets()));
    connect(bar1, SIGNAL(displayExited()),  this, SLOT(pauseWidgets()));

    MButton *button = new MButton("Visibility Information");
    connect(button, SIGNAL(clicked()), this, SLOT(buttonPress()));
    policy->addItem(button, 5, 1);

    MButton *button2 = new MButton("Applet Stuff");
    connect(button2, SIGNAL(clicked()), this, SLOT(button2Press()));
    policy->addItem(button2, 6, 1);

    layout->setLandscapePolicy(policy);
}

void MainPage::indicatorVisible()
{
    qDebug() << qobject_cast<MProgressIndicator *>(sender())->objectName() << " visible";

}

void MainPage::indicatorHidden()
{
    qDebug() << qobject_cast<MProgressIndicator *>(sender())->objectName() << " hidden";
}

void MainPage::buttonPress()
{
    anotherPage->appear(KeepWhenDone);
}

void MainPage::button2Press()
{
    canvasPage->appear(KeepWhenDone);
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

