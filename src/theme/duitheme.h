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

#ifndef DUITHEME_H
#define DUITHEME_H

#include "duiexport.h"
#include "duinamespace.h"

#include <QObject>
#include <QString>
#include <QSize>
#include <QRect>
#include <QColor>

class QPixmap;
class QGraphicsWidget;
class DuiWidgetView;
class DuiThemePrivate;
class DuiWidgetController;
class DuiStyle;
class DuiScalableImage;
class DuiStyleContainer;
class DuiLibrary;
class DuiPalette;
class DuiDefaultFonts;

/*!
 * Singleton theme class for Direct UI
 *
 * Theme maintains all the graphics for the whole UI. It is also responsible
 * of managing the style objects for all the objects that need styling.
 */
class DUI_EXPORT DuiTheme : public QObject
{
    Q_OBJECT

public:

    friend class DuiWidgetController;
    friend class DuiStyle;

    typedef QString ViewType;

    /*!
     * Defines what kind of theme daemon should be used.
     */
    enum ThemeService {
        AnyTheme = 0,   //!< Default theme daemon will be used.
        RemoteTheme,    //!< Remote theme daemon will be used.
        LocalTheme      //!< Local theme daemon will be used.
    };

    enum InsertMode {
        Overwrite,
        Append
    };

    /*!
     * Constructor for theme class.
     * Tries to connect to theme daemon, if couldn't connect then creates a local theme daemon.
     * \param themeIdentifier an identifier for the theme. The identifier is
     * used as the first part of the search path for resources.
     * \param imglistFilename file name for image list file. If set, DuiTheme will create local
     * theme daemon which will output list of used images to the file @p imglistFilename.
     * \param themeService what kind of theme daemon should be used. If RemoteTheme is supplied
     * as themeService, the constructor won't return until remote theme daemon is available.
     * RemoteTheme parameter cannot be used if imglistFilename is set.
     */
    DuiTheme(const QString &applicationName, const QString &imglistFilename = QString(), ThemeService themeService = AnyTheme);

    /*!
     * Destructor for theme class
     */
    virtual ~DuiTheme();

    /*!
     * Returns the style object with the given parameters.
     * This method does not transfer ownership.
     * \param styleClassName the name of the class (e.g. "DuiLabelStyle" )
     * \param objectName the name of the object (e.g. "MyButton" )
     * \param mode the name of the mode of the widget
     * (e.g. "active", "selected", "disabled" )
     * \param type the type of view that is used
     * (e.g. for the button: "default", "checkbox", "toggle" )
     * \param orientation the orientation that the style should be optimized for:
     * portrait or landscape
     * \param parent the controller of the widget for which the style is used
     */
    static const DuiStyle *style(const char *styleClassName,
                                 const QString &objectName,
                                 const QString &mode,
                                 const QString &type,
                                 Dui::Orientation orientation,
                                 const DuiWidgetController *parent = NULL);

    /*!
      This is an overloaded function.
      Returns the style object with the given \a styleClassName and \a objectName
    */
    static const DuiStyle *style(const char *styleClassName,
                                 const QString &objectName = "");

    /*!
      \brief Called when style is no longer needed, performs clean up.
    */
    static void releaseStyle(const DuiStyle *style);

    /*!
     * Adds directory for the daemon to search .png and .svg -files for this application.
     * \param directoryName relative or absolute path to the directory.
     * \param recursive should the directory be added recursively.
     * \return boolean, true if operation successful.
     */
    static bool addPixmapDirectory(const QString &directoryName, Dui::RecursionMode mode = Dui::NonRecursive);

    /*!
     * Clear all pixmap directories from this application.
     */
    static void clearPixmapDirectories();

    /*!
     * Returns singleton DuiTheme object
     * \return DuiTheme object
     */
    static DuiTheme *instance();

