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

#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <MGConfItem>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include "clientmanager.h"
#include "testclient.h"

#define CLIENT_ID(client) "Client (0x" + QString::number((quint32) client, 16) + ')'

void removeDirectoryRecursive(const QString &path)
{
    QDir root(path);
    if (root.exists()) {
        QFileInfoList entries = root.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
        for (int i = 0; i < entries.count(); i++) {
            if (entries[i].isDir()) {
                removeDirectoryRecursive(entries[i].filePath());
                root.rmdir(entries[i].absoluteFilePath());
            } else {
                root.remove(entries[i].absoluteFilePath());
            }
        }
    }
}

void ClientManager::cleanup()
{
    QDir themeDirectory(THEME_ROOT_DIRECTORY);
    QStringList list = themeDirectory.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.size(); i++) {
        QString path = list.at(i);

        QDir theme(themeDirectory.absolutePath() + QDir::separator() + path + QDir::separator() + QString("meegotouch"));
        // find all client directories
        QStringList directories = theme.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        for(int j=0; j<directories.size(); j++) {
            if(directories.at(j).startsWith("client") || directories.at(j) == QString("icons"))
            {
                removeDirectoryRecursive(theme.absolutePath() + QDir::separator() + directories.at(j));
                theme.rmdir(directories.at(j));
            }
        }
        // loop all locales
        for(int localeIndex=1; localeIndex<locales.size(); localeIndex++)
        {
            // remove all locale-specific icons
            QString localeSpecificIconsDirectory = theme.absolutePath() + QDir::separator() + 
                                                   "locale" + QDir::separator() + 
                                                   locales[localeIndex] + QDir::separator() + 
                                                   QString("icons");
            removeDirectoryRecursive(localeSpecificIconsDirectory);
            theme.rmdir(localeSpecificIconsDirectory);
        }
    }
}

void createSVGFile(const QString& path, int index)
{
    QString filename = path + QDir::separator() + QString("icon-") + QString::number(index) + ".svg";

    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QSvgGenerator generator;
    generator.setFileName(filename);
    generator.setSize(QSize(200, 200));
    generator.setViewBox(QRect(0, 0, 200, 200));
    generator.setOutputDevice(&file);

    QPainter painter;
    painter.begin(&generator);

    QColor color(rand() % 255, rand() % 255, rand() % 255);
    painter.fillRect(generator.viewBox(), color);
    painter.end();

    file.close();
}

ClientManager::ClientManager(QProcess &process) : shutdown(false), themedaemon(process)
{
    locales.append("en");
    locales.append("fi");

    cleanup();

    int count = 50;
    // create svg images for icons for all themes & locales
    QDir themeDirectory(THEME_ROOT_DIRECTORY);
    QStringList list = themeDirectory.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.size(); i++) {
        QString themeName = list.at(i);

        // create path for images
        QString iconsPath = themeName + QDir::separator() + QString("meegotouch") + QDir::separator() + QString("icons");
        if(themeDirectory.mkpath(iconsPath))
        {
            // create svg icon
            //int count = (rand() + 10) % 200;

            // create imagery to theme's root icons dir
            for (int imageIndex = 0; imageIndex < count; imageIndex++) {
                createSVGFile(themeDirectory.absolutePath() + QDir::separator() + iconsPath, imageIndex);
            }

            // create locales
            QString localeRoot = themeName + QDir::separator() + QString("meegotouch") + QDir::separator() + QString("locale");
            for(int localeIndex=1; localeIndex<locales.size(); localeIndex++) {
                QString localeIconPath = localeRoot + QDir::separator() + locales[localeIndex] + QDir::separator() + QString("icons");
                if(themeDirectory.mkpath(localeIconPath)) {
                    // create svg imagery
                    for (int imageIndex = 0; imageIndex < count; imageIndex++) {
                        createSVGFile(themeDirectory.absolutePath() + QDir::separator() + localeIconPath, imageIndex);
                    }
                }
            }
        }
    }

    // start the test after 1 sec (allow themedaemon to get online)
    QTimer::singleShot(1000, this, SLOT(start()));
}

ClientManager::~ClientManager()
{
    // perform cleanup
    cleanup();
}

void ClientManager::spawnClient()
{
    static unsigned int clientId = 0;
    ClientThread *client = new ClientThread(this);
    client->setId("client" + QString::number(clientId++));

    // generate imagery for testing purposes
    connect(client, SIGNAL(started()), this, SLOT(clientStarted()));
    connect(client, SIGNAL(finished()), this, SLOT(clientFinished()));
    clients.insert(client);

    client->start();
    qDebug() << "INFO: ClientManager - Client" << client->getId() << "started, number of active clients:" << clients.count();
}

void ClientManager::start()
{
#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO: ClientManager - Starting up...";
#endif
    // change theme & begin theme switching
    changeThemeAndLocale();

    // start performing consistency checks & spawn new clients
    QTimer::singleShot(0, this, SLOT(checkConsistency()));
}

void ClientManager::stop()
{
    shutdown = true;
    qDebug() << "INFO: ClientManager - Shutting down...";

    if (clients.count() == 0) {
        qApp->quit();
    }
}

void ClientManager::clientStarted()
{
}


