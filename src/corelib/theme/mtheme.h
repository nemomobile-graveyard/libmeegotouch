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

#ifndef MTHEME_H
#define MTHEME_H

#include "mexport.h"
#include "mnamespace.h"

#include <QObject>
#include <QString>
#include <QSize>
#include <QRect>
#include <QColor>

class QPixmap;
class QGraphicsWidget;
class MWidgetView;
class MThemePrivate;
class MWidgetController;
class MStyle;
class MScalableImage;
class MStyleContainer;
class MLibrary;
class MPalette;
class MDefaultFonts;
class MComponentData;
class QAbstractAnimation;

/*!
 \class MTheme
 \brief MTheme abstracts the theming and styling of DirectUI applications and widgets

 MTheme is a singleton class providing runtime access to the MeeGo Touch theming parameters,
 as well as factory methods for instantiating various theming resources such as shared
 graphical assets in the form of QPixmaps and MScalableImages, as well as style and view
 instances for the widgets.

 MTheme communicates with a theme server whenever a graphical asset
 is requested. The theme server is responsible for loading the graphics (either from
 static image files, by rasterizing SVG or through procedural generation) and sharing them.
 The sharing of graphics assets is platform specific, MeeGo Touch ships with a reference theme
 server called \a mthemedaemon that shares assets using the X11 system. In case a theme server
 is not available, no sharing occurs and each graphical asset is duplicated for each application.
 */
class M_CORE_EXPORT MTheme : public QObject
{
    Q_OBJECT

public:

    /*!
    ViewTypes are standardized widget variant names, as defined by the widgets themselves. For example,
    MButton::checkboxType can be expected to be implemented by each theme, while a random non-standard
    type is unlikely to be. There is a mapping of view types to view implementation class names in the
    theme configurations.
    */
    typedef QString ViewType;

    //! Defines the type of theme service the process will use.
    enum ThemeService {
        AnyTheme = 0,   //!< Default theme service will be used.
        RemoteTheme,    //!< Remote theme service will be used.
        LocalTheme      //!< Local theme service will be used.
    };

    //! Defines the how resources that are registered with the theme at runtime are handled
    enum InsertMode {
        Overwrite, //!< The resource replaces any existing resource of the same type.
        Append     //!< The resource is appended to the theme data, leaving any existing resources active. The existing resources may be prioritized when making requests.
    };

    /*!
     Constructs the MTheme instance.

     By default the theme system firsrt attempts to connect to a remote theme service,
     if this fails a local theme service is created unless overriden by the \a themeService parameter.
     Note that on some platforms failing to connect to a remote theme service at startup may be a fatal error.
     If the \a themeService is RemoteTheme, the constructor will not return until a remote theme service becomes
     available. This option should be used when the application may potentially start before the theme service
     during system bootup. The RemoteTheme option can also be toggled by the -remote-theme command line parameter
     as passed to MApplication. The RemoteTheme option cannot be used if imglistFilename is also set.


     \a applicationName is used as the first part of the search path for resources, ensuring that application
     specific theme resources are prioritized.
     \a imglistFilename is not in use anymore.
     */
    MTheme(const QString &applicationName, const QString &imglistFilename = QString(), ThemeService themeService = AnyTheme);

    /*!
     * Destroys the theme instance.
     */
    virtual ~MTheme();

    /*!
     Returns the style object matching the given parameters.

     A style object is a collection of data properties, from which widget views (or any other
     type of class) access their run time parameters. The data is in the form of standard Qt properties
     and the style object is automatically filled through Qt property introspection based
     on values defined in CSS sylesheets.

     Ownership of the returned object remains with MTheme, the caller must
     not delete it as it may be shared by multiple users. Once the caller no longer needs the
     object, it should be passed to the releaseStyle method of MTheme.

     The parameters are:

     - \a styleClassName, the name of the class (e.g. "MLabelStyle")
     - \a objectName, the name of the object (e.g. "MyButton")
     - \a mode, the name of the mode of the widget (e.g. "active", "selected", "disabled")
     - \a type, the type of view that is used (e.g. for the button: "default", "checkbox", "toggle")
     - \a orientation, the orientation that the style should be optimized for (e.g portrait or landscape)
     - \a parent (optional), the controller of the widget for which the style is used

     \sa releaseStyle
     */
    static const MStyle *style(const char *styleClassName,
                               const QString &objectName,
                               const QString &mode,
                               const QString &type,
                               M::Orientation orientation,
                               const MWidgetController *parent = NULL);

