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

#ifndef MEXTENSIONHANDLE_H
#define MEXTENSIONHANDLE_H

#include <MWidgetController>

#include <QPixmap>
#include <QProcess>
#include <QSizeF>
#include <QDataStream>
#include <QLocalServer>
#include <QTime>
#include "mextensionhandlemodel.h"

class MAppletCommunicator;
class MAppletMessage;
class MExtensionHandlePrivate;
class QTimer;

//! \internal

/*!
 * @class MExtensionHandle
 *
 * Interfaces with the actual extension that resides in another process. Handle starts the
 * 'runner' process that opens the actual .so extension and knows how to deal with it internally.
 *
 * If the extension insists that its minimum and preferred size hints are larger
 * than the width of the screen MExtensionHandle will draw the extension pixmap
 * downscaled so that it fits the screen.
 *
 * When the remote runner process dies the handle needs to either restart
 * or go to a broken state. To detect that whether the runner is not
 * responding/crashed (become a zombie), the handle sends
 * APPLET_ALIVE_MESSAGE_REQUEST (pings) to the runner which replies to the
 * requests with APPLET_ALIVE_MESSAGE_RESPONSE (pongs). The handle starts a
 * timer after sending each ping request to check if the response is received
 * in a certain time interval. If not, then it means the runner is not
 * responding and it is set to a "broken" state.
 *
 * The handling of the broken state goes currently like this:
 *
 * - If the handle enters the broken state the first time, it is simply restarted.
 * - If the handle enters the broken state a second time withing a given interval (currently 30 seconds) user interaction is required. It is up to the derived classes to determine what to do in this case.
 * - If the handle doesn't enter the broken state within the time interval, it is considered to behave correctly and it re-enters the normal running state.
 */
class MExtensionHandle : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MExtensionHandle)

    Q_PROPERTY(MExtensionHandleModel::State state READ state)

public:
    /*!
     * Constructs a MExtensionHandle.
     *
     * \param parent the parent QGraphicsItem for the handle, defaults to \c NULL
     */
    MExtensionHandle(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the MExtensionHandle.
     */
    virtual ~MExtensionHandle();

    /*!
     * Initializes the handle and launches runner process.
     *
     * \param runnerBinary the runner binary to be used
     * \param metaDataFileName metadata file name
     * \param extraArguments arguments to be appended to the command line arguments
     */
    virtual void init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &extraArguments = QStringList());

    /*!
     * Initializes the place holder handle. Sets the handle to INSTALLING state if no error has occurred or to BROKEN state if an error has occurred.
     *
     * \param packageName Package of which installation progress should be tracked by the handle
     * \param installationError An empty string if no error has occurred or the reason for the error if an error has occurred
     */
    virtual void initPlaceHolder(const QString &packageName, const QString &installationError = QString());

    /*!
     * Stops the handle's runner process.
     */
    virtual void kill();

    /*!
     * Reinitializes the handle by first stopping the current process and then
     * restarting it.
     */
    void reinit();

    /*!
     * Sets the timeout that handle waits for the remote process to respond to alive requests.
     * If the remote process fails to respond to the request in time, MExtensionHandle assumes that the remote process
     * has become unresponsive and will emit the notResponding - signal \see notResponding.
     * The timeout defaults to 3000 milliseconds.
     *
     * \param timeout Timeout in milliseconds to wait for remote process to respond to an alive request.
     */
    void setAliveResponseTimeout(uint timeout);

    /*!
     * Sends the message of type \c MAppletSetGeometryMessage about changed geometry.
     * \param rect the new size
     * \param pixmapHandle the X pixmap handle the runner should use
     */
    void sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle);

    /*!
     * Stops the communication to the runner.
     */
    void stopCommunication();

    /*!
     * Sets the remote actions to \p items. Any previous remote actions are
     * removed. Initially the remote actions list is empty.
     * \param items a list of the names of the remote actions.
     */
    void setRemoteActions(QList<QString> items = QList<QString>());

    /*!
     * Returns the state of the handle.
     */
    MExtensionHandleModel::State state() const;

    /*!
     * Sets the size hints of the handle. The purpose of this is to set the
     * size hints while the runner is not yet running.
     *
     * \param sizeHints the size hints for the handle
     */
    void setSizeHints(const QVector<QSizeF> &sizeHints);

protected:
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     */
    MExtensionHandle(MExtensionHandlePrivate *dd, MExtensionHandleModel *model, QGraphicsItem *parent);
    MExtensionHandlePrivate *const d_ptr;

    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    //! \reimp_end

protected slots:
    /*!
     * \brief A slot that gets called when the communication channel to the
     * runner binary has been established.
     */
    void connectionEstablished();

    /*!
     * \brief A slot for running the remote process
     */
    void run();

    /*!
     * \brief A slot for notifying that a message has been received from the remote process
     */
    void messageReceived(const MAppletMessage &message);

    /*!
     * \brief A slot for notifying that communication between the handle and the runner has dropped.
     * This slot can be called if establishing of communication between the handle and the runner failed
     * or if communication is dropped during the lifetime of the extension.
     */
    void communicationTimerTimeout();

    /*!
     * \brief A slot for notifying that an alive request should be sent to the remote process
     */
    void sendAliveMessageRequest();

    /*!
     * \brief A slot for notifying that the visibility of the extension has changed due to hide()/show()
     * TODO: Remove this redundant slot in next API/ABI break window.
     */
    void visibilityEvent(bool visible);

    /*!
     * \brief A slot for notifying that the visibility of the extension has changed due to hide()/show()
     */
    void visibilityChanged();

    /*!
     * \brief A slot for notifying that the orientation has changed
     */
    void orientationEvent(M::Orientation);

    /*!
     * \brief A slot for notifying that there is data to be read from the runner's standard error stream
     */
    void processStdErrorReady();

    /*!
     * \brief A slot for notifying that there is data to be read from the runner's standard output stream
     */
    void processStdOutputReady();

    /*!
     * \brief A slot for handling errors in the runner process.
     * \param error the error code.
     */
    void processError(QProcess::ProcessError error);

    /*!
     * \brief A slot for notifying that the runner process has finished
     */
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    /*!
     * A slot to be called when an remote action gets triggered.
     * Sends a message to the remote process to notify about the action triggering.
     */
    void remoteActionTriggered();

    /*!
     * \brief Stops or starts the alive timer depending on the display blanking state
     */
    void updateDisplayState();

signals:
    /*!
     * \brief Signals that a pixmap has been taken into use by the runner
     */
    void pixmapTakenIntoUse(Qt::HANDLE pixmapHandle);

    /*!
     * \brief Content's of the pixmap have changed
     */
    void pixmapModified(const QRectF &rect);

private:
    Q_DECLARE_PRIVATE(MExtensionHandle)

    //! Initializes the object
    void initialize();

    //! Sets the state to BROKEN and restarts if this is the first time
    void setBrokenState();

    //! Displays the list of actions on the host process
    void displayContextMenu(QList<QString> actions);

#ifdef UNIT_TEST
    friend class Ut_MExtensionHandle;
#endif

    Q_PRIVATE_SLOT(d_func(), void operationComplete( const QString &operation,
                       const QString &pkg, const QString &error) )
    Q_PRIVATE_SLOT(d_func(), void operationProgress( const QString &operation,
                       const QString &pkg, int percentage) )
};

//! \internal_end

#endif // MEXTENSIONHANDLE_H