    /*!
     * Returns QPixmap from element id
     * Loads the requested pixmap asynchronously. Pixmap returned can be one of following:
     * 1) 50x50 red pixmap which marks that pixmap with requested id was not found.
     * 2) Transparent gray 1x1 or \a size sized pixmap which marks that the pixmap is not yet loaded.
     * 3) The real pixmap if the pixmap was already loaded by the system.
     * in cases 2) and 3) the pixmap data can be changed at any time by the theme (when theme changes,
     * images update, images get loaded or any other reason)
     * \param id element id of the requested image
     * \param size Size of the requested image. QSize(0,0) marks
     * that the original size of the pixmap is used.
     * \note Returned pixmap should be released using releasePixmap(const QPixmap*) method.
     * \return QPixmap, which matches the given parameters but does not necessary contain the real data yet.
     */
    static const QPixmap *pixmap(const QString &id, const QSize &size = QSize(0, 0));

    /*!
     * Returns a QPixmap copy from element id. This pixmap can be modified.
     * *The ownership of the pixmap is transferred to caller*.
     * Loads the pixmap synchorously and makes a copy and returns it to caller.
     * This method can be very slow, so use asynchronous \see pixmap method instead.
     * \param id element id of the requested image
     * \param size Size of the requested image. QSize(0,0) marks
     * that the original size of the pixmap is used.
     * \return QPixmap, which matches the given parameters but does not necessary contain the real data yet.
     */
    static QPixmap *pixmapCopy(const QString &id, const QSize &size = QSize(0, 0));

    static const DuiScalableImage *scalableImage(const QString &id, int left, int right, int top, int bottom);
    static void releaseScalableImage(const DuiScalableImage *image);

    /*!
     * Releases pixmap
     * \param pixmap QPixmap being released by the application
     */
    static void releasePixmap(const QPixmap *pixmap);

    /*!
     * Returns a view for the given object.
     * \param controller controller needing the view
     * \return DuiWidgetView A theme-specific view for the object
     * TODO: this should be protected or private method, and only DuiWidgetController should be able to access this.
     */
    static DuiWidgetView *view(const DuiWidgetController *controller);

    /*!
     * \brief Returns system-wide logical colors.
     */
    static const DuiPalette &palette();

    /*!
     * \brief Returns system-wide logical fonts.
     */
    static const DuiDefaultFonts &fonts();

    /*!
     * Loads the requested CSS file to the current theme.
     * Note that CSS files should be loaded before constructing
     * the style objects.
     * \param filename Filename of the CSS file
     * \param mode Defines how the loaded stylesheet is inserted into theming system.
     * \return boolean, true if the CSS file was loaded, otherwise false
     */
    static bool loadCSS(const QString &filename, InsertMode mode = Append);

    /*!
     * Returns the current theme in use
     */
    static QString currentTheme();

    /*!
     * Finds all available themes installed in the system.
     */
    static QStringList findAvailableThemes();

    /*!
     * Sends a signal to theme daemon to change the theme.
     * \param theme_id ID of theme to be used
     * TODO: check if we want to allow any application to change the theme.
     */
    void changeTheme(const QString &theme_id);

    /*!
     * Checks whether there are pending pixmap requests.
     */
    static bool hasPendingRequests();
protected:

    /*!
     * Causes all widgets to update their views when theme is changed
     */
    void rebuildViewsForWidgets();

protected:
    DuiThemePrivate *const d_ptr;
private:
    Q_DISABLE_COPY(DuiTheme)
    Q_DECLARE_PRIVATE(DuiTheme)

#ifndef UNIT_TEST
    Q_PRIVATE_SLOT(d_func(), void themeChangedSlot(const QStringList &))
    Q_PRIVATE_SLOT(d_func(), void pixmapChangedSlot(const QString &, const QSize &, Qt::HANDLE))
    Q_PRIVATE_SLOT(d_func(), void pixmapCreatedSlot(const QString &, const QSize &, Qt::HANDLE))
    Q_PRIVATE_SLOT(d_func(), void localeChangedSlot())
#endif

    friend class DuiApplicationPrivate;

#ifdef QT_TESTLIB_LIB
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_DuiTheme;
    friend class Ft_Theme;
#endif
};


#endif
