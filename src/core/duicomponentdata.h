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

#ifndef DUICOMPONENTDATA_H
#define DUICOMPONENTDATA_H

#include <QObject>

#include "duiexport.h"
#include "duinamespace.h"

class DuiComponentDataPrivate;
class DuiWindow;
class DuiApplicationWindow;
class DuiApplicationService;
class DuiFeedbackPlayer;

/*!
 * \class DuiComponentData
 * \brief DuiComponentData manages the GUI application's control flow and main settings.
 *
 * DuiComponentData provides all the settings that are necessary for a QApplication in
 * order to initialize the DUI framework. As such it is used by DuiApplication.
 * which adds the event loop. DuiComponentData can be used by plain Qt applications
 * in order to provide access to resources of the DUI framework.
 *
 * DuiComponentData instance automatically extracts application name from the arguments
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
 * DuiComponentData will also create a QDBus service with name made by prepending 'com.nokia.' to
 * the application name provided in the constructor (or the binary name if no name is provided
 * in the constructor). It will provide an interface called DuiComponentDataIf which calls service
 * methods in an instance of DuiApplicationService. By default, DuiComponentData will construct an
 * instance of DuiApplicationService, but the application programmer can derive a class from
 * DuiApplicationService and provide a pointer to that in the constructor for DuiComponentData to
 * use instead. This way, the application programmer can override the methods in
 * DuiComponentDataSerivce and change the behaviour of the application's interface.
 *
 * For some applications, it is necessary to implement a custom DuiApplicationService. See
 * DuiApplicationService for more information.
 *
 */

class DUI_EXPORT DuiComponentData : public QObject
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
    DuiComponentData(int &argc, char **argv, const QString &appIdentifier = QString(), DuiApplicationService *service = 0);
    explicit DuiComponentData(DuiApplicationService *service);

    //! Cleans up any window system resources that were allocated by this application.
    virtual ~DuiComponentData();

    //! returns the DuiComponentData instance
    static DuiComponentData *instance();

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
    //! Return true if prestarted state active
    static bool prestarted();
    //! Returns the prestart mode
    static Dui::PrestartMode prestartMode();
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
    //! Sets the prestarted flag
    static void setPrestarted(bool flag);
    //! Sets the prestart mode
    static void setPrestartMode(Dui::PrestartMode mode);
    //! Set if two finger gestures should be emulated or not.
    static void setEmulateTwoFingerGestures(bool flag);

    /*!
     * Returns the currently active application window.
     * \note If the active window is of type DuiWindow (and not DuiApplicationWindow)
     *       this method will return a null pointer.
     * \sa activeWindow()
     */
    static DuiApplicationWindow *activeApplicationWindow();
    //! Returns the currently active window
    static DuiWindow *activeWindow();
    //! Returns a list of all windows in the application
    static QList<DuiWindow *> windows();

    //! Returns the application's app name
    static QString appName();
    //! Returns the application's binary name
    static QString binaryName();
    //! Returns the application's service name
    static QString serviceName();

    //! Returns object which provide interface for nonvisual feedback or NULL
    static DuiFeedbackPlayer *feedbackPlayer();

    //! Returns whether automatic loading of DuiInputContext is enabled
    static bool isLoadDuiInputContextEnabled();

    //! Sets whether DuiComponentData should automatically load dui input context
    static void setLoadDuiInputContext(bool enable);

Q_SIGNALS:
    /*!
     * \brief Signal emitted when the default system locale changes.
     */
    void localeSettingsChanged();

protected:
    DuiComponentData(DuiComponentDataPrivate &dd, int &argc, char **argv, const QString &appIdentifier = QString());
    DuiComponentDataPrivate *const d_ptr;

private:
    static DuiComponentData *self;
    static void registerWindow(DuiWindow *);
    static void unregisterWindow(DuiWindow *);
    static void setActiveWindow(DuiWindow *);

    Q_DECLARE_PRIVATE(DuiComponentData)
    Q_DISABLE_COPY(DuiComponentData)

    friend class DuiWindow;
    friend class DuiApplicationWindow;
    friend class DuiSceneWindow;
    friend class DuiTheme;
    friend class DuiDeviceProfile;
};

#endif
