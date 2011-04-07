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

#ifndef MCOMPONENTDATA_H
#define MCOMPONENTDATA_H

#include <QObject>

#include "mexport.h"
#include "mnamespace.h"

class QUrl;
class MComponentDataPrivate;
class MWindow;
class MApplicationWindow;
class MApplicationService;
class MFeedbackPlayer;

/*!
 * \class MComponentData
 * \brief MComponentData manages the GUI application's control flow and main settings.
 *
 * MComponentData provides all the settings that are necessary for a QApplication in
 * order to initialize the MeeGo Touch UI framework. As such it is used by MApplication,
 * which adds the event loop. MComponentData can be used by plain Qt applications
 * in order to provide access to resources of the MeeGo Touch UI framework.
 *
 * MComponentData instance automatically extracts application name from the arguments
 * given in the constructor. It also loads css - stylesheet, svg file and adds application
 * specific image paths to the pixmap search paths used by the current MTheme instance.
 *
 * CSS and SVG files are searched for in order from the following places:
 * <ol>
 *   <li>Directory in which application was launched.</li>
 *   <li>Application specific theme path (global theme path appended with the application name)</li>
 *   <li>Global theme path (Qt data path appended by themes/m)</li>
 * </ol>
 * When first instance of CSS or SVG file is found it is loaded to the current MTheme.
 * Also image - subdirectory of each of the aforementioned paths is appended to the pixmap search
 * paths used by MTheme.
 *
 * MComponentData will also create a QDBus service with name made by prepending 'com.nokia.' to
 * the application name provided in the constructor (or the binary name if no name is provided
 * in the constructor). It will provide an interface called MComponentDataIf which calls service
 * methods in an instance of MApplicationService. By default, MComponentData will construct an
 * instance of MApplicationService, but the application programmer can derive a class from
 * MApplicationService and provide a pointer to that in the constructor for MComponentData to
 * use instead. This way, the application programmer can override the methods in
 * MComponentDataSerivce and change the behaviour of the application's interface.
 *
 * For some applications, it is necessary to implement a custom MApplicationService. See
 * MApplicationService for more information.
 *
 */

class M_CORE_EXPORT MComponentData : public QObject
{
    Q_OBJECT

public:
    //! Initializes the window system and constructs an application object.
    /*!
     * \param argc number of arguments passed to the application from the command line
     * \param argv argument strings passed to the application from the command line
     * \param appIdentifier an optional identifier for the application. Can
     * contain alphabetical characters, numbers, dashes and underscores. If
     * an empty string is given (the default) the application binary file
     * name is used.
     */
    // \deprecated Ctor is deprecated for public usage and should be made protected during next API break.
    MComponentData(int &argc, char **argv, const QString &appIdentifier = QString(), MApplicationService *service = 0);
    explicit MComponentData(MApplicationService *service);

    //! Cleans up any window system resources that were allocated by this application.
    virtual ~MComponentData();

    //! returns the MComponentData instance or 0 if no instance is created yet
    /*!
     * \see createInstance()
     */
    static MComponentData *instance();

    //! returns the MComponentData instance
    /*!
     * always returns a MComponentData pointer
     * if no instance was created yet, it creates a new instance and returns the pointer
     * if an instance was already created it returns the pointer to the existing instance
     * \see instance()
     */
    static MComponentData* createInstance(int &argc, char **argv, const QString &appIdentifier = QString(), MApplicationService *service = 0);
    static MComponentData* createInstance(MApplicationService *service);

    //! Target device name
    static QString deviceName();
    //! Software rendering command line option set
    static bool softwareRendering();
    //! Full screen command line option set
    static bool fullScreen();
    //! Show bounding rectangles command line option set
    static bool showBoundingRect();
    //! Show frame rate command line option set
    static bool showFps();
    //! Log frame rate command line option set
    static bool logFps();
    //! Show widgets sizes
    static bool showSize();
    //! Show widgets positions
    static bool showPosition();
    //! Show widgets margins
    static bool showMargins();
    //! Show object names
    static bool showObjectNames();
    //! Show style names
    static bool showStyleNames();
    //! Show cursor
    static bool showCursor();
    //! Return true if prestarted state active
    static bool prestarted();
    //! Returns the prestart mode
    static M::PrestartMode prestartMode();
    //! Emulate two finger gestures
    static bool emulateTwoFingerGestures();
    //! Sets if position of widgets should be shown or not
    static void setShowPosition(bool show);
    //! Sets if margins of widgets should be shown or not
    static void setShowMargins(bool show);
    //! Sets if names of objects should be shown or not
    static void setShowObjectNames(bool show);
    //! Sets if names of styles should be shown or not
    static void setShowStyleNames(bool show);
    //! Sets if sizes of widgets should be shown or not
    static void setShowSize(bool show);
    //! Sets if bounding rects of widgets should be shown or not
    static void setShowBoundingRect(bool show);
    //! Sets if a frames-per-second counter should be shown or not
    static void setShowFps(bool show);
    //! Sets if whether fps logging should be enabled or not
    static void setLogFps(bool show);
    //! Sets if the cursor should be shown or not
    static void setShowCursor(bool show);
    //! Sets the prestarted flag
    static void setPrestarted(bool flag);
    //! Sets the prestart mode
    static void setPrestartMode(M::PrestartMode mode);
    //! Set if two finger gestures should be emulated or not.
    static void setEmulateTwoFingerGestures(bool flag);

