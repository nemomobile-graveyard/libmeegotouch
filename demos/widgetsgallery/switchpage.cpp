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

#include "switchpage.h"
#include <MLabel>
#include <MLayout>
#include <MLocale>
#include <MApplicationPage>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MMessageBox>
#include <QGraphicsLinearLayout>

SwitchPage::SwitchPage() :
        TemplatePage(TemplatePage::Buttons),
    switch1(0), switch2(0),
    switch3(0), switch4(0),
    switch5(0),
    label1(0), label2(0),
    label3(0), label4(0),
    label5(0)
{
}

SwitchPage::~SwitchPage()
{
}

QString SwitchPage::timedemoTitle()
{
    return "Switch";
}

void SwitchPage::createContent()
{
    TemplatePage::createContent();

    //landscapePolicy->insertStretch(1);

    switch1 = new MButton();
    switch1->setViewType(MButton::switchType);
    switch1->setCheckable(true);
    switch1->setChecked(true);
    label1 = new MLabel();
    label1->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->addItem(label1);
    l->addItem(switch1);
    l->setAlignment(switch1, Qt::AlignCenter);
    //l->setAlignment(label1, Qt::AlignRight);
    containerPolicy->addItem(l);
    connect(switch1, SIGNAL(toggled(bool)), this, SLOT(switchToggled(bool)));

    switch2 = new MButton();
    switch2->setViewType(MButton::switchType);
    switch2->setCheckable(true);
    label2 = new MLabel();
    label2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->addItem(label2);
    l->addItem(switch2);
    l->setAlignment(switch2, Qt::AlignCenter);
    //l->setAlignment(label2, Qt::AlignRight);
    containerPolicy->addItem(l);

    switch3 = new MButton();
    switch3->setViewType(MButton::switchType);
    switch3->setCheckable(true);
    label3 = new MLabel();
    label3->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->addItem(label3);
    l->addItem(switch3);
    l->setAlignment(switch3, Qt::AlignCenter);
    //l->setAlignment(label3, Qt::AlignRight);
    containerPolicy->addItem(l);

    switch4 = new MButton();
    switch4->setViewType(MButton::switchType);
    switch4->setCheckable(true);
    label4 = new MLabel();
    label4->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->addItem(label4);
    l->addItem(switch4);
    l->setAlignment(switch4, Qt::AlignCenter);
    //l->setAlignment(label4, Qt::AlignRight);
    containerPolicy->addItem(l);

    switch5 = new MButton();
    switch5->setViewType(MButton::switchType);
    switch5->setCheckable(true);
    switch5->setChecked(true);
    label5 = new MLabel();
    label5->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->addItem(label5);
    l->addItem(switch5);
    l->setAlignment(switch5, Qt::AlignCenter);
    //l->setAlignment(label5, Qt::AlignRight);
    containerPolicy->addItem(l);

    retranslateUi();
}

void SwitchPage::retranslateUi()
{
    //% "Switch"
    setTitle(qtTrId("xx_switch_page_title"));
    if (!isContentCreated())
        return;

    //% "A Switch button differs from both a push button and an "
    //% "icon button visually. It looks like a switch, "
    //% "communicating that pressing this button will not go to "
    //% "another view or will not perform any other actions except "
    //% "to toggle the state of the button.\n\n"
    //% "Switches are used to indicate e.g. settings values"
    infoLabel->setText("<a></a>" + qtTrId("xx_switch_page_info_label").replace('\n', "<br>"));

    //% "Headlights"
    label1->setText(qtTrId("xx_switch_page_switch1"));
    //% "Autopilot"
    label2->setText(qtTrId("xx_switch_page_switch2"));
    //% "Warp Drive"
    label3->setText(qtTrId("xx_switch_page_switch3"));
    //% "Reactor Shields"
    label4->setText(qtTrId("xx_switch_page_switch4"));
    //% "Infinite Improbability Drive"
    label5->setText(qtTrId("xx_switch_page_switch5"));
}

void SwitchPage::switchToggled(bool toggle)
{
    MMessageBox *msgBox = new MMessageBox();

    if (toggle)
        msgBox->setText("The feature was enabled");
    else
        msgBox->setText("The feature was disabled");

    msgBox->appear(MDialog::DestroyWhenDone);
}

