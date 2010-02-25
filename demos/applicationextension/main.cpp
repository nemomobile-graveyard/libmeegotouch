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

#include <QGraphicsLinearLayout>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiApplicationPage>
#include <DuiButton>
#include <DuiSlider>
#include <DuiLabel>
#include <duiapplicationextensionarea.h>
#include <demoapplicationextensioninterface.h>
#include <main.h>

void DemoPage::createContent()
{
    DuiWidget *panel = centralWidget();
    QGraphicsLinearLayout *vbox = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(vbox);

    extensionValueLabel = new DuiLabel;
    extensionValueLabel->setAlignment(Qt::AlignHCenter);
    vbox->addItem(extensionValueLabel);

    DuiButton *button = new DuiButton;
    button->setText("Reset Extensions");
    vbox->addItem(button);
    connect(button, SIGNAL(clicked()), this, SLOT(resetExtensions()));

    DuiApplicationExtensionArea *area =
            new DuiApplicationExtensionArea("com.nokia.dui.core.DemoApplicationExtensionInterface/1.0");
    vbox->addItem(area);

    connect(area, SIGNAL(extensionInstantiated(DuiApplicationExtensionInterface*)),
            this, SLOT(addExtension(DuiApplicationExtensionInterface*)));
    connect(area, SIGNAL(extensionRemoved(DuiApplicationExtensionInterface*)),
            this, SLOT(removeExtension(DuiApplicationExtensionInterface*)));
}

void DemoPage::addExtension(DuiApplicationExtensionInterface *extension)
{
    DemoApplicationExtensionInterface *demoExtension = static_cast<DemoApplicationExtensionInterface *>(extension);
    demoExtension->setDemoApplicationInterface(*this);
    extensions.append(demoExtension);
    updateLabel();
}

void DemoPage::removeExtension(DuiApplicationExtensionInterface *extension)
{
    DemoApplicationExtensionInterface *demoExtension = static_cast<DemoApplicationExtensionInterface *>(extension);
    extensions.removeOne(demoExtension);
    valueText = "";
    value = 0;
    updateLabel();
}

void DemoPage::resetExtensions()
{
    foreach(DemoApplicationExtensionInterface *demoExtension, extensions) {
        demoExtension->setDemoApplicationControlledValue(0);
    }
}

void DemoPage::updateLabel()
{
    extensionValueLabel->setText(QString::number(value).prepend("Value from extension: ").append(" " + valueText));
}

void DemoPage::setDemoExtensionControlledValue(int value)
{
    this->value = value;
    updateLabel();

    foreach(DemoApplicationExtensionInterface *demoExtension, extensions) {
        demoExtension->setDemoApplicationControlledValue(value);
    }
}

void DemoPage::setDemoExtensionControlledValue(const QString &value)
{
    this->valueText = value;
    updateLabel();
}

int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);
    DuiApplicationWindow window;
    DemoPage page;
    window.show();
    page.appearNow();

    return app.exec();
}