    /*!
      This is an overloaded function.

      Returns the style object matching the given parameters. Instead of taking the controller of
      the widget to get the contextual information for the style, this function gets a list of
      QStringLists, describing the metaclass hierarchy of each parent, starting from the most
      direct.

      This overload is not for using in regular MeeGo Touch applications. It is intended to be
      used by other libraries that need the styling information but do not have widget controllers
      or views instantiated.
    */
    static const MStyle *style(const char *styleClassName,
                               const QString &objectName,
                               const QString &mode,
                               const QString &type,
                               M::Orientation orientation,
                               const QList<QStringList> &parentClassHierarchies,
                               const QString &parentStyleName);

    /*!
      This is an overloaded function.

      Returns the style object with the given \a styleClassName and \a objectName
    */
    static const MStyle *style(const char *styleClassName,
                               const QString &objectName = "");

    /*!
      This method should be called on style objects when they are no longer needed.

      releaseStyle performs cleanup and frees the style if there are no other clients using it.
    */
    static void releaseStyle(const MStyle *style);

    /*!
     Adds a directory to the theme service search path for this application.

     The theme service will look for PNG and SVG files in the location specified by
     \a directoryName, which can be either an relative or absolute path. If \a mode
     is defined as Recursive, subdirectories of the specified directory are searched as well.
     The default is not to search subdirectories.

     Note: A custom pixmap directory path must be added prior requesting a pixmap from it,
        otherwise an invalid pixmap will be displayed.

     \sa pixmap

     */
    static bool addPixmapDirectory(const QString &directoryName, M::RecursionMode mode = M::NonRecursive);

    /*!
     Clear all pixmap directories from the theme service search path for this application.
     */
    static void clearPixmapDirectories();

    /*!
     Returns the singleton MTheme instance.
     */
    static MTheme *instance();

    /*!
     Returns a QPixmap that contains the graphical asset specified by the \ref logicalid "logical ID".

     Note: While loading the graphics the application UI thread is blocked. In case the graphics is large or
     you do not need to immediately know the final size of the requested graphics, consider using the
     asyncPixmap() method instead to keep the UI responsive.

     Prior requesting a pixmap from a custom path, the path must be added through addPixmapDirectory() method.

     The \a id is a logical identifier for a single graphical theme asset.
     The source format of the graphics is abstracted and determined by the theme service. The default
     graphics loading policy is as follows:

     - Static images such as PNG have a higher priority than assets which are dynamic or require
     rasterization (SVG), thus allowing for static caching of assets. For static images the
     id is the filename without the filetype suffix.
     - Application supplied assets have higher priority than base theme assets, while assets in
     a theme for a specific application has higher priority than assets supplied by the application itself.

     The \a size parameter determines the size of the returned pixmap. The default \a size of QSize(0,0) marks
     that the original source graphics size should be used.

     The returned pixmap is owned by the theme and should be freed with releasePixmap() when no longer needed.

     \sa releasePixmap
     \sa asyncPixmap
     \sa addPixmapDirectory
     */
    static const QPixmap *pixmap(const QString &id, const QSize &size = QSize(0, 0));

    /*!
     Returns a QPixmap that contains the graphical asset specified by the \ref logicalid "logical ID".

     Loads the pixmap specified by \a id of size \a size.

     Note: Prior requesting a pixmap from a custom path, the path must be added through addPixmapDirectory() method.

     The requested pixmap is loaded asynchronously. The returned pixmap can be one of the following:
     - The real pixmap if the pixmap was already loaded by the system.
     - A transparent (or green, if GConf key /meegotouch/debug/show_async_requests is set to true),
       \a size sized (QSize(0,0) by default) pixmap, serving as a placeholder while the pixmap data is loading.
     - A red pixmap of 50x50 size, indicating that the pixmap with the requested id was not found in the theme.

     In the first two cases, the pixmap data can be changed at any time by the theme. This may happen when
     the theme changes, as pixmaps are updated, pixmaps finish loading or for any other reason.

     The returned pixmap is owned by the theme and should be freed with releasePixmap() when no longer needed.

     \sa pixmap
     \sa releasePixmap
     \sa addPixmapDirectory
     */
    static const QPixmap *asyncPixmap(const QString &id, const QSize &size = QSize(0, 0));

    /*!
     Returns a copy of the graphical asset with the given \ref logicalid "logical ID".

     Loads the pixmap specified by \a id of size \a size synchronously and returns a copy to the caller.
     This method can be very slow, use the pixmap() or asyncPixmap() methods instead if possible.

     Note: Prior requesting a pixmap from a custom path, the path must be added through addPixmapDirectory() method.

     The ownership of the returned pixmap is transferred to caller, and the pixmap may be modified.

     \sa pixmap
     \sa asyncPixmap
     \sa addPixmapDirectory
     */
    static QPixmap *pixmapCopy(const QString &id, const QSize &size = QSize(0, 0));

