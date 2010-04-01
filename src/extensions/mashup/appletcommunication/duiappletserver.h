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

#ifndef DUIAPPLETSERVER_H_
#define DUIAPPLETSERVER_H_

#include "duiexport.h"
#include "duiappletcommunicator.h"

class QLocalServer;

/*!
 * The DuiAppletServer implements a server for interprocess communication
 * between two processes (a host application and an applet).
 */
class DUI_EXPORT DuiAppletServer : public DuiAppletCommunicator
{
    Q_OBJECT

public:
    /*!
     * Constructs a DuiAppletServer.
     */
    DuiAppletServer();

    /*!
     * Destroys the DuiAppletServer.
     */
    virtual ~DuiAppletServer();

    /*!
     * Starts the server.
     *
     * \param serverName the name of the server
     * \return true if connection setup was successful, false otherwise
     */
    bool startServer(const QString &serverName);

    /*!
     * Closes the connection.
     */
    virtual void closeConnection();

private:
    //! The local server used for listening to connections
    QLocalServer *server;

private slots:
    /*!
     * \brief A slot that gets called when a new connection has been established by this communicator.
     */
    void newConnection();
};

#endif /* DUIAPPLETSERVER_H_ */
