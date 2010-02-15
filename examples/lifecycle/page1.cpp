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
#include <DuiButton>
#include <DuiLabel>
#include <DuiTheme>
#include <DuiLocale>
#include <DuiImage>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>
#include <DuiProgressIndicator>
#include <DuiSceneManager>

#include "progressindicatorpage.h"

ProgressIndicatorPage::ProgressIndicatorPage() : DuiApplicationPage()
{
    //% "Progress Indicator"
    setTitle(qtTrId("xx_progressindicator_page_title"));
    setObjectName("lifecycleypage");
}

ProgressIndicatorPage::~ProgressIndicatorPage()
{
}

void ProgressIndicatorPage::createContent()
{
    DuiLayout *layout = new DuiLayout();
    centralWidget()->setLayout(layout);

    l_policy = new DuiGridLayoutPolicy(layout);
    l_policy->setContentsMargins(32, 32, 32, 32);
    l_policy->setRowSpacing(1, 32.0f);
    l_policy->setSpacing(36);

    p_policy = new DuiGridLayoutPolicy(layout);
    p_policy->setContentsMargins(32, 32, 32, 32);
    p_policy->setRowSpacing(1, 32.0f);
    p_policy->setRowSpacing(3, 32.0f);

    //% "unknown duration - bar"
    DuiLabel  *label1 = new DuiLabel(qtTrId("xx_progressindicator_unknown_duration_bar"), this);
    label1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    label1->setMaximumWidth(382);
    label1->setMinimumWidth(382);

    bar1 = new DuiProgressIndicator(this, "bar");
    bar1->setRange(0, 99);
    bar1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar1->setRunUnknown(TRUE);
    bar1->setObjectName("bar");

    //% "known duration - bar"
    DuiLabel *label2  = new DuiLabel(qtTrId("xx_progressindicator_known_duration_bar"), this);
    label2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    bar2 = new DuiProgressIndicator(this, "bar");
    bar2->setRange(0, 99);
    bar2->setValue(0);
    bar2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar2->setObjectName("bar");

    //% "unknown duration - circular"
    DuiLabel *label3  = new DuiLabel(qtTrId("xx_progressindicator_unknown_duration_circular"), this);
    label3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    circ1 = new DuiProgressIndicator(this);
    circ1->setRange(0, 99);
    circ1->setMaximumHeight(64);
    circ1->setMinimumHeight(64);
    circ1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    circ1->setRunUnknown(true);
    circ1->setObjectName("circle");

    //% "known duration - circular"
    DuiLabel  *label4  = new DuiLabel(qtTrId("xx_progressindicator_known_duration_circular"), this);
    label4->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    circ2 = new DuiProgressIndicator(this);
    circ2->setRange(0, 99);
    circ2->setValue(0);
    circ2->setMaximumHeight(64);
    circ2->setMinimumHeight(64);
    circ2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    circ2->setObjectName("circle");

    //% "Stop"
    buttonLeft = new DuiButton(qtTrId("xx_progressindicator_button_stop"), this);
    //% "Increase"
    DuiButton *buttonRight = new DuiButton(qtTrId("xx_progressindicator_button_increase"), this);

    buttonLeft->setMinimumHeight(72);
    buttonRight->setMinimumHeight(72);

    l_policy->addItem(label1,  0, 0);
    l_policy->addItem(bar1, 1, 0);
    l_policy->addItem(label2,  0, 1);
    l_policy->addItem(bar2, 1, 1, 1, 1, Qt::AlignVCenter);
    l_policy->addItem(label3,  2, 0);
    l_policy->addItem(circ1, 3, 0);
    l_policy->addItem(label4,  2, 1);
    l_policy->addItem(circ2, 3, 1, 1, 1, Qt::AlignVCenter);
    l_policy->addItem(buttonLeft, 4, 0);
    l_policy->addItem(buttonRight, 4, 1, 1, 1, Qt::AlignVCenter);

    p_policy->addItem(label1,  0, 0, 1, 2);
    p_policy->addItem(bar1, 1, 0, 1, 2);
    p_policy->addItem(label2,  2, 0, 1, 2);
    p_policy->addItem(bar2, 3, 0, 1, 2);
    p_policy->addItem(label3,  4, 0, 1, 2);
    p_policy->addItem(circ1, 5, 0, 1, 2);
    p_policy->addItem(label4,  6, 0, 1, 2);
    p_policy->addItem(circ2, 7, 0, 1, 2);

    connect(buttonLeft, SIGNAL(pressed()), this, SLOT(buttonLeftSlot()));
    connect(buttonRight, SIGNAL(pressed()), this, SLOT(buttonRightSlot()));

    connect(DuiSceneManager::instance(),
            SIGNAL(orientationChanged(Dui::Orientation)),
            this,
            SLOT(pageRotated(Dui::Orientation)));
}

void ProgressIndicatorPage::pageRotated(const Dui::Orientation &orientation)
{
    QSize s = DuiSceneManager::instance()->visibleSceneSize();

    centralWidget()->setMinimumSize(s);
    centralWidget()->setMaximumSize(s);
    centralWidget()->setPreferredSize(s);

    if (orientation == Dui::Portrait) {
        p_policy->activate();
    } else {
        l_policy->activate();
    }
}

void ProgressIndicatorPage::buttonLeftSlot()
{
    bar1->setRunUnknown(!bar1->runUnknown());
    circ1->setRunUnknown(!circ1->runUnknown());
    if (bar1->runUnknown())
        //% "Stop"
        buttonLeft->setText(qtTrId("xx_progressindicator_button_stop"));
    else
        //% "Run"
        buttonLeft->setText(qtTrId("xx_progressindicator_button_run"));
}

void ProgressIndicatorPage::buttonRightSlot()
{
    bar2->setValue((bar2->value() + 10) % bar2->maximum());
    circ2->setValue((circ2->value() + 10) % circ2->maximum());
}

void ProgressIndicatorPage::rotatel()
{
    rotate(-1);
}

void ProgressIndicatorPage::rotater()
{
    rotate(1);
}

void ProgressIndicatorPage::rotate(int d)
{
    static const Dui::OrientationAngle angles[] = {
        Dui::Angle0,
        Dui::Angle90,
        Dui::Angle180,
        Dui::Angle270
    };

    static int a = 0;

    a = (a + d) & 3;

    DuiSceneManager::instance()->setOrientationAngle(angles[a]);
}
