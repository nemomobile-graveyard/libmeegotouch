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

#ifndef MAPPLICATION_H
#define MAPPLICATION_H

#include <QApplication>
#ifdef Q_WS_X11
#include <QX11Info>
#endif

#include "mexport.h"
#include "mnamespace.h"

class MApplicationPrivate;
class MWindow;
class MApplicationWindow;
class MApplicationService;
class MFeedbackPlayer;
class MSceneWindow;


/*!
 * \class MApplication
 * \brief MApplication manages the GUI application's control flow and main settings.
 *
 * The MApplication instance automatically extracts the application name from the arguments
 * given in the constructor. It also loads the CSS stylesheet and the SVG file and adds application
 * specific image paths to the pixmap search paths used by the current MTheme instance.
 *
 * CSS and SVG files are searched for in order from the following places:
 * <ol>
 *   <li>The directory in which the application was launched.</li>
 *   <li>Application specific theme path (global theme path appended with the application name)</li>
 *   <li>Global theme path (Qt data path appended by themes/m)</li>
 * </ol>
 * When the first instance of the CSS or SVG file is found, it is loaded to the current MTheme.
 * Also the image subdirectory of each of the aforementioned paths is appended to the pixmap search
 * paths used by MTheme.
 *
 * MApplication will also create a QDBus service with a name made by prepending 'com.nokia.' to
 * the application name provided in the constructor (or the binary name if no name is provided
 * in the constructor). It will provide an interface called MApplicationIf which calls service
 * methods in an instance of MApplicationService. By default, MApplication will construct an
 * instance of MApplicationService, but the application programmer can derive a class from
 * MApplicationService and provide a pointer to that in the constructor for MApplication to
 * use instead. This way, the application programmer can override the methods in
 * MApplicationService and change the behaviour of the application's interface.
 *
 * The default behaviour is to only allow a single instance of any
 * application. When an application is launched, it attempts to register
 * itself as a DBUS service (as above). Only the first instance of the
 * application will be successful, and subsequent attempts will fail. When
 * DBUS registration fails, the default behaviour is to call the first
 * instance's MApplicationService launch() method, and then quit. This
 * causes the first instance to become visible (raises the window).
 *
 * If other behaviour is required, for example if you want multiple
 * instances of an application, then it is necessary to derive a class from
 * MApplicationService and override its methods.
 *
 * Notice that it is not supported to create, delete and again create an MApplication instance
 * in the same program.
 */

class M_CORE_EXPORT MApplication : public QApplication
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
     * lifetime of the MApplication object. In addition, argc must be greater than zero and
     * argv must contain at least one valid character string.
     */
    MApplication(int &argc, char **argv, const QString &appIdentifier = QString(), MApplicationService *service = 0);
    MApplication(int &argc, char **argv, MApplicationService *service);

    //! Cleans up any window system resources that were allocated by this application.
    virtual ~MApplication();

    //! \reimp
    // Reimplement QApplication::notify so we catch eventual
    // exceptions thrown from within event handlers
    virtual bool notify(QObject *receiver, QEvent *event);
    //! \reimp_end

    /*! \brief Returns a pointer to the application's MApplication instance.
     * If no instance has been allocated, null is returned.
     * \sa QCoreApplication::instance()
     */
    static MApplication *instance();

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
    //! Enables or disables logging of frames
    static void setLogFps(bool show);
    //! Sets if the cursor should be shown or not
    static void setShowCursor(bool show);

    /*!
     * Returns the currently active application window.
     * \note If the active window is of type MWindow (and not MApplicationWindow)
     *       this method will return a null pointer.
     * \sa activeWindow()
     */
    static MApplicationWindow *activeApplicationWindow();
    /*! Returns the currently active window.
     * \note In comparison to QApplication::activeWindow(), this method will return
     *       a pointer to the MWindow even if it doesn't have focus. For single-window
     *       applications this method will return a pointer to the window once it's
     *       created (even before it's shown). For multiple-window applications this method
     *       will return a pointer to the current topmost window.
     */
    static MWindow *activeWindow();
    //! Returns a list of all windows in the application
    static QList<MWindow *> windows();
    //! Returns the application's app name
    static QString appName();
    //! Returns the application's binary name
    static QString binaryName();

    //! Returns object which provide interface for nonvisual feedback or NULL
    //
    // \deprecated Please use class MFeedback to play input feedbacks. Since 0.20.43
    static MFeedbackPlayer *feedbackPlayer();

    //! Returns whether automatic loading of MInputContext is enabled
    static bool isLoadMInputContextEnabled();

    //! Sets whether MApplication should automatically load m input context
    static void setLoadMInputContext(bool enable);

#ifdef __arm__
    static void fastFloatMath(bool val);
#endif

    /*!
     * \brief Select the prestart mode.
     * \param mode Prestart mode
     */
    static void setPrestartMode(M::PrestartMode mode);

    /*!
     * \brief Return the current prestart mode
     * \return prestart mode
     */
    static M::PrestartMode prestartMode();

    /*!
     *\brief Return true if application is in the prestarted state.
     *\return <code>true</code> if application is in the prestarted state.
     */
    static bool isPrestarted();

    /*!
     * \brief Called when MApplication returns to prestarted state (if supported by the mode).
     *  Hides all windows by default.
     *  Re-imp this if desired.
     */
    virtual void restorePrestart();

    /*!
     * \brief Called when MApplication is released from the prestarted state.
     *  Effectively this method just shows MApplication::activeWindow().
     *  Does nothing by default if multi-windowed prestart is selected.
     *  Re-imp this if desired.
     */
    virtual void releasePrestart();

    /*!
     * \brief Manually release/restore the application from/to the prestarted state.
     *  Multi-window applications may need this if they want to show some
     *  specific window using some other D-Bus service than MApplicationService.
     *  This triggers corresponding signals and handlers if the state changes.
     * \param enable If set to true, prestart is restored if lazy shutdown selected.
     *        If set to false, prestart is released (equals to MApplicationService::lauch()).
     */
    static void setPrestarted(bool enable);

Q_SIGNALS:

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
     * Signal that is emitted when the application is being minimized to switcher
     * and the minimize animation started.
     */
    void minimizing();

    /*!
     * Signal that is emitted when the application is being minimized to switcher
     * and the minimize animation finished.
     */
    void minimized();

    /*!
     * Signal that is emitted when the application is being minimized to switcher
     * and the minimize animation was canceled (the window wasn't minimized).
     *
     * \note This signal is useful only when the window manager provides minimize animation
     *       that's capable of being canceled.
     */
    void minimizingCanceled();

    /*!
     * Signal application about a changed X pixmap
     */
    void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);
#endif

protected:
    MApplication(MApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier = QString());
    MApplicationPrivate *const d_ptr;

#ifdef Q_WS_X11
    //! \reimp
    bool x11EventFilter(XEvent *event);
    //! \reimp_end
#endif

private:

    Q_DECLARE_PRIVATE(MApplication)
    Q_DISABLE_COPY(MApplication)

#ifdef UNIT_TEST
    friend class Ut_MApplication;
#endif
};

#endif