    /*!
     * Set the syslog server to log to:
     * \param url can point to either a local domain socket, for example
     * 'file:///dev/log', or a remote server specified as 'udp://servername[:port]'.
     * \note It is not safe to change the syslog server, while your process has
     * more than one active threads.
     */
    static bool setSyslogServer(const QUrl &url);

    /*!
     * Returns the currently active application window.
     * \note If the active window is of type MWindow (and not MApplicationWindow)
     *       this method will return a null pointer.
     * \sa activeWindow()
     */
    static MApplicationWindow *activeApplicationWindow();
    //! Returns the currently active window
    static MWindow *activeWindow();
    //! Returns a list of all windows in the application
    static QList<MWindow *> windows();

    //! Returns the application's app name
    static QString appName();
    //! Returns the application's binary name
    static QString binaryName();
    //! Returns the application's service name
    static QString serviceName();

    //! Returns object which provide interface for nonvisual feedback or NULL
    //
    // \deprecated Please use class MFeedback to play input feedbacks. Since 0.20.43
    static MFeedbackPlayer *feedbackPlayer();

    //! Returns whether automatic loading of MInputContext is enabled
    static bool isLoadMInputContextEnabled();

    //! Sets whether MComponentData should automatically load m input context
    static void setLoadMInputContext(bool enable);

    //! Returns whether MCompositor is present
    static bool isMeeGoWindowManagerRunning();

    //! \internal
    static M::OrientationAngle forcedOrientationAngle();

    static bool isOrientationForced();

#ifdef Q_WS_X11
    typedef QPair<WId,QString> ChainData;

    /**
     * Pushes a ChainData onto the stack of chain data
     * \param chain data
     */
    static void pushChainData(const ChainData &chainData);

    /**
     * Pops a ChainData off the stack of chained window ids
     */
    static ChainData popChainData();

    /**
     * Returns if the chained ChainData stack is empty
     */
    static bool chainDataStackIsEmpty();

    /**
     * Returns the WindowId of the last chain parent.
     * Make sure you check that there is chain task data
     * available with " ! chainDataStackIsEmpty() " before
     * calling this function. Returns 0 when there is no
     * chain data available.
     */
    static WId lastChainParentWinId();
#endif // Q_WS_X11
    //! \internal_end

Q_SIGNALS:
    /*!
     * \brief Signal emitted when the default system locale changes.
     */
    void localeSettingsChanged();

    void chainTaskDataChanged();

protected:
    MComponentData(MComponentDataPrivate &dd, int &argc, char **argv, const QString &appIdentifier = QString());
    MComponentDataPrivate *const d_ptr;

private:
    static MComponentData *self;
    static void registerWindow(MWindow *);
    static void unregisterWindow(MWindow *);
    static void setActiveWindow(MWindow *);

    void reinit(int &argc, char **argv, const QString &appIdentifier = QString(), MApplicationService *service = 0);

    Q_DECLARE_PRIVATE(MComponentData)
    Q_DISABLE_COPY(MComponentData)
    #ifdef HAVE_GCONF
    Q_PRIVATE_SLOT(d_func(), void _q_updateDebugOptionsFromGConfValues())
    #endif
    Q_PRIVATE_SLOT(d_func(), void _q_notifyInputMethodActiveWindowOrientationChangeStarted())
    Q_PRIVATE_SLOT(d_func(), void _q_notifyInputMethodActiveWindowOrientationChangeFinished())

    friend class MWindow;
    friend class MApplicationWindow;
    friend class MSceneWindow;
    friend class MTheme;
    friend class MDeviceProfile;
    friend class MComponentCachePrivate;
    friend class MStyleSheet;
};

#endif
