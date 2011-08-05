/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "debugpage.h"
#include <QGraphicsGridLayout>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MPannableViewport>

#include <QDebug>
#include <QBitArray>

DebugPage::DebugPage()
    : TemplatePage(TemplatePage::ApplicationView),
    debugOptionsShortcuts("/meegotouch/debug/info_display"),
    label1(0),
    labelNames(0),
    labelObjects(0),
    labelPosition(0),
    labelSize(0),
    labelBoundingRect(0),
    labelMargins(0),
    labelFps(0),
    checkBoxNames(0),
    checkBoxobjNames(0),
    checkBoxPosition(0),
    checkBoxSize(0),
    checkBoxBoundingRect(0),
    checkBoxMargins(0),
    checkBoxFps(0)
{
}

DebugPage::~DebugPage()
{
}

void DebugPage::createContent()
{
    TemplatePage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    QGraphicsWidget *panel = centralWidget();
    panel->setContentsMargins(0,0,0,0);

    setContentsMargins(0,0,0,0);

    layout = new MLayout(panel);

    layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->setSpacing(0);
    layoutPolicy->setContentsMargins(0,0,0,0);

    layout->setPortraitPolicy(layoutPolicy);
    layout->setLandscapePolicy(layoutPolicy);
    label1 = new MLabel ("Information Display");
    label1->setObjectName("label1");
    label1->setStyleName(inv("CommonApplicationHeader"));
    layoutPolicy->addItem(label1);

    labelNames= new MLabel ("Show style names");
    labelNames->setObjectName("labelNames");
    labelNames->setStyleName(inv("CommonSingleTitle"));

    checkBoxNames = new MButton();
    checkBoxNames->setObjectName("checkBoxNames");
    checkBoxNames->setViewType(MButton::checkboxType);
    checkBoxNames->setStyleName(inv("CommonRightCheckBox"));
    checkBoxNames->setCheckable(true);

    labelObjects= new MLabel ("Show object names");
    labelObjects->setObjectName("labelObjects");
    labelObjects->setStyleName(inv("CommonSingleTitle"));

    checkBoxobjNames = new MButton();
    checkBoxobjNames->setObjectName("checkBoxobjNames");
    checkBoxobjNames->setViewType(MButton::checkboxType);
    checkBoxobjNames->setStyleName(inv("CommonRightCheckBox"));
    checkBoxobjNames->setCheckable(true);

    labelSize= new MLabel ("Show sizes");
    labelSize->setObjectName("labelSize");
    labelSize->setStyleName(inv("CommonSingleTitle"));

    checkBoxSize = new MButton();
    checkBoxSize->setObjectName("checkBoxSize");
    checkBoxSize->setViewType(MButton::checkboxType);
    checkBoxSize->setCheckable(true);
    checkBoxSize->setStyleName(inv("CommonRightCheckBox"));

    labelBoundingRect= new MLabel ("Show bounding rect");
    labelBoundingRect->setObjectName("labelBoundingRect");
    labelBoundingRect->setStyleName(inv("CommonSingleTitle"));

    checkBoxBoundingRect = new MButton();
    checkBoxBoundingRect->setObjectName("checkBoxBoundingRect");
    checkBoxBoundingRect->setViewType(MButton::checkboxType);
    checkBoxBoundingRect->setCheckable(true);
    checkBoxBoundingRect->setStyleName(inv("CommonRightCheckBox"));

    labelPosition= new MLabel ("Show positions");
    labelPosition->setObjectName("labelPosition");
    labelPosition->setStyleName(inv("CommonSingleTitle"));

    checkBoxPosition = new MButton();
    checkBoxPosition->setObjectName("checkBoxPosition");
    checkBoxPosition->setViewType(MButton::checkboxType);
    checkBoxPosition->setCheckable(true);
    checkBoxPosition->setStyleName(inv("CommonRightCheckBox"));


    labelMargins= new MLabel ("Show margins");
    labelMargins->setObjectName("labelMargins");
    labelMargins->setStyleName(inv("CommonSingleTitle"));

    checkBoxMargins = new MButton();
    checkBoxMargins->setObjectName("checkBoxMargins");
    checkBoxMargins->setViewType(MButton::checkboxType);
    checkBoxMargins->setCheckable(true);
    checkBoxMargins->setStyleName(inv("CommonRightCheckBox"));


    labelFps= new MLabel ("Show FPS");
    labelFps->setObjectName("labelFps");
    labelFps->setStyleName(inv("CommonSingleTitle"));

    checkBoxFps = new MButton();
    checkBoxFps->setObjectName("checkBoxFps");
    checkBoxFps->setViewType(MButton::checkboxType);
    checkBoxFps->setCheckable(true);
    checkBoxFps->setStyleName(inv("CommonRightCheckBox"));

    QGraphicsGridLayout *gridOptions = new QGraphicsGridLayout(layout);
    gridOptions->setContentsMargins(0, 0, 0, 0);
    gridOptions->setSpacing(0);
    gridOptions->addItem(labelNames, 0, 0, Qt::AlignVCenter);
    gridOptions->addItem(checkBoxNames, 0, 1, Qt::AlignVCenter);
    gridOptions->addItem(labelSize, 1, 0, Qt::AlignVCenter);
    gridOptions->addItem(checkBoxSize, 1, 1, Qt::AlignVCenter);
    gridOptions->addItem(labelBoundingRect, 2, 0, Qt::AlignVCenter);
    gridOptions->addItem(checkBoxBoundingRect, 2, 1, Qt::AlignVCenter);
    gridOptions->addItem(labelPosition, 3, 0, Qt::AlignVCenter);
    gridOptions->addItem(checkBoxPosition, 3, 1, Qt::AlignVCenter);
    gridOptions->addItem(labelMargins, 4, 0, Qt::AlignVCenter);
    gridOptions->addItem(checkBoxMargins, 4, 1, Qt::AlignVCenter);
    gridOptions->addItem(labelFps, 5, 0, Qt::AlignVCenter);
    gridOptions->addItem(checkBoxFps, 5, 1, Qt::AlignVCenter);
    gridOptions->addItem(labelObjects, 6, 0, Qt::AlignVCenter);
    gridOptions->addItem(checkBoxobjNames, 6, 1, Qt::AlignVCenter);

    this->layoutPolicy->addItem(gridOptions);

    connect(checkBoxNames, SIGNAL(clicked()),this, SLOT(applyOptions()));
    connect(checkBoxobjNames, SIGNAL(clicked()),this, SLOT(applyOptions()));
    connect(checkBoxSize, SIGNAL(clicked()),this, SLOT(applyOptions()));
    connect(checkBoxBoundingRect, SIGNAL(clicked()),this, SLOT(applyOptions()));
    connect(checkBoxPosition, SIGNAL(clicked()),this, SLOT(applyOptions()));
    connect(checkBoxMargins, SIGNAL(clicked()),this, SLOT(applyOptions()));
    connect(checkBoxFps, SIGNAL(clicked()),this, SLOT(applyOptions()));
}

