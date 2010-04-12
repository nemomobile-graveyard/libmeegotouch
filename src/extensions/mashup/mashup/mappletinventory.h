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

#ifndef MAPPLETINVENTORY_H
#define MAPPLETINVENTORY_H

#include <QDir>
#include <QStringList>
#include "mappletinventorymodel.h"
#include <MApplicationExtensionInterface>
#include "mappletinstallationsourceinterface.h"

class QFileSystemWatcher;
class MWidget;
class MMashupCanvas;

//! \internal

/*!
 * MAppletInventory is a widget that maintains collection of available
 * applets and instantiates new instances of those applets.
 *
 * MAppletInventory will start to monitor an applet path after
 * setEnabled(true) is called. When active, all applets that are available in
 * the inventory are visualised as instantiable options in this inventory
 * widget.
 *
 * User can instantiate applets from an MAppletInventory by selecting them.
 * When an applet is instantiated from MAppletInventory the widget
 * of the applet is added to the MMashupCanvas container that is specified in
 * the call to the setMashupCanvas() method.
 *
 * Available applets are defined as .desktop metadata files in
 * /usr/share/applications/m/applets. The files conform to the
 * freedesktop.org desktop file format.
 *
 * MAppletInventory inherits from MAppletInventoryInterface interface which
 * applet installation sources can use to instantiate applets in a package. MAppletInventory
 * will create an instance of application extension area and instantiate the available
 * installation sources when needed.
 *
 * \see appletinstallationsource
 */

class MAppletInventory : public MWidgetController, public MAppletInventoryInterface
{
    Q_OBJECT
    M_CONTROLLER(MAppletInventory)

public:
    /*!
     * Constructs an MAppletInventory widget.
     *
     * \param parent the parent widget of the MAppletInventory, defaults to NULL
     */
    MAppletInventory(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the MAppletInventory.
     */
    virtual ~MAppletInventory();

    /*!
     * Sets the mashup canvas to add instantiated applet widgets to.
     *
     * \param mashupCanvas The mashup canvas to add instantiated applet widgets to.
     */
    void setMashupCanvas(MMashupCanvas &mashupCanvas);

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

    //! \reimp
    void instantiateAppletsInPackage(const QString &packageName);
    //! \reimp_end

    /*!
     * \brief Lets the extension know about the applet inventory interface
     */
    void setMAppletInventoryInterface(MApplicationExtensionInterface *extension);

private:
    /*!
     * Initalizes the applet inventory if necessary.
     * Connects applet inventory to a MMashupCanvas widget set using
     * setMashupCanvas and starts monitoring the applet metadata files.
     */
    void initializeIfNecessary();

    //! Mashup canvas for the applet instances
    MMashupCanvas *mashupCanvas;

    //! File system watcher to monitor changes in the applet binary directory
    QFileSystemWatcher *watcher;

    //! Plugin binary path
    QDir appletPath;

    //! A list of names of the applet categories to show in this applet inventory. An empty list will show all applets.
    QStringList appletCategories;

    //! Whether the applet inventory has been initialized or not
    bool initialized;

#ifdef UNIT_TEST
    friend class Ut_MAppletInventory;
#endif
};

//! \internal_end

#endif // MAPPLETINVENTORY_H
