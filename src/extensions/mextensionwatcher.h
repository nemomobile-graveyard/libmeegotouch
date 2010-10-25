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

#ifndef MEXTENSIONWATCHER_H
#define MEXTENSIONWATCHER_H

#include <QFileSystemWatcher>
#include <QHash>
#include <QSharedPointer>
#include "mexport.h"

class MDesktopEntry;

/*!
 * \brief \c MExtensionWatcher provides means to watch if either a given
 * applet or a application extension has been updated by the package manager.
 * If the change happens a corresponding signal will be emitted.
 *
 * The user of this class is responsible for the lifecycle management of the
 * watched entries. This class does not assume ownership of the items nor can
 * it detect if they are deleted. The user must remove the item from this
 * watcher before deleting the item. Otherwise desktop file modification
 * notification leads to undefined behaviour, because the target item of the
 * notification will not exist.
 */
class M_EXTENSIONS_EXPORT MExtensionWatcher : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs the watcher instance
     */
    MExtensionWatcher();


    /*!
     * Destroys the wathcer instance
     */
    virtual ~MExtensionWatcher();

    /*!
     * Add an extension to the list of watched extensions.
     * If the extension (its desktop entry) changes,
     * a signal \see extensionChanged will be emitted.
     *
     * \param extension The extension to watch.
     */
    void addExtension(QSharedPointer<const MDesktopEntry> extension);

    /*!
     * Removes an extension from the list of watched extension
     *
     * \param fileName Name of the desktop file of the extension to
     * remove
     */
    void removeExtension(const QString &fileName);

Q_SIGNALS:

    /*!
     * Informs that an extension (its desktop file) has changed.
     *
     * \param extension The extension that has changed
     */
    void extensionChanged(const MDesktopEntry &extension) const;

private slots:
    //! Listens in on the actual changes
    void notifyDataChanged(const QString &path) const;

private:
    //! The watcher instance for observing changes to desktop files
    mutable QFileSystemWatcher watcher;
    //! The list of applets to watch
    QHash<QString, QSharedPointer<const MDesktopEntry> > extensionsToWatch;

#ifdef UNIT_TEST
    friend class Ut_MExtensionWatcher;
#endif
};

#endif //MEXTENSIONWATCHER_H
