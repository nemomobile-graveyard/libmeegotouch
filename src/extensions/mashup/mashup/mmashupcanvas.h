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

#ifndef MMASHUPCANVAS_H
#define MMASHUPCANVAS_H

#include "mextensionarea.h"
#include "mmashupcanvasmodel.h"

class MDataStore;
class MAppletInstanceManager;
class MMashupCanvasPrivate;

//! \internal
/*!
 * MMashupCanvas is a widget which can be populated with applet instances.
 * A MMashupCanvas can be placed on any view that wants to leverage applet support.
 *
 * \section overview Overview
 *
 * Applets are small programs that are embedded into mashup canvases. The mashup canvas composites applet images as part of its own visual presentation.
 * Applets are primarily run in separate processes in order to safeguard against possible bugs in applets such as blocking the GUI or crashing.
 * The mashup canvas contains an applet inventory (\c MAppletInventory), which is a special type of menu which populates its content from .desktop files that applets have installed during their package installation.
 * It automatically registers applets (through \c MAppletInstanceManager) and implements state handling, memory management etc.
 *
 * \section metadata Applet metadata
 *
 * Each applet package installs a .desktop file into \c /usr/share/m/applets/ in which the applet specifies its metadata. \c MAppletInventory monitors this directory and enables instantiation of installed applets onto mashup canvas. The applet inventory is opened by tapping a button on bottom of the mashup canvas area. The inventory shows the icon of each applet in a grid view. When icon is tapped the applet is instantiated to the mashup canvas. When applet is instantiated a new !QGraphicsWidget - object from applet binary is constructed and added to the mashup canvas. The applet inventory can be closed/hidden by tapping 'x' on the top right corner.
 *
 * Applet metadata is defined in .desktop files following freedesktop.org <a href="http://www.freedesktop.org/wiki/Specifications/desktop-entry-spec?action=show&redirect=Standards%2Fdesktop-entry-spec">desktop entry specification</a>. Applet metadata extends .desktop entry specification by defining a new type \c MApplet. Applet metadata specification follows desktop entry specification so that required keys (Type, Name and Exec) have to be defined in the applet metadata. \c Exec key in applet metadata defines the runner binary which is launched in separate process to run the applet binary. This key needs to be defined but can be left empty. If \c Exec key is left empty the applet is ran in the same process with the mashup canvas.
 *
 * Applet metadata extends the desktop specification with following new keys. These keys need to be defined under group \c M.
 *
 * <table>
 * <tr><th>Key</th><th>Description</th><th>Value type</th><th>REQ</th></tr>
 * <tr><td>\c X-MApplet-Applet</td><td>Defines the applet binary that will be loaded using the runner defined by the \c Exec key. This binary needs to be located in \c /usr/lib/m/applets/ directory.</td><td>string</td><td>YES</td></tr>
 * <tr><td>\c X-MApplet-Identifier</td><td>Defines an identifier for the applet. The identifier is used for example defining the applet specific style resource locations.</td><td>string (can contain characters [a-zA-Z0-9_-])</td><td>NO</td></tr>
 * </table>
 *
 * Following is an example of applet metadata specification:
 * \code
 * [Desktop Entry]
 * Type=MApplet
 * Name=ExampleApplet
 * Icon=icon-l-music
 * Exec=mappletrunner
 *
 * [M]
 * X-MApplet-Applet=libexampleapplet.so
 * \endcode
 *
 * \section lifecycle Applet lifecycle
 *
 * Below is a list of events that define the applet lifecycle. These events can occur on an applet type (e.g. weather applet) or on an applet instance (e.g. weather applet instance that shows weather in London).
 *
 * - Install - A new applet type is installed. Package manager is invoked to install a new applet type from a debian package.
 * - Add - A new applet instance is placed on the mashup canvas. Mashup canvas initiates an instance construction from applet binary. The same operation occurs when already added applet instance is restored after mashup canvas has been closed and reopened (e.g. when application has been turned off and back on). When applet instance is added for the first time to the canvas the applet instance specific data is empty. When applet instance is restored the applet instance specific data is restored from pre-closed state.
 * - Close - An applet instance is closed. This might occur for instance when application with mashup canvas is shut down (e.g. when device is turned off). It's good to make a distinction between this and \b remove.
 * - Update - An applet type is updated. This is the case where an already installed applet type (e.g. weather applet) is updated to a newer version. Applet developers should assure that instance specific applet data of an old applet type is compatible with the new applet version (new data can naturally be introduced when applet is updated to a newer version).
 * - Remove - This occurs when an applet instance is removed from applet canvas. All applet instance specific data is removed. Also applet developer should remove any data it has stored for this particular applet instance. This means that applet instance is not added to the mashup canvas again when application is restarted the next time.
 * - Uninstall - An already installed applet type is uninstalled. This will implicitly \b remove all applet instances of the specified type. Package manager will handle the exact package removal process.
 *
 * \section notes Notes
 *
 * Every MMashupCanvas has to be identified by a unique identifier that is passed in through
 * the constructor. The user of this class should ensure that they use unique identifiers for the
 * mashup canvases they construct. If the user of this class fails to provide unique identifiers,
 * unpredictable side effects may occur because the implementation ensures that every mashup canvas has a
 * unique identifier. The unexpected side effects may include, but isn't limited to:
 * - mixing of the contents of the canvases
 * - missing applets from the canvases upon restart of the application
 *
 * \see \ref appletdevelopment
 */
class M_EXTENSIONS_EXPORT MMashupCanvas : public MExtensionArea
{
    Q_OBJECT
    M_CONTROLLER(MMashupCanvas)

public:
    /*!
     * Constructor
     * \param identifier A unique identifier of this mashup canvas. The data of the instantiated
     * applets is stored and restored based on this identifier.
      \param parent Optional Object's parent
     */
    explicit MMashupCanvas(const QString &identifier, QGraphicsItem *parent = NULL);

    /*!
     * Default destructor
     */
    virtual ~MMashupCanvas();

    /*!
     * Gets the applet instance manager for managing applet instances.
     *
     * \return an instance of MAppletInstanceManager
     */
    MAppletInstanceManager *appletInstanceManager() const;

    /*!
     * Sets the applet categories available in this mashup canvas.
     * The default value for the categories is an empty list. This
     * means filtering by categories is disabled and all applets are
     * shown in the applet inventory.
     *
     * \param categories a list of names of the applet categories to show in this mashup canvas.
     */
    void setCategories(const QStringList &categories);

    /*!
     * Gets the applet categories available in this mashup canvas.
     *
     * \return a list of names of the applet categories to show in this mashup canvas.
     */
    QStringList categories() const;

    /*!
     * Returns the unique identifier of this canvas
     * \return the identifier
     */
    QString identifier() const;

    /*!
     * Returns the D-Bus service address of this canvas
     * \return the D-Bus address
     */
    QString serviceAddress() const;

protected:
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     */
    MMashupCanvas(MMashupCanvasPrivate *dd, MMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier);

private:
    Q_DECLARE_PRIVATE(MMashupCanvas)
    Q_DISABLE_COPY(MMashupCanvas)
};
//! \internal_end


#endif // MMASHUPCANVAS_H
