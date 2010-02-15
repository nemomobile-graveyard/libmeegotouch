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

#ifndef DUIAPPLETRUNNER_H
#define DUIAPPLETRUNNER_H

#include <QObject>
#include <QRectF>
#include <QPixmap>

#include <duinamespace.h>

class DuiWidget;
class DuiScene;
class DuiWindow;
class QGraphicsWidget;
class DuiAppletClient;
class DuiAppletMessage;
class DuiAppletMouseMessage;
class DuiAppletCancelMessage;
class DuiFileDataStore;
class DuiAppletMetaData;
class DuiAppletInterface;
class DuiAppletSharedMutex;
class DuiDataAccess;
class DuiGConfDataStore;
class DuiSettingsLanguageBinary;
class DuiAppletSettings;
class QAction;
#ifdef QT_OPENGL_LIB
class QGLContext;
#endif


/**
 * \ingroup appletrunner
 *
 * DuiAppletRunner is a class that is used to start and run oop (out of process)
 * applet widgets.
 */
class DuiAppletRunner : public QObject
{
    Q_OBJECT

public:
    /*!
     * DuiAppletRunner constructor
     */
    DuiAppletRunner();

    //! DuiAppletRunner destructor
    virtual ~DuiAppletRunner();

    /*!
     * Initialize the applet and establish an IPC connection between the runner and
     * the host application.
     * \param appletId applet ID as a string
     * \param appletInstanceFileDataPath path to data file used to store instance specific data about this applet.
     * \param metaData Metadata of the applet.
     * \param serverName the name of the host process's server.
     * \return \c true if the applet and ipc connection was initialized correctly,
     *         \c false if something went wrong.
     */
    bool init(const QString &appletId, const QString &appletInstanceFileDataPath, DuiAppletMetaData &metaData, const QString &serverName);

    /*!
     * Sends a message of type DuiAppletUpdateGeometryMessage to the host.
     */
    void sendUpdateGeometryMessage();

private:
    //! AppletClient for communicating between the applet and the host processes.
    DuiAppletClient *communicator;

    //! A parent widget for the actual applet widget
    QGraphicsWidget *parentWidget;

    //! The applet widget instantiated from shared library
    DuiWidget *widget;

    //! Scene where the widget is inserted.
    DuiScene *scene;

    //! View into the scene.
    DuiWindow *view;

    /*! Pixmap in which the applet is painted. The underlying X pixmap is
        shared between the host and applet processes. */
    QPixmap appletPixmap;

    //! Data for the applet instance
    DuiFileDataStore *appletInstanceDataStore;

    //! List to hold object menu actions received from widget.
    QList<QAction *> widgetObjectMenuActions;

#ifdef QT_OPENGL_LIB
    //! OpenGL context
    QGLContext *context;
#endif

    //! Timer for checking that the host process is alive
    QTimer *aliveTimer;

    //! Shared mutex for locking the pixmap
    DuiAppletSharedMutex *pixmapMutex;

    //! An interface for applets to access their settings
    DuiAppletSettings *appletSettings;

    //! Combined changed rect for the scene
    QRectF changedRect;

    //! Visibility status of the applet
    bool appletVisible;

Q_SIGNALS:
    /*!
     * \brief A signal that is emitted when item is shown/hidden
     */
    void visibilityChanged(bool visibility);

private slots:
    /*!
     * \brief A slot for receiving messages from the applet server
     */
    void messageReceived(const DuiAppletMessage &message);

    /*!
     * \brief A slot for notifying that the scene has changed and needs to be painted
     */
    void sceneChanged(const QList<QRectF> &region);

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
     * \brief Slot to be called when the host process has not sent any messages in ALIVE_TIMER_TIMEOUT milliseconds
     */
    void hostProcessNotAlive();

private:
    /*!
     * Sets the icon, title and text properties of an applet and connects any
     * related signals to slots so the changes in the properties are handled
     * correctly.
     */
    void setAppletProperties();

    /*!
     * Frees all resources that have been reserved by this object currently.
     */
    void teardown();

    /*!
     * Handles an AppletMouseMessage.
     *
     * \param message the AppletMouseMessage to handle
     */
    void handleMouseEvent(const DuiAppletMouseMessage *message);

    /*!
     * Handles an AppletCancelMessage.
     *
     * \param message to be handled
     */
    void handleCancelEvent(const DuiAppletCancelMessage *message);

    /*!
     * Sets appletPixmap to use the received X pixmap handle
     *
     * \param handle the X pixmap handle to use
     */
    void createAppletPixmap(Qt::HANDLE handle);
};

#endif // DUIAPPLETRUNNER_H
