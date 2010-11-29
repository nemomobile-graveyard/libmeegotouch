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

#include "pt_mcomponentdata2.h"

#include <mbenchmark.h>

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <qglobal.h>
#include <QInputContextFactory>
#include <QInputContext>

#include "mtheme.h"
#include "mthemedaemon.h"
#include "mcomponentdata.h"
#include "mlocale.h"
#include "mfeedbackplayer.h"
#include "mfeedbackplayer_p.h"
#include "mcomponentcache.h"
#include "mcomponentdata_p.h"
#include "mapplicationservice.h"
#include <MDebug>

#include <QtPlugin>
#include <QPluginLoader>
#include <QLibraryInfo>
#include <QDBusConnection>
#include "testabilityinterface.h"

#ifdef HAVE_XFIXES
#include <QX11Info>
#include <X11/extensions/Xfixes.h>
#endif // HAVE_XFIXES

void mMessageHandler(QtMsgType, const char *) {}

namespace {
    int argc = 3;
    char appName[] = "./pt_mcomponentdata";
    char style[] = "-style";
    char windows[] = "windows";
    char *argv[] = { appName, style, windows };
    QApplication *app;
    QString themeIdentifier;
}

void Pt_MComponentData2::initTestCase()
{
    app = new QApplication(argc, argv);
}

void Pt_MComponentData2::testabilityPlugin()
{
    MBENCHMARK_ONCE(
       QString testabilityPluginPostfix = ".so";
       QString testabilityPlugin = "testability/libtestability";

       testabilityPlugin = QLibraryInfo::location(QLibraryInfo::PluginsPath) + QDir::separator() + testabilityPlugin + testabilityPluginPostfix;
       QPluginLoader loader(testabilityPlugin.toLatin1().data());

       QObject *plugin = loader.instance();
       if (plugin) {
           mDebug("MComponentData") << "Testability plugin loaded successfully!";
           TestabilityInterface *testabilityInterface = qobject_cast<TestabilityInterface *>(plugin);

           if (testabilityInterface) {
               mDebug("MComponentData") << "Testability interface obtained!";
               testabilityInterface->Initialize();
           } else {
               mDebug("MComponentData") << "Failed to get testability interface!";
           }
       } else {
           mDebug("MComponentData") << QString("Testability plugin %1 load failed with error: %2")
                                        .arg(testabilityPlugin).arg(loader.errorString());
       }
    )
}

void Pt_MComponentData2::installMessageHandler()
{
    MBENCHMARK_ONCE(
        qInstallMsgHandler(mMessageHandler);
    )
}

void Pt_MComponentData2::themeId()
{
    MBENCHMARK_ONCE(
        QFileInfo fileInfo(argv[0]);
        themeIdentifier = fileInfo.fileName();
    )
}

void Pt_MComponentData2::theme()
{
    MTheme *theme = 0;
    MBENCHMARK_ONCE(
        theme = new MTheme(themeIdentifier, QString(), MTheme::RemoteTheme);
    )
    delete theme;
}

void Pt_MComponentData2::locale()
{
    MBENCHMARK_ONCE(
        QString catalog = themeIdentifier;
        // set the path for the icu extra data:
        MLocale::setDataPath(M_ICUEXTRADATA_DIR);

        MLocale systemLocale; // gets the current system locale, creating it if necessary.
        systemLocale.addTranslationPath(TRANSLATION_DIR);

        // installs the libmeegotouch translations
        systemLocale.installTrCatalog("libmeegotouch");
        // installs the common translation catalog:
        systemLocale.installTrCatalog("common");
        // installs the translation catalog of the application if we
        // already know the application/catalog name (not just populate
        // cache)
        if (!MComponentCache::populating()) {
            systemLocale.installTrCatalog(catalog);
        }
        MLocale::setDefault(systemLocale);
    )
}

void Pt_MComponentData2::feedbackPlayer()
{
    MBENCHMARK_ONCE(
        MFeedbackPlayer *feedbackPlayer = new MFeedbackPlayer();
        if (!feedbackPlayer->d_ptr->init(themeIdentifier)) {
            delete feedbackPlayer;
            feedbackPlayer = 0;
        }
    )
}

void Pt_MComponentData2::dbusConnection()
{
    MBENCHMARK_ONCE(
        QDBusConnection connection = QDBusConnection::sessionBus();
    )

}

void Pt_MComponentData2::isConnected()
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    MBENCHMARK_ONCE(
        if (connection.isConnected() == false) {
            qCritical("ERROR: No DBUS session bus found. Exiting now. Please make sure that a dbus session bus\n"
                      "is running. In Scratchbox you should execute meego-sb-session start. On the target device\n"
                      "it should already be running. You should also make sure that the DBUS_SESSION_BUS_ADDRESS\n"
                      "environment variable is set correctly. For that you can execute the following command:\n"
                      "source /tmp/session_bus_address.user\n");
        }
    )
}

void Pt_MComponentData2::registerDefaultService()
{
    MBENCHMARK_ONCE(
    QString serviceName = "com.nokia." + themeIdentifier;
    MApplicationService *service = new MApplicationService(serviceName);
    service->registerService();
    )
}

void Pt_MComponentData2::showCursor()
{
    MBENCHMARK_ONCE(
#ifdef HAVE_XFIXES
        XFixesShowCursor(QX11Info::display(), QX11Info::appRootWindow());
#else
        qApp->restoreOverrideCursor();
#endif
    )
}

void Pt_MComponentData2::inputContext()
{
    if (!MComponentData::instance()) {
        new MComponentData(argc, argv);
    }
    MBENCHMARK_ONCE(
        QInputContext *ic = QInputContextFactory::create("MInputContext", 0);
        if (ic != 0) {
            qApp->setInputContext(ic);
        }
    )
}

QTEST_APPLESS_MAIN(Pt_MComponentData2)