QString DebugPage::timedemoTitle()
{
    return "Information Display";
}

void DebugPage::applyOptions()
{
    QBitArray optionsSelected;
    optionsSelected.resize(7);

    checkBoxNames->isChecked() ? optionsSelected.setBit(0, true) : optionsSelected.setBit(0, false);
    checkBoxobjNames->isChecked() ? optionsSelected.setBit(1, true) : optionsSelected.setBit(1, false);
    checkBoxSize->isChecked() ? optionsSelected.setBit(2, true) : optionsSelected.setBit(2, false);
    checkBoxBoundingRect->isChecked() ? optionsSelected.setBit(3, true) : optionsSelected.setBit(3, false);
    checkBoxPosition->isChecked() ? optionsSelected.setBit(4, true) : optionsSelected.setBit(4, false);
    checkBoxMargins->isChecked() ? optionsSelected.setBit(5, true) : optionsSelected.setBit(5, false);
    checkBoxFps->isChecked() ? optionsSelected.setBit(6, true) : optionsSelected.setBit(6, false);

    QString gconfBitArray;

    for (int i = 0; i < optionsSelected.size(); i++) {
        if (optionsSelected.at(i) == true) {
            gconfBitArray+='1';
        } else {
            gconfBitArray+='0';
        }
    }
    debugOptionsShortcuts.set(gconfBitArray);
}

