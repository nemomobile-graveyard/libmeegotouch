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

#ifndef DUIAPPLICATION_H
#define DUIAPPLICATION_H

#include <QApplication>
#ifdef Q_WS_X11
#include <QX11Info>
#endif

#include "duiexport.h"
#include "duinamespace.h"

class DuiApplicationPrivate;
class DuiWindow;
class DuiApplicationWindow;
class DuiApplicationService;
class DuiFeedbackPlayer;
class DuiSceneWindow;


/*!
 * \class DuiApplication
 * \brief DuiApplication manages the GUI application's control flow and main settings.
 *
 * DuiApplication instance automatically extracts application name from the arguments
 * given in the constructor. It also loads css - stylesheet, svg file and adds application
 * specific image paths to the pixmap search paths used by the current DuiTheme instance.
 *
 * CSS and SVG files are searched for in order from the following places:
 * <ol>
 *   <li>Directory in which application was launched.</li>
 *   <li>Application specific theme path (global theme path appended with the application name)</li>
 *   <li>Global theme path (Qt data path appended by themes/dui)</li>
 * </ol>
 * When first instance of CSS or SVG file is found it is loaded to the current DuiTheme.
 * Also image - subdirectory of each of the aforementioned paths is appended to the pixmap search
 * paths used by DuiTheme.
 *
 * DuiApplication will also create a QDBus service with name made by prepending 'com.nokia.' to
 * the application name provided in the constructor (or the binary name if no name is provided
 * in the constructor). It will provide an interface called DuiApplicationIf which calls service
 * methods in an instance of DuiApplicationService. By default, DuiApplication will construct an
 * instance of DuiApplicationService, but the application programmer can derive a class from
 * DuiApplicationService and provide a pointer to that in the constructor for DuiApplication to
 * use instead. This way, the application programmer can override the methods in
 * DuiApplicationService and change the behaviour of the application's interface.
 *
 * The default behaviour is to only allow a single instance of any
 * application. When an application is launched, it attempts to register
 * itself as a dbus service (as above). Only the first instance of the
 * application will be successful, and subsequent attempts will fail. When
 * dbus registration fails, the default behaviour is to call the first
 * instance's DuiApplicationService launch() method, and then quit. This
 * causes the first instance to become visible (raises the window).
 *
 * If other behaviour is required, for example if you want multiple
 * instances of an application, then it is necessary to derive a class from
 * DuiApplicationService and override its methods.
 */

class DUI_EXPORT DuiApplication : public QApplication
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
     *
     * \note Warning: The data referred to by argc and argv must stay valid for the entire
     * lifetime of the DuiApplication object. In addition, argc must be greater than zero and
     * argv must contain at least one valid character string.
     */
    DuiApplication(int &argc, char **argv, const QString &appIdentifier = QString(), DuiApplicationService *service = 0);
    DuiApplication(int &argc, char **argv, DuiApplicationService *service);

    //! Cleans up any window system resources that were allocated by this application.
    virtual ~DuiApplication();

    static DuiApplication *instance();

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
    //! Show widgets sizes
    static bool showSize();
    //! Show widgets positions
    static bool showPosition();
    //! Show widgets margins
    static bool showMargins();
    //! Show object names
    static bool showObjectNames();
    //! Show cursor
    static bool showCursor();
    //! Emulate two finger gestures
    static bool emulateTwoFingerGestures();
    //! Sets if position of widgets should be shown or not
    static void setShowPosition(bool show);
    //! Sets if margins of widgets should be shown or not
    static void setShowMargins(bool show);
    //! Sets if names of objects should be shown or not
    static void setShowObjectNames(bool show);
    //! Sets if sizes of widgets should be shown or not
    static void setShowSize(bool show);
    //! Sets if bounding rects of widgets should be shown or not
    static void setShowBoundingRect(bool show);
    //! Sets if a frames-per-second counter should be shown or not
    static void setShowFps(bool show);
    //! Sets if the cursor should be shown or not
    static void setShowCursor(bool show);

    /*!
     * Returns the currently active application window.
     * \note If the active window is of type DuiWindow (and not DuiApplicationWindow)
     *       this method will return a null pointer.
     * \sa activeWindow()
     */
    static DuiApplicationWindow *activeApplicationWindow();
    /*! Returns the currently active window.
     * \note In comparison to QApplication::activeWindow(), this method will return
     *       a pointer to the DuiWindow even if it doesn't have focus. For single-window
     *       applications this method will return a pointer to the window once it's
     *       created (even before it's shown). For multiple-window applications this method
     *       will return a pointer to the current topmost window.
     */
    static DuiWindow *activeWindow();
    //! Returns a list of all windows in the application
    static QList<DuiWindow *> windows();
    //! Returns the application's app name
    static QString appName();
    //! Returns the application's binary name
    static QString binaryName();

    //! Returns object which provide interface for nonvisual feedback or NULL
    static DuiFeedbackPlayer *feedbackPlayer();

    //! Returns whether automatic loading of DuiInputContext is enabled
    static bool isLoadDuiInputContextEnabled();

    //! Sets whether DuiApplication should automatically load dui input context
    static void setLoadDuiInputContext(bool enable);

#ifdef __arm__
    static void fastFloatMath(bool val);
#endif

    /*!
     * \brief Select the prestart mode.
     * \param mode Prestart mode
     */
    static void setPrestartMode(Dui::PrestartMode mode);

    /*!
     * \brief Return the current prestart mode
     * \return prestart mode
     */
    static Dui::PrestartMode prestartMode();

    /*!
     *\brief Return true if application is in the prestarted state.
     *\return <code>true</code> if application is in the prestarted state.
     */
    static bool isPrestarted();

    /*!
     * \brief Called when DuiApplication returns to prestarted state (if supported by the mode)
     *  Re-imp this if desired
     */
    virtual void restorePrestart();

    /*!
     * \brief Called when DuiApplication is released from the prestarted state
     *  Re-imp this if desired
     */
    virtual void releasePrestart();

Q_SIGNALS:

    /*!
     * \brief The application is requested to release as much memory as possible
     * \deprecated Since 0.19.3.
     */
    void releaseMemoryRequested();

    /*!
     * \brief Signal emitted when the default system locale changes.
     */
    void localeSettingsChanged();

    /*!
     * Signal that is emitted when the application has been released from the
     * prestarted state. Next the application should show a window.
     */
    void prestartReleased();

    /*!
     * Signal that is emitted when the application has been restored
     * to the prestarted state.
     */
    void prestartRestored();

#ifdef Q_WS_X11
    /*!
     * Signal application about a changed X pixmap
     */
    void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);
#endif

protected:
    DuiApplication(DuiApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier = QString());
    DuiApplicationPrivate *const d_ptr;

#ifdef Q_WS_X11
    //! \reimp
    bool x11EventFilter(XEvent *event);
    //! \reimp_end
#endif

private:

    Q_DECLARE_PRIVATE(DuiApplication)
    Q_DISABLE_COPY(DuiApplication)

#ifdef UNIT_TEST
    friend class Ut_DuiApplication;
#endif
};

#endif
