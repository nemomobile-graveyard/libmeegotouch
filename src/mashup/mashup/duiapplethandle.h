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

#ifndef DUIAPPLETHANDLE_H
#define DUIAPPLETHANDLE_H

#include <DuiWidgetController>
#include "duiappletinstancemanager.h"

#include <QPixmap>
#include <QProcess>
#include <QSizeF>
#include <QDataStream>
#include <QLocalServer>
#include <QTime>
#include "duiapplethandlemodel.h"
#include "duiappletid.h"

class DuiAppletCommunicator;
class DuiAppletMessage;
class DuiAppletHandlePrivate;
class QTimer;

//! \internal

/*!
 * @class DuiAppletHandle
 *
 * Interfaces with the actual applet that resides in another process. Handle starts the
 * 'runner' process that opens the actual .so applet and knows how to deal with it internally.
 *
 * If the applet insists that its minimum and preferred size hints are larger
 * than the width of the screen DuiAppletHandle will draw the applet pixmap
 * downscaled so that it fits the screen.
 */
class DuiAppletHandle : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiAppletHandle)
    Q_PROPERTY(QString appletIcon READ appletIcon WRITE setAppletIcon NOTIFY appletIconChanged)
    Q_PROPERTY(QString appletTitle READ appletTitle WRITE setAppletTitle NOTIFY appletTitleChanged)
    Q_PROPERTY(QString appletText READ appletText WRITE setAppletText NOTIFY appletTextChanged)

    Q_PROPERTY(DuiAppletHandleModel::AppletState state READ state)

public:
    /*!
     * Constructs an DuiAppletHandle.
     */
    DuiAppletHandle();

    /*!
     * Destroys the DuiAppletHandle.
     */
    virtual ~DuiAppletHandle();

    /*!
     * Initializes the handle and launches runner process.
     *
     * \param runnerBinary the runner binary to be used
     * \param appletInstanceFileDataPath path to data file used to store instance specific data about this applet.
     * \param metaDataFileName applet metadata file name
     * \param appletId Applet global identifier
     */
    void init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId);

    /*!
     * Initializes the place holder handle. Sets the applet handle to INSTALLING state if no error has occurred or to BROKEN state if an error has occurred.
     *
     * \param appletId Applet global identifier
     * \param packageName Package of which installation progress should be tracked by the handle
     * \param installationError An empty string if no error has occurred or the reason for the error if an error has occurred
     */
    void initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError = QString());

    /*!
     * Stops the handle's runner process.
     */
    void kill();

    /*!
     * Reinitializes the handle by first stopping the current process and then
     * restarting it.
     */
    void reinit();

    /*!
     * Sets the timeout that applet handle waits for applet instance to respond to applet alive requests.
     * If the applet instance fails to respond to the request in time, DuiAppletHandle assumes that the applet instance
     * has become unresponsive and will emit the appletNotResponding - signal \see appletNotResponding.
     * The timeout defaults to 3000 milliseconds.
     *
     * \param timeout Timeout in milliseconds to wait for applet instance to respond to an applet alive request.
     */
    void setAliveResponseTimeout(uint timeout);

    /*!
     * Sends the applet a message of type \c DuiAppletSetGeometryMessage about changed geometry.
     * \param appletRect the new size of the applet
     * \param pixmapHandle the X pixmap handle the applet should use
     */
    void sendGeometryMessage(QRectF appletRect, Qt::HANDLE pixmapHandle);

    /*!
     * Sends a signal to the instance manager to remove this applet
     */
    void removeApplet();

    /*!
     * Stops the communication to the applet runner.
     */
    void stopCommunication();

    /*!
     * Sets the applet specific actions to \p items. Any previous applet
     * specific actions are removed. Initially the applet specific actions
     * list is empty.
     * \param items a list of the names of the applet specific actions.
     */
    void setAppletSpecificActions(QList<QString> items = QList<QString>());

    /*!
     * Returns the state of the applet handle.
     */
    DuiAppletHandleModel::AppletState state() const;

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

    /*!
     * Sets the size hints of the applet. The purpose of this is to set the
     * size hints while the applet is not yet running.
     *
     * \param sizeHints the size hints for the applet
     */
    void setSizeHints(const QVector<QSizeF> &sizeHints);

protected:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    //! \reimp_end

protected slots:
    /*!
     * \brief A slot that gets called when the communication channel to the applet
     * runner binary has been established.
     */
    void connectionEstablished();

    /*!
     * \brief A slot for running the applet process
     */
    void run();

    /*!
     * \brief A slot for notifying that a message has been received from the applet
     */
    void messageReceived(const DuiAppletMessage &message);

    /*!
     * \brief A slot for notifying that communication between applet handle and applet runner has dropped.
     * This slot can be called if establishing of communication between applet handle and applet runner failed
     * or if communication is dropped during the lifetime of the applet.
     */
    void communicationTimerTimeout();

    /*!
     * \brief A slot for notifying that an alive request should be sent to the applet
     */
    void sendAliveMessageRequest();

    /*!
     * \brief A slot for notifying that the visibility of the applet has changed due to hide()/show()
     */
    void visibilityEvent(bool visible);

    /*!
     * \brief A slot for notifying that the orientation has changed
     */
    void orientationEvent(const Dui::Orientation &);

    /*!
     * \brief A slot for notifying that there is data to be read from the applet's standard error stream
     */
    void processStdErrorReady();

    /*!
     * \brief A slot for notifying that there is data to be read from the applet's standard output stream
     */
    void processStdOutputReady();

    /*!
     * \brief A slot for handling errors in the applet runner process.
     * \param error the error code.
     */
    void processError(QProcess::ProcessError error);

    /*!
     * \brief A slot for notifying that the applet process has finished
     */
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    /*!
     * A slot to be called when an applet specific action gets triggered.
     * Sends a message to the applet to notify about the action triggering.
     */
    void appletSpecificActionTriggered();

signals:
    /*!
     * Signal that is emitted to indicate that the applet removal is requested
     * \param appletId The appletId object
     */
    void appletRemovalRequested(DuiAppletId appletId);

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

    /*!
     * \brief Signals that a pixmap has been taken into use by the applet
     */
    void pixmapTakenIntoUse(Qt::HANDLE pixmapHandle);

    /*!
     * \brief Content's of the applet's pixmap have changed
     */
    void appletPixmapModified(const QRectF &rect);

private:
    Q_DECLARE_PRIVATE(DuiAppletHandle)

    //! Sets the applet state to BROKEN and restarts it if this is the first time the applet broke
    void setAppletBrokenState();

    //! Displays the list of actions on the host process
    void displayContextMenu(QList<QString> actions);

#ifdef UNIT_TEST
    friend class Ut_DuiAppletHandle;
#endif
};

//! \internal_end

#endif // DUIAPPLETHANDLE_H
