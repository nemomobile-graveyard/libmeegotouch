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

#include <QGraphicsLinearLayout>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MButton>
#include <MSceneManager>
#include <MSlider>
#include <MLabel>
#include <mapplicationextensionarea.h>
#include <demoapplicationextensioninterface.h>
#include <main.h>

void DemoPage::createContent()
{
    QGraphicsWidget *panel = centralWidget();
    QGraphicsLinearLayout *vbox = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(vbox);

    extensionValueLabel = new MLabel;
    extensionValueLabel->setAlignment(Qt::AlignHCenter);
    vbox->addItem(extensionValueLabel);
    extensionValueLabel->setObjectName("ExtensionValueLabel");

    MButton *button = new MButton;
    button->setText("Reset Extensions");
    vbox->addItem(button);
    connect(button, SIGNAL(clicked()), this, SLOT(resetExtensions()));

    MApplicationExtensionArea *area =
            new MApplicationExtensionArea("com.meego.core.DemoApplicationExtensionInterface/1.0");
    area->setInProcessFilter(QRegExp("/applicationextensiondemo-demoextension\\d?\\.desktop$"));
    connect(area, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)),
            this, SLOT(addExtension(MApplicationExtensionInterface*)));
    connect(area, SIGNAL(extensionRemoved(MApplicationExtensionInterface*)),
            this, SLOT(removeExtension(MApplicationExtensionInterface*)));
    area->init();
    vbox->addItem(area);
}

void DemoPage::addExtension(MApplicationExtensionInterface *extension)
{
    DemoApplicationExtensionInterface *demoExtension = static_cast<DemoApplicationExtensionInterface *>(extension);
    demoExtension->setDemoApplicationInterface(*this);
    extensions.append(demoExtension);
    updateLabel();
}

void DemoPage::removeExtension(MApplicationExtensionInterface *extension)
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
    MApplication app(argc, argv);
    MApplicationWindow window;
    DemoPage page;
    window.show();
    window.sceneManager()->appearSceneWindowNow(&page);

    return app.exec();
}
