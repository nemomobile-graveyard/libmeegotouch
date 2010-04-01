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

#ifndef DUIAPPLETCLIENT_H_
#define DUIAPPLETCLIENT_H_

#include "duiexport.h"
#include "duiappletcommunicator.h"


/*!
 * The DuiAppletClient implements a client for interprocess communication
 * between two processes (a host application and an applet).
 */
class DUI_EXPORT DuiAppletClient : public DuiAppletCommunicator
{
    Q_OBJECT

public:
    /*!
     * Constructs a DuiAppletClient.
     */
    DuiAppletClient();

    /*!
     * Destroys the DuiAppletClient.
     */
    virtual ~DuiAppletClient();

    /*!
     * Connects to the server.
     *
     * \param serverName the name of the server
     * \return true if connection setup was successful, false otherwise
     */
    bool connectToServer(const QString &serverName);

    /*!
     * Closes the connection.
     */
    virtual void closeConnection();

private slots:
    /*!
     * \brief A slot that gets called when a communication client manages to establish a connection.
     */
    void connected();

    /*!
     * \brief A slot that gets called if the client side socket encounters an error.
     * \param error the error type.
     */
    void socketError(QLocalSocket::LocalSocketError error);
};

#endif /* DUIAPPLETCLIENT_H_ */
