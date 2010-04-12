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

#ifndef MAPPLETHANDLE_H
#define MAPPLETHANDLE_H

#include "mextensionhandle.h"
#include "mapplethandlemodel.h"
#include "mappletid.h"

class MAppletHandlePrivate;

//! \internal

/*!
 * @class MAppletHandle
 *
 * Interfaces with the actual applet that resides in another process. Handle starts the
 * 'runner' process that opens the actual .so applet and knows how to deal with it internally.
 *
 * If the applet insists that its minimum and preferred size hints are larger
 * than the width of the screen MAppletHandle will draw the applet pixmap
 * downscaled so that it fits the screen.
 */
class MAppletHandle : public MExtensionHandle
{
    Q_OBJECT
    M_CONTROLLER(MAppletHandle)
    Q_PROPERTY(QString appletIcon READ appletIcon WRITE setAppletIcon NOTIFY appletIconChanged)
    Q_PROPERTY(QString appletTitle READ appletTitle WRITE setAppletTitle NOTIFY appletTitleChanged)
    Q_PROPERTY(QString appletText READ appletText WRITE setAppletText NOTIFY appletTextChanged)

public:
    /*!
     * Constructs an MAppletHandle.
     */
    MAppletHandle(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the MAppletHandle.
     */
    virtual ~MAppletHandle();

    /*!
     * Initializes the handle and launches runner process.
     *
     * \param runnerBinary the runner binary to be used
     * \param appletInstanceFileDataPath path to data file used to store instance specific data about this applet.
     * \param metaDataFileName applet metadata file name
     * \param appletId Applet global identifier
     */
    void init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const MAppletId &appletId);

    /*!
     * Initializes the place holder handle. Sets the applet handle to INSTALLING state if no error has occurred or to BROKEN state if an error has occurred.
     *
     * \param appletId Applet global identifier
     * \param packageName Package of which installation progress should be tracked by the handle
     * \param installationError An empty string if no error has occurred or the reason for the error if an error has occurred
     */
    void initPlaceHolder(const MAppletId &appletId, const QString &packageName, const QString &installationError = QString());

    /*!
     * Stops the handle's runner process.
     */
    void kill();

    /*!
     * Sends a signal to the instance manager to remove this applet
     */
    void removeApplet();

    /*!
     * Sets the icon ID of the applet.
     *
     * \param newIcon the new icon ID
     */
    void setAppletIcon(const QString &newIcon);

    /*!
     * Sets the title of the applet.
     *
     * \param newTitle the new title
     */
    void setAppletTitle(const QString &newTitle);

    /*!
     * Sets the text of the applet.
     *
     * \param newText the new text
     */
    void setAppletText(const QString &newText);

    /*!
     * Returns the icon ID of the applet.
     *
     * \return the icon ID of the applet
     */
    QString appletIcon() const;

    /*!
     * Returns the title of the applet.
     *
     * \return the title of the applet
     */
    QString appletTitle() const;

    /*!
     * Returns the text of the applet.
     *
     * \return the text of the applet
     */
    QString appletText() const;

protected slots:
    /*!
     * \brief A slot for notifying that a message has been received from the applet
     */
    void messageReceived(const MAppletMessage &message);

signals:
    /*!
     * Signal that is emitted to indicate that the applet removal is requested
     * \param appletId The appletId object
     */
    void appletRemovalRequested(MAppletId appletId);

    /*!
     * \brief Applet changed its icon, part of container communication API
     */
    void appletIconChanged(const QString &newIcon);

    /*!
     * \brief Applet changed its title, part of container communication API
     */
    void appletTitleChanged(const QString &newTitle);

    /*!
     * \brief Applet changed its additional text, part of container communication API
     */
    void appletTextChanged(const QString &newText);

private:
    Q_DECLARE_PRIVATE(MAppletHandle)

#ifdef UNIT_TEST
    friend class Ut_MAppletHandle;
#endif
};

//! \internal_end

#endif // MAPPLETHANDLE_H
