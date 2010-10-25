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

#ifndef MAPPLICATIONEXTENSIONMETADATA_H_
#define MAPPLICATIONEXTENSIONMETADATA_H_

#include <mdesktopentry.h>

/*!
 * This class provides M application extension metadata such as extension
 * binary and extension runner binary.
 *
 * Application extension metadata is stored in a .desktop file.
 *
 * Extensions are ran using an extension runner binary. Each extension has to
 * define an extension binary which is compiled as a shared library. Both
 * extension binaries and extension runner binaries are searched from a
 * centralized place in the file system. The location for extension binaries
 * and extension runner binaries is determined by APPLICATION_EXTENSION_LIBS
 * compile-time definition. All extension binaries and extension runner
 * binaries are searched from this location.
 *
 * \code
 * // Instantiate metadata from an extension file relative to binary path.
 * MApplicationExtensionMetaData data("filename.desktop");
 *
 * // Make sure that application extension metadata is valid before using it.
 * if(data.isValid())
 * {
 *     // Access metadata..
 * }
 * \endcode
 */
class M_EXTENSIONS_EXPORT MApplicationExtensionMetaData : public MDesktopEntry
{
public:
    /*!
     * Constructs a new instance of MApplicationExtensionMetaData by reading the .desktop
     * file in that is given as a construction parameter.
     * \param filename Location of .desktop file to be read by constructed MApplicationExtensionMetaData instance.
     */
    explicit MApplicationExtensionMetaData(const QString &filename);

    /*!
     * Destructor
     */
    virtual ~MApplicationExtensionMetaData();

    /*!
     * Checks if the given metadata file contains all necessary data for an extension.
     * At least extension binary needs to be specified.
     *
     * Note that extension metadata file needs to be valid desktop file before extension metadata-specific
     * keys are investigated.
     * \see MDesktopEntry::isValid()
     */
    bool isValid() const;

    /*!
     * Returns absolute path of the extension runner binary. The extension runner absolute path is
     * effectively the APPLICATION_EXTENSION_LIBS compile-time path definition appended by the extension runner binary
     * defined in the extension metadata.
     *
     * If runner binary is not specified the extension described by this metadata should be ran
     * in the host process.
     *
     * \return Path to the extension runner binary to be used for launching the extension.
     */
    QString runnerBinary() const;

    /*!
     * Get absolute path of the extension binary. The extension binary absolute path is
     * effectively the APPLICATION_EXTENSION_LIBS compile-time path definition appended by the extension binary
     * defined in the extension metadata.
     */
    QString extensionBinary() const;

    /*!
     * Get the name of the interface implemented by this extension.
     */
    QString interface() const;

    /*!
     * Get a resource identifier for the extension, used for specifying the
     * directory of extension resources.
     */
    QString resourceIdentifier() const;

private:
    /**
     * Extracts a library name from a library file name. E.g. "libfoo.so"
     * and "/usr/lib/libfoo.so" both produce "foo" as the name. If the
     * given argument doesn't represent a valid library file name an empty
     * string is returned.
     * \param libFileName the file name of the library.
     */
    static QString extractLibraryName(const QString &libFileName);

    Q_DISABLE_COPY(MApplicationExtensionMetaData)
};

#endif /* MAPPLICATIONEXTENSIONMETADATA_H_ */
