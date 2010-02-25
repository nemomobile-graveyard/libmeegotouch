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

#ifndef DUIEXTENSIONHANDLE_H
#define DUIEXTENSIONHANDLE_H

#include <DuiWidgetController>

#include <QPixmap>
#include <QProcess>
#include <QSizeF>
#include <QDataStream>
#include <QLocalServer>
#include <QTime>
#include "duiextensionhandlemodel.h"

class DuiAppletCommunicator;
class DuiAppletMessage;
class DuiExtensionHandlePrivate;
class QTimer;

//! \internal

/*!
 * @class DuiExtensionHandle
 *
 * Interfaces with the actual extension that resides in another process. Handle starts the
 * 'runner' process that opens the actual .so extension and knows how to deal with it internally.
 *
 * If the extension insists that its minimum and preferred size hints are larger
 * than the width of the screen DuiExtensionHandle will draw the extension pixmap
 * downscaled so that it fits the screen.
 */
class DuiExtensionHandle : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiExtensionHandle)

    Q_PROPERTY(DuiExtensionHandleModel::State state READ state)

public:
    /*!
     * Constructs a DuiExtensionHandle.
     *
     * \param parent the parent QGraphicsItem for the handle, defaults to \c NULL
     */
    DuiExtensionHandle(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the DuiExtensionHandle.
     */
    virtual ~DuiExtensionHandle();

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
     * If the remote process fails to respond to the request in time, DuiExtensionHandle assumes that the remote process
     * has become unresponsive and will emit the notResponding - signal \see notResponding.
     * The timeout defaults to 3000 milliseconds.
     *
     * \param timeout Timeout in milliseconds to wait for remote process to respond to an alive request.
     */
    void setAliveResponseTimeout(uint timeout);

    /*!
     * Sends the message of type \c DuiAppletSetGeometryMessage about changed geometry.
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
    DuiExtensionHandleModel::State state() const;

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
    DuiExtensionHandle(DuiExtensionHandlePrivate *dd, DuiExtensionHandleModel *model, QGraphicsItem *parent);

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
    void messageReceived(const DuiAppletMessage &message);

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
     */
    void visibilityEvent(bool visible);

    /*!
     * \brief A slot for notifying that the orientation has changed
     */
    void orientationEvent(const Dui::Orientation &);

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
    Q_DECLARE_PRIVATE(DuiExtensionHandle)

    //! Sets the state to BROKEN and restarts if this is the first time
    void setBrokenState();

    //! Displays the list of actions on the host process
    void displayContextMenu(QList<QString> actions);

#ifdef UNIT_TEST
    friend class Ut_DuiExtensionHandle;
#endif
};

//! \internal_end

#endif // DUIEXTENSIONHANDLE_H