    /*!
     Returns a MScalableImage that contains the graphical asset specified by the \ref logicalid "logical ID".

     The logic for acquiring a scalable image is the same as that for a regular pixmap.
     The \a left, \a right, \a top and \a bottom parameters specify the non-scaled border values of the
     image.

     The returned image is owned by the theme and should be freed with releaseScalableImage() when no longer needed.

     \sa pixmap
     \sa releaseScalableImage
     */
    static const MScalableImage *scalableImage(const QString &id, int left, int right, int top, int bottom);

    /*!
     Releases a shared MScalableImage acquired through MTheme.

     releaseScalableImage performs cleanup and frees the image if there are no other clients using it.

     \sa scalableImage
     */
    static void releaseScalableImage(const MScalableImage *image);

    /*!
     Releases a shared QPixmap acquired through MTheme.

     releasePixmap performs cleanup and frees the pixmap if there are no other clients using it.

     \sa pixmap
     */
    static void releasePixmap(const QPixmap *pixmap);

    /*!
     Returns an animation instance with the given type.

    \param animationTypeName Name of the animation type.

     The returned animation is theme specific.
     */
    static QAbstractAnimation *animation(const QString &animationTypeName);

    /*!
     Returns an effect instance with the given type.

    \param effectTypeName Name of the effect type.

     The returned effect is theme specific.
     */
    static QGraphicsEffect *effect(const QString &effectTypeName);

    /*!
     Returns a view instance for the given \a controller.

     The returned view is theme specific and can be changed in the theme configuration.
     */
    static MWidgetView *view(const MWidgetController *controller);

    /*!
     Returns the theme's logical color palette.

     The palette can be used to programmatically query the common colors defined in the theme.
     */
    static const MPalette &palette();

    /*!
     Returns the theme's logical font information.

     The font information can be used to programmatically query the common fonts defined in the theme.
     */
    static const MDefaultFonts &fonts();

    /*!
     Loads the requested CSS file to the current active theme.

     Either appends to or replaces the existing theme CSS using the style
     sheet located by \a filename, depending on the \a mode. The method
     returns true if the CSS file was successfully loaded.

     Note that CSS files should be loaded before constructing the style objects.
     */
    static bool loadCSS(const QString &filename, InsertMode mode = Append);

    /*!
     Returns the name of the current theme in use or an empty string no theme is selected.
     */
    static QString currentTheme();

    /*!
     Returns true if there are pending asynchronous pixmap requests in the system.
     */
    static bool hasPendingRequests();

    static void cleanupGarbage();

Q_SIGNALS:
    /*!
     This signal is emitted when all pixmap requests have finished.

     The signal is emitted regardless of if the request was successfull or not.

     Note that for regular applications, listening for this signal is
     not necessary, since any change automatically triggers an update of
     the interface.

     Widget implementations must not connect to this signal. All widgets
     are repainted after the outstanding requests have finished.

     \sa asyncPixmap
     */
    void pixmapRequestsFinished();

    /*!
     This signal is emitted when the current theme was changed but before the old
     resources are deleted. This allows for applications to stop using the old
     pixmaps before they become invalid.

     Note that for regular application, listening to this signal is not necessary.
     This is mostly useful for custom applications that only use selected parts of
     MeegoTouch Framework.
     */
    void themeIsChanging();

    /*!
     This signal is emitted when the theme service and all applications have changed the theme.

     Note that for regular applications, listening for this signal is not necessary,
     since when this signal is emitted the applications are already running with the new
     theme.
     */
    void themeChangeCompleted();

protected:

    /*!
     Causes all widgets to update their views when the theme is changed.
     */
    void rebuildViewsForWidgets();

    //! \internal
    MThemePrivate *const d_ptr;
    //! \internal_end

private:
    Q_DISABLE_COPY(MTheme)
    Q_DECLARE_PRIVATE(MTheme)

#ifndef UNIT_TEST
    Q_PRIVATE_SLOT(d_func(), void themeChangedSlot(QStringList,QStringList))
    Q_PRIVATE_SLOT(d_func(), void pixmapCreatedOrChangedSlot(QString,QSize,MPixmapHandle))
    Q_PRIVATE_SLOT(d_func(), void localeChangedSlot())
    Q_PRIVATE_SLOT(d_func(), void changeTheme())
#endif

    friend class MApplicationPrivate;
    friend class MAssemblyPrivate;
    friend class MComponentCachePrivate;
    friend class MComponentData;
    friend class MWidgetController;
    friend class MStyle;

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_MTheme;
    friend class Ft_Theme;
#endif
};

#endif
