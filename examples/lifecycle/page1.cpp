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
#include <MButton>
#include <MLabel>
#include <MTheme>
#include <MLocale>
#include <MImage>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MProgressIndicator>
#include <MSceneManager>

#include "progressindicatorpage.h"

ProgressIndicatorPage::ProgressIndicatorPage() : MApplicationPage()
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
    MLayout *layout = new MLayout();
    centralWidget()->setLayout(layout);

    l_policy = new MGridLayoutPolicy(layout);
    l_policy->setContentsMargins(32, 32, 32, 32);
    l_policy->setRowSpacing(1, 32.0f);
    l_policy->setSpacing(36);

    p_policy = new MGridLayoutPolicy(layout);
    p_policy->setContentsMargins(32, 32, 32, 32);
    p_policy->setRowSpacing(1, 32.0f);
    p_policy->setRowSpacing(3, 32.0f);

    //% "unknown duration - bar"
    MLabel  *label1 = new MLabel(qtTrId("xx_progressindicator_unknown_duration_bar"), this);
    label1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    label1->setMaximumWidth(382);
    label1->setMinimumWidth(382);

    bar1 = new MProgressIndicator(this, "bar");
    bar1->setRange(0, 99);
    bar1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar1->setRunUnknown(TRUE);
    bar1->setObjectName("bar");

    //% "known duration - bar"
    MLabel *label2  = new MLabel(qtTrId("xx_progressindicator_known_duration_bar"), this);
    label2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    bar2 = new MProgressIndicator(this, "bar");
    bar2->setRange(0, 99);
    bar2->setValue(0);
    bar2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    bar2->setObjectName("bar");

    //% "unknown duration - circular"
    MLabel *label3  = new MLabel(qtTrId("xx_progressindicator_unknown_duration_circular"), this);
    label3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    circ1 = new MProgressIndicator(this);
    circ1->setRange(0, 99);
    circ1->setMaximumHeight(64);
    circ1->setMinimumHeight(64);
    circ1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    circ1->setRunUnknown(true);
    circ1->setObjectName("circle");

    //% "known duration - circular"
    MLabel  *label4  = new MLabel(qtTrId("xx_progressindicator_known_duration_circular"), this);
    label4->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    circ2 = new MProgressIndicator(this);
    circ2->setRange(0, 99);
    circ2->setValue(0);
    circ2->setMaximumHeight(64);
    circ2->setMinimumHeight(64);
    circ2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    circ2->setObjectName("circle");

    //% "Stop"
    buttonLeft = new MButton(qtTrId("xx_progressindicator_button_stop"), this);
    //% "Increase"
    MButton *buttonRight = new MButton(qtTrId("xx_progressindicator_button_increase"), this);

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

    connect(MSceneManager::instance(),
            SIGNAL(orientationChanged(M::Orientation)),
            this,
            SLOT(pageRotated(M::Orientation)));
}

void ProgressIndicatorPage::pageRotated(const M::Orientation &orientation)
{
    QSize s = MSceneManager::instance()->visibleSceneSize();

    centralWidget()->setMinimumSize(s);
    centralWidget()->setMaximumSize(s);
    centralWidget()->setPreferredSize(s);

    if (orientation == M::Portrait) {
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
    static const M::OrientationAngle angles[] = {
        M::Angle0,
        M::Angle90,
        M::Angle180,
        M::Angle270
    };

    static int a = 0;

    a = (a + d) & 3;

    MSceneManager::instance()->setOrientationAngle(angles[a]);
}
