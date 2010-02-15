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

#ifndef DUIAPPLETINVENTORY_H
#define DUIAPPLETINVENTORY_H

#include <QDir>
#include <QStringList>
#include "duiappletinventorymodel.h"

class QFileSystemWatcher;
class DuiWidget;
class DuiMashupCanvas;

//! \internal

/*!
 * DuiAppletInventory is a widget that maintains collection of available
 * applets and instantiates new instances of those applets.
 *
 * DuiAppletInventory will start to monitor an applet path after
 * setEnabled(true) is called. When active, all applets that are available in
 * the inventory are visualised as instantiable options in this inventory
 * widget.
 *
 * User can instantiate applets from an DuiAppletInventory by selecting them.
 * When an applet is instantiated from DuiAppletInventory the widget
 * of the applet is added to the DuiMashupCanvas container that is specified in
 * the call to the setMashupCanvas() method.
 *
 * Available applets are defined as .desktop metadata files in
 * /usr/share/applications/dui/applets. The files conform to the
 * freedesktop.org desktop file format.
 */

class DuiAppletInventory : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiAppletInventory)

public:
    /*!
     * Constructs an DuiAppletInventory widget.
     *
     * \param parent the parent widget of the DuiAppletInventory, defaults to NULL
     */
    DuiAppletInventory(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the DuiAppletInventory.
     */
    virtual ~DuiAppletInventory();

    /*!
     * Sets the mashup canvas to add instantiated applet widgets to.
     *
     * \param mashupCanvas The mashup canvas to add instantiated applet widgets to.
     */
    void setMashupCanvas(DuiMashupCanvas &mashupCanvas);

    /*!
     * Sets the applet categories available in this applet inventory.
     * The default value for the categories is an empty list. This
     * means filtering by categories is disabled and all applets are
     * shown in the applet inventory.
     *
     * \param categories a list of names of the applet categories to show in this applet inventory.
     */
    void setCategories(const QStringList &categories);

    /*!
     * Gets the applet categories available in this applet inventory.
     *
     * \return a list of names of the applet categories to show in this applet inventory.
     */
    QStringList categories() const;

    /*!
     * If enabled is true, the applet inventory is enabled (items can be clicked, button is visible); otherwise, it is disabled.
     *
     * The applet inventory is disabled by default.
     *
     * \param enabled if true, the applet inventory is enabled; otherwise, it is disabled
     */
    void setEnabled(bool enabled);

    /*!
     * Sets the names of the installation sources to show in the inventory
     * \param installationSources a list of the source names
     */
    void setInstallationSources(const QStringList &installationSources);

signals:
    /*!
     * \brief Called when a .desktop file that used to be present is removed
     */
    void appletUninstalled(const QString &desktopFile);

    /*!
     * \brief Emitted when the applet inventory should be hidden
     */
    void hideAppletInventory();

private slots:
    /*!
     * Called when the applet binary path has changed.
     *
     * \param path Path that has changed (should be the applet binary path)
     */
    void appletPathChanged(const QString &path);

    /*!
     * Called when the user clicks an applet button
     */
    void appletButtonClicked();

    /*!
      * Called to instantiate applets from a package
      *
      * \param packageName the name of the applet package to be instantiated
      */
    void instantiateAppletsFromPackage(const QString &packageName);

private:
    /*!
     * Initalizes the applet inventory if necessary.
     * Connects applet inventory to a DuiMashupCanvas widget set using
     * setMashupCanvas and starts monitoring the applet metadata files.
     */
    void initializeIfNecessary();

    /*!
     * Loads and instantiates an applet installation source from the given library
     * \param path the path to the library
     * \return the installation source widget
     */
    DuiWidget *loadAppletInstallationSource(const QString &path);

    /*!
     * Creates the applet installation source widgets and stores them into the model
     */
    void createInstallationSourceWidgets();

    /*!
     * Returns a list of paths to the available installation source binaries
     * \return a list of absolute file paths
     */
    QStringList installationSourceBinaryPaths() const;

    //! Mashup canvas for the applet instances
    DuiMashupCanvas *mashupCanvas;

    //! File system watcher to monitor changes in the applet binary directory
    QFileSystemWatcher *watcher;

    //! Plugin binary path
    QDir appletPath;

    //! A list of names of the applet categories to show in this applet inventory. An empty list will show all applets.
    QStringList appletCategories;

    //! A list of installation source names
    QStringList installationSources;

    //! Whether the applet inventory has been initialized or not
    bool initialized;

#ifdef UNIT_TEST
    friend class Ut_DuiAppletInventory;
#endif
};

//! \internal_end

#endif // DUIAPPLETINVENTORY_H
