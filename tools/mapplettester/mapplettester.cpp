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

#include "mapplettester.h"

#include <MAppletMetaData>
#include <mfiledatastore.h>
#include <MWidget>
#include <mappletloader.h>
#include <MApplication>
#include "mapplettesterwindow.h"
#include <MAction>
#include <MLocale>
#include <MDialog>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <MSettingsLanguageWidgetFactory>
#include <MSettingsLanguageWidget>
#include <MSettingsLanguageParser>
#include <mappletsettingsdialog.h>
#include <mappletsettings.h>
#include <mgconfdatastore.h>
#include <MScene>
#include <QDir>
#include <QDebug>

// Horizontal margin around the applet
static const int hMargin = 10;
// Vertical margin around the applet
static const int vMargin = 10;

MAppletTester::MAppletTester() :
    m_app(NULL),
    m_metadata(NULL),
    m_appletInstanceDataStore(NULL),
    m_widget(NULL),
    m_window(NULL),
    appletSettings(NULL)
{
}

MAppletTester::~MAppletTester()
{
    // Set m_widget to NULL (since it's already destroyed at this point) to prevent double deletion
    m_widget = NULL;
    teardown();
}

bool MAppletTester::init(const MAppletMetaData &metaData, MAppletId appletId)
{
    // Try to construct a valid instance data store and bail out if not successful
    m_appletInstanceDataStore = generateInstanceDataStorePath(metaData.appletBinary());
    if (m_appletInstanceDataStore == NULL) {
        teardown();
        return false;
    }

    // Construct applet settings and a dialog for them
    appletSettings = new MAppletSettings(metaData.fileName(), appletId);

    // Load the applet
    m_widget = MAppletLoader::loadApplet(metaData, *m_appletInstanceDataStore, *appletSettings->dataAccess());

    if (m_widget == NULL) {
        qWarning() << "Failed to construct actual applet widget.";
        teardown();
        return false;
    }
    m_metadata = &metaData;

    // Attach a maction if the applet contains settings
    if (appletSettings->hasSettings()) {
        //% "Applet Settings"
        MAction *action = new MAction(qtTrId("mapplethandle_applet_settings"), m_widget);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(openAppletSettings()), Qt::QueuedConnection);
        m_widget->addAction(action);
    }
    return true;
}

void MAppletTester::openAppletSettings()
{
    if ( appletSettings )
	MAppletSettingsDialog::exec(*appletSettings);
}

int MAppletTester::exec()
{
    if (m_widget == NULL) {
        return -1;
    }

    if (m_window == NULL) {
        m_window = new MAppletTesterWindow;
        connect(m_window, SIGNAL(resized(QSize)), this, SLOT(windowResized(QSize)));

#ifndef __arm__
        // Bring back the window frame in case somebody has messed with it and removed it...
        if (m_window->windowFlags() & Qt::FramelessWindowHint) {
            m_window->setWindowFlags(m_window->windowFlags() ^ Qt::FramelessWindowHint);
        }
#endif
    }

    m_window->setWindowTitle(QString("M applet tester: ") + m_metadata->appletBinary());
    m_widget->setPos(0.0, 0.0);

    QGraphicsLinearLayout widgetLayout;
    widgetLayout.setContentsMargins(0, 0, 0, 0);

    QGraphicsWidget *parentWidget = new QGraphicsWidget();
    parentWidget->setLayout(&widgetLayout);
    widgetLayout.addItem(m_widget);
    m_window->scene()->addItem(parentWidget);
    parentWidget->setZValue(-1);

    QRect rect = QRect(QPoint(), m_widget->preferredSize().toSize());
    rect.adjust(0, 0, hMargin * 2, vMargin * 2);
    m_window->setFrameStyle(0);
    m_window->setGeometry(rect);
    m_window->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_window->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_window->centerOn(0.0, 0.0);

    m_window->show();

    return qApp->exec();
}

void MAppletTester::windowResized(const QSize &newSize)
{
    if (m_widget != NULL) {
        QSizeF newWidgetSize(newSize.width() - (hMargin * 2), newSize.height() - (vMargin * 2));
        newWidgetSize.boundedTo(m_widget->maximumSize());
        newWidgetSize.expandedTo(m_widget->minimumSize());
        m_widget->setGeometry(0.0, 0.0, newWidgetSize.width(), newWidgetSize.height());

        // Twingle with the widget position if the new window size is bigger than the scene rect.
        // This makes the applet top-left corner stay in the window top-left corner.
        QPointF widgetPos = m_widget->pos();
        if (m_window->sceneRect().width() < newSize.width()) {
            widgetPos.setX((m_window->sceneRect().width() - newSize.width()) / 2);
        }
        if (m_window->sceneRect().height() < newSize.height()) {
            widgetPos.setY((m_window->sceneRect().height() - newSize.height()) / 2);
        }
        m_widget->setPos(widgetPos);
    }
}

MDataStore *MAppletTester::generateInstanceDataStorePath(const QString &appletBinary)
{
    QString appletInstanceFileDataPath(appletBinary);
    appletInstanceFileDataPath = QDir::tempPath() + '/' + appletInstanceFileDataPath.replace('/', '_');
    appletInstanceFileDataPath.append(".datastore");
    appletInstanceFileDataPath = QDir::cleanPath(appletInstanceFileDataPath);
    qDebug() << "Instance file:" << appletInstanceFileDataPath;

    MFileDataStore *dataStore = new MFileDataStore(appletInstanceFileDataPath);
    if (!dataStore->isReadable() || !dataStore->isWritable()) {
        qWarning() << "Couldn't create applet instance data store";
        delete dataStore;
        dataStore = NULL;
    }

    return dataStore;
}

void MAppletTester::teardown()
{
    delete m_widget;
    m_widget = NULL;
    delete m_appletInstanceDataStore;
    m_appletInstanceDataStore = NULL;

    m_metadata = NULL;

    delete appletSettings;
    appletSettings = NULL;
}
