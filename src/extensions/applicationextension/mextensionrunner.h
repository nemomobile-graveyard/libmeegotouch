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

#ifndef MEXTENSIONRUNNER_H_
#define MEXTENSIONRUNNER_H_

#include <QObject>
#include <QRectF>
#include <QPixmap>

#include <mnamespace.h>
#include <mexport.h>

class MWidget;
class MScene;
class MWindow;
class QGraphicsWidget;
class MAppletClient;
class MAppletMessage;
class MAppletMouseMessage;
class MAppletCancelMessage;
class MAppletMetaData;
class MAppletInterface;
class MAppletSharedMutex;
class QAction;
#ifdef QT_OPENGL_LIB
class QGLContext;
#endif
#ifdef HAVE_CONTEXTSUBSCRIBER
class ContextProperty;
#endif

//! \internal
/**
 * MExtensionRunner is a class that is used to start and run oop (out of process)
 * extension widgets.
 *
 * W A R N I N G
 * -------------
 * This file is not part of the libmeegotouch API. It should only be used by applet runner developers.
 */
class M_EXTENSIONS_EXPORT MExtensionRunner : public QObject
{
    Q_OBJECT

public:
    /*!
     * MExtensionRunner constructor
     */
    MExtensionRunner();

    //! MExtensionRunner destructor
    virtual ~MExtensionRunner();

    /*!
     * Initialize the extension and establish an IPC connection between the runner and
     * the host application.
     *
     * \param serverName the name of the host process's server.
     * \return \c true if the extension and IPC connection were initialized correctly,
     *         \c false if something went wrong.
     */
    virtual bool init(const QString &serverName);

    /*!
     * Sends a message of type MAppletUpdateGeometryMessage to the host.
     */
    virtual void sendUpdateGeometryMessage();

Q_SIGNALS:
    /*!
     * \brief A signal that is emitted when item is shown/hidden
     * TODO: Remove this redundant signal during next API/ABI break
     */
    void visibilityChanged(bool visibility);

    /*!
     * \brief A signal that is emitted when item is shown/hidden
     */
    void visibilityChanged();

protected slots:
    /*!
     * \brief A slot for receiving messages from the host application
     */
    virtual void messageReceived(const MAppletMessage &message);

    /*!
     * \brief A slot for notifying that the scene has changed and needs to be painted
     */
    virtual void sceneChanged(const QList<QRectF> &region);

    /*!
     * \brief Slot to be called when the host process has not sent any messages in ALIVE_TIMER_TIMEOUT milliseconds
     */
    virtual void hostProcessNotAlive();

    /*!
     * \brief Stops or starts the alive timer depending on the display blanking state
     */
    void updateDisplayState();

protected:
    //! AppletClient for communicating between the runner and the host processes.
    MAppletClient *communicator;

    //! A parent widget for the actual extension widget
    QGraphicsWidget *parentWidget;

    //! The extension widget instantiated from shared library
    QGraphicsWidget *widget;

    //! Scene where the widget is inserted.
    MScene *scene;

    //! View into the scene.
    MWindow *view;

    /*! Pixmap in which the extension is painted. The underlying X pixmap is
        shared between the host and runner processes. */
    QPixmap pixmap;

    //! List to hold object menu actions received from widget.
    QList<QAction *> widgetObjectMenuActions;

#ifdef HAVE_CONTEXTSUBSCRIBER
    //! Context property for getting the screen blanking status
    ContextProperty *screenBlankProperty;
#endif

    //! True if the display is currently blanked
    bool displayBlanked;

    //! Timer for checking that the host process is alive
    QTimer *aliveTimer;

    //! Shared mutex for locking the pixmap
    MAppletSharedMutex *pixmapMutex;

    //! Combined changed rect for the scene
    QRectF changedRect;

    //! Visibility status of the extension
    bool visible;

    /*!
     * Frees all resources that have been reserved by this object currently.
     */
    virtual void teardown();

    /*!
     * Handles an AppletMouseMessage.
     *
     * \param message the AppletMouseMessage to handle
     */
    virtual void handleMouseEvent(const MAppletMouseMessage *message);

    /*!
     * Handles an AppletCancelMessage.
     *
     * \param message to be handled
     */
    virtual void handleCancelEvent(const MAppletCancelMessage *message);

    /*!
     * Sets pixmap to use the received X pixmap handle
     *
     * \param handle the X pixmap handle to use
     */
    virtual void createAppletPixmap(Qt::HANDLE handle);

#ifdef UNIT_TEST
    friend class Ut_MExtensionRunner;
#endif
};
//! \internal_end

#endif /* MEXTENSIONRUNNER_H_ */