void ClientManager::clientFinished()
{
    ClientThread *client = static_cast<ClientThread *>(sender());

    removeDirectoryRecursive(QString(IMAGESDIR) + QDir::separator() + client->getId());
    clients.remove(client);
    client->exit();
    client->wait();
    qDebug() << "INFO: ClientManager - Client" << client->getId() << "finished, number of active clients:" << clients.count();
    delete client;
    if (shutdown) {
        if (clients.count() == 0) {
            qApp->quit();
        }
    }
}

void ClientManager::checkConsistency()
{
    // check that themedaemon has not crashed
    if (themedaemon.state() != QProcess::Running) {
        qWarning() << "ERROR: ClientManager (consistency check)- Themedaemon is not running";
        qDebug() << themedaemon.readAllStandardError();
        qDebug() << themedaemon.readAllStandardOutput();

        shutdown = true;
        foreach(ClientThread * thread, clients) {
            thread->exit();
            thread->wait();
            delete thread;
        }
        qApp->quit();
    }

    if (shutdown == false) {
        // spawn new clients if there is room
        while (clients.count() < ClientManager::MAX_CLIENT_COUNT) {
            spawnClient();
        }
        // get ready to perform another consistency check
        QTimer::singleShot(5000, this, SLOT(checkConsistency()));
    }
}

void ClientManager::changeThemeAndLocale()
{
#ifdef HAVE_GCONF
    // get list of themes
    QDir themeDirectory(THEME_ROOT_DIRECTORY);
    QStringList list = themeDirectory.entryList(QDir::Dirs|QDir::NoDotAndDotDot);

    if(list.size() == 0)
        return;

    int themeIndex = rand() % list.size();

#ifdef PRINT_INFO_MESSAGES
    qDebug() << "INFO: Changing theme to:" << list[themeIndex];
#endif
    MGConfItem themeName("/meegotouch/theme/name");
    //themeName.set(list[themeIndex]);

//    int localeIndex = rand() % locales.size();
//    MGConfItem localeName("/meegotouch/i18n/language");
//    localeName.set(locales[localeIndex]);

    // change theme and locale again after a short period of time
    QTimer::singleShot(2000, this, SLOT(changeThemeAndLocale()));
#endif
}

void ClientManager::pixmapReady(const QString& theme, TestClient* client, quint32 handle, const QString& imageId, const QSize& size)
{
    if(!verifyPixmap(theme, client, handle, imageId, size)) {
        qWarning() << "ERROR:" << client->getId() << "- incorrect color found when verifying returned pixmap (" << imageId << ')';
    } else {
#ifdef PRINT_INFO_MESSAGES
        qDebug() << "INFO:" << client->getId() << "- pixmap comparison OK (" << imageId << ')';
#endif
    }
    client->pixmapVerified(imageId, size);
}

bool ClientManager::verifyPixmap(const QString& theme, TestClient* client, quint32 handle, const QString& imageId, const QSize& size)
{
    // this is what we got from daemon
    QPixmap daemon = QPixmap::fromX11Pixmap(handle, QPixmap::ImplicitlyShared);

    // this is what we have
    QPixmap clientPixmap(size);

    QDir themeDirectory(THEME_ROOT_DIRECTORY);

    // either icon-<xx> or just <xx>
    if(imageId.startsWith("icon"))
    {
#ifdef HAVE_GCONF
//        MGConfItem localeName("/meegotouch/i18n/language");
//        QString locale = localeName.value().toString();
        QDir iconDirectory;
//        if(locale.isEmpty())
        {
            iconDirectory = QDir(theme + QDir::separator() +
                                 QString("meegotouch") + QDir::separator() + QString("icons"));
        }
//        else
//        {
//            iconDirectory = QDir(theme + QDir::separator() +
//                                 QString("meegotouch") + QDir::separator() +
//                                 QString("locale") + QDir::separator() +
//                                 locale + QDir::separator() +
//                                 QString("icons"));
//        }

        // create svg renderer
        QString filename = iconDirectory.absolutePath() + QDir::separator() + imageId + ".svg";
        QSvgRenderer renderer(filename);
        if(!renderer.isValid())
        {
            qWarning() << "ERROR: Failed to construct SVG renderer for:" << filename;
            return false;
        }
        // render pixmap
        QPainter painter(&clientPixmap);
        renderer.render(&painter);
#else
        return true;
#endif
    }
    else
    {
        QDir imageDirectory = theme + QDir::separator() + client->getImageDirectory();
        QString filename = imageDirectory.absolutePath() + QDir::separator() + imageId + ".png";
        if(!clientPixmap.load(filename, "PNG"))
            qWarning() << "ERROR: Failed to construct PNG image:" << filename;
    }

    // make sure that the pixel in the center of the pixmap is equal (these are always one-color images)
    QImage d = daemon.toImage();
    QImage c = clientPixmap.toImage();

    QRgb color = d.pixel(size.width() / 2, size.height() / 2);
    QRgb color2 = c.pixel(size.width() / 2, size.height() / 2);

    if(color != color2) {
        qWarning() << "ERROR: Colors don't match:" << theme << QColor(color) << QColor(color2);
        return false;
    }

    return true;
}
