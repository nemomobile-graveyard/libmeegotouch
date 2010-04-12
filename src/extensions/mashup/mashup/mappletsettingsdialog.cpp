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

#include "mappletsettingsdialog.h"

#include "mdialog.h"
#include "mwidgetcontroller.h"
#include "mwidgetview.h"
#include "msettingslanguagewidgetfactory.h"
#include "msettingslanguagewidget.h"
#include "mlocale.h"
#include "mcontainer.h"
#include <QGraphicsLinearLayout>
#include "mdebug.h"

class QGraphicsLayoutItem;
class QGraphicsItem;
class MSettingsLanguageBinary;
class MDataStore;

/*!
 * Creates a container for showing applet instance/global settings
 * Also places the widgets into the container
 *
 * \param parent The parent of the container
 * \param title The title of the container
 * \param settingsBinaryList a list of applet settings binaries
 * \param settingsDataStore the settings data store for the settings
 */
static QGraphicsLayoutItem *createAppletSettingsWidgets(QGraphicsItem *parent,
							const QString &title,
							const MSettingsLanguageBinary &settingsBinary,
							MDataStore *settingsDataStore)
{
    MContainer *container = new MContainer(parent);
    container->setTitle(title);
    MWidgetController *centralWidget = new MWidgetController(container);
    container->setCentralWidget(centralWidget);
    QGraphicsLinearLayout *containerLayout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget->setLayout(containerLayout);

    // Create and add settings widgets to container
    containerLayout->addItem(MSettingsLanguageWidgetFactory::createWidget(settingsBinary, settingsDataStore));

    return container;
}


void MAppletSettingsDialog::exec(const MAppletSettings& settings)
{
    if (!settings.hasSettings()) {
        return;
    }

    // Create a central widget for the settings dialog
    MWidgetController *widget = new MWidgetController;
    widget->setView(new MWidgetView(widget));
    widget->setObjectName("MAppletHandleSettingsWidget");
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    widget->setLayout(layout);

    const MSettingsLanguageBinary* instanceSettingsBinary = settings.instanceSettingsBinary();
    if (instanceSettingsBinary != NULL) {
        //~ uispec-document ??? FIXME
        //% "Applet Instance Settings"
        layout->addItem(createAppletSettingsWidgets(widget, qtTrId("mappletsettingsdialog_applet_instance_settings_title"), *instanceSettingsBinary, settings.instanceDataStore()));
    }

    const MSettingsLanguageBinary* globalSettingsBinary = settings.globalSettingsBinary();
    if (globalSettingsBinary != NULL) {
        //~ uispec-document ??? FIXME
        //% "Applet Global Settings"
        layout->addItem(createAppletSettingsWidgets(widget, qtTrId("mappletsettingsdialog_applet_global_settings_title"), *globalSettingsBinary, settings.globalDataStore()));
    }

    // Create a dialog to show the applet instance and global settings
    //~ uispec-document ??? FIXME
    //% "Applet Settings"
    MDialog dialog(qtTrId("mapplethandle_applet_settings_title"), M::NoStandardButton);
    dialog.setCentralWidget(widget);
    dialog.setObjectName("AppletSettingsDialog");
    dialog.exec();
}


MAppletSettingsDialog::MAppletSettingsDialog()
{
}

MAppletSettingsDialog::~MAppletSettingsDialog()
{
}
