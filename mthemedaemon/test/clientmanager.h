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

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QProcess>
#include <QSet>
#include <QSize>

class ClientThread;
class TestClient;

static const QString THEME_ROOT_DIRECTORY = QString("themes");

class ClientManager : public QObject
{
    Q_OBJECT

    static const int MAX_CLIENT_COUNT = 10;

public:
    ClientManager(QProcess &process);
    ~ClientManager();

protected:
    void spawnClient();

private slots:
    void start();
    void stop();

    void checkConsistency();

    void clientStarted();
    void clientFinished();
    void changeThemeAndLocale();

    void pixmapReady(const QString& theme, TestClient* client, quint32 handle, const QString& imageId, const QSize& size);

private:
    void cleanup();
    bool verifyPixmap(const QString& theme, TestClient* client, quint32 handle, const QString& imageId, const QSize& size);

    QStringList locales;
    QSet<ClientThread *> clients;
    bool shutdown;
    QProcess &themedaemon;
};

#endif
