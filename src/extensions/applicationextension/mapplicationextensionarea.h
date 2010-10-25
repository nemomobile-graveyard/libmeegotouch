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

#ifndef MAPPLICATIONEXTENSIONAREA_H
#define MAPPLICATIONEXTENSIONAREA_H

#include "mextensionarea.h"
#include "mapplicationextensionareamodel.h"

class MApplicationExtensionAreaPrivate;
class MApplicationExtensionInterface;

/*!
 * MApplicationExtensionArea is a widget which can be populated with
 * application extensions. MApplicationExtensionArea can be placed on
 * any view that wants to have application extension support.
 *
 * The MApplicationExtensionArea will load application extensions which
 * implement the specified interface.
 *
 * \see \ref applicationextensions
 */
class M_EXTENSIONS_EXPORT MApplicationExtensionArea : public MExtensionArea
{
    Q_OBJECT
    M_CONTROLLER(MApplicationExtensionArea)

public:
    /*!
     * Constructs an application extension area. The area is not initialized
     * until init() has been called.
     *
     * \param interface the name of the interface the application extensions
     *                  must implement in order to be loaded to this
     *                  application extension area
     * \param parent optional parent graphics item
     */
    explicit MApplicationExtensionArea(const QString &interface, QGraphicsItem *parent = NULL);

    /*!
     * Destroys the application extension area.
     */
    virtual ~MApplicationExtensionArea();

    /*!
     * Sets a filter for allowing only certain extensions to be ran in the
     * same process as the extension area. The filter is a regular expression
     * which is matched against the .desktop file names of the extensions. By
     * default all extensions are allowed to be loaded in-process. For example,
     * use QRegExp("$^") to allow no extensions, QRegExp("/test(1|A).desktop$")
     * to allow extensions test1.desktop and testA.desktop and QRegExp() to
     * allow all extensions.
     *
     * Must be called before init().
     * Calling this after init() has no effect.
     *
     * \param inProcessFilter a regular expression for defining which
     *                        extension are allowed in-process
     */
    void setInProcessFilter(const QRegExp &inProcessFilter);

    /*!
     * Sets a filter for allowing only certain extensions to be ran in separate
     * processes. The filter is a regular expression which is matched against
     * the .desktop file names of the extensions. By default all extensions are
     * allowed to be loaded in separate processes. For example,
     * use QRegExp("$^") to allow no extensions, QRegExp("/test(1|A).desktop$")
     * to allow extensions test1.desktop and testA.desktop and QRegExp() to
     * allow all extensions.
     *
     * Must be called before init().
     * Calling this after init() has no effect.
     *
     * \param outOfProcessFilter a regular expression for defining which
     *                           extension are allowed out-of-process
     */
    void setOutOfProcessFilter(const QRegExp &outOfProcessFilter);

    /*!
     * Defines the order in which extensions are placed if they are found. Each
     * extension is placed in the position in which it appears for the first
     * time in the list. An empty string can be used to mark the place for
     * unnamed extensions. Must be called before init(). Calling this
     * after init() has no effect.
     *
     * \param order a list of extension .desktop file names for defining the
     *              order in which certain extensions appear
     */
    void setOrder(const QStringList &order);

    /*!
     * Initializes the application extension area. All setters introduced by
     * this class must be called before the area is initialized, and the
     * signals must be connected, because calling init() will result in
     * the extensions being created. Initialization
     * will fail if no interface name has been provided or if the area has
     * already been initialized.
     *
     * \return \c false if the initialization fails, \c true otherwise
     */
    bool init();

    /*!
     * Returns a list of in process extensions loaded by this manager
     *
     * \return List of application extension interface pointers. Receving party should appropriately cast them.
     */
    QList<MApplicationExtensionInterface*> extensions();

    //! \internal
    /*!
     * Constructs an application extension area. This constructor calls
     * init() immediately so it is not possible to use the functions
     * that need to be called before init() is called.
     *
     * \param interface Specifies which interface the application extensions
     *                  must implement in order to be loaded to this
     *                  application extension area.
     * \param enableInProcessExtensions \c true if in process extensions should
     *                                  be loaded. \c false otherwise
     * \param parent optional parent graphics item
     *
     * \deprecated This constructor is deprecated in favour of
     *             MApplicationExtensionArea(const QString &, QGraphicsItem *)
     *             and will be removed in libmeegotouch 0.21.
     */
    explicit MApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent = NULL);
    //! \internal_end

signals:
    /*!
     * Signal sent when an extension has been instantiated.
     *
     * \param extension the extension that was instantiated
     */
    void extensionInstantiated(MApplicationExtensionInterface *extension);

    /*!
     * Signal sent when an extension has been removed.
     *
     * \param extension the extension that was removed
     */
    void extensionRemoved(MApplicationExtensionInterface* extension);

protected:
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     */
    MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface);

    //! \internal
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     *
     * \deprecated
     */
    MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model,
                              QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(MApplicationExtensionArea)
    Q_DISABLE_COPY(MApplicationExtensionArea)
};

#endif // MAPPLICATIONEXTENSIONAREA_H
