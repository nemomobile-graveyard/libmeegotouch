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

#ifndef UT_DUIAPPLETSERVER_H
#define UT_DUIAPPLETSERVER_H

#include <QObject>

class QLocalSocket;
class DuiAppletServer;

class Ut_DuiAppletServer : public QObject
{
    Q_OBJECT

private:
    QLocalSocket *socket;
    DuiAppletServer *m_subject;
    bool connected;

    void setupServer();

private slots:
    void init();
    void cleanup();

    void testConnection();
    void testReconnect();

public slots:
    void connectionEstablished();
};

#endif // UT_DUIAPPLETSERVER_H
