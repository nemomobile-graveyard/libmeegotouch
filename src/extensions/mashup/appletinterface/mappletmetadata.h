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

#ifndef MAPPLETMETADATA_H
#define MAPPLETMETADATA_H

#include <mdesktopentry.h>

/*!
 * This class provides m applet metadata such as applet binary, appletrunner binary,
 * icon and description.
 *
 * Applet metadata is stored in a .desktop file. The metadata file may also contain applet
 * specific metadata such as script or markup source. Applet-specific information can also be accessed
 * through this interface.
 *
 * Applet is ran using applet runner binary. Each applet has to define an applet binary which is
 * compiled as a shared library. Both applet binaries and applet runner binaries are searched
 * from a centralized place in the file system. The location for applet binaries and applet runner binaries
 * is determined by APPLET_LIBS compile-time definition. All applet binaries and applet runner binaries
 * are searched from this location.
 *
 * \code
 * // Instantiate metadata from an applet file relative to binary path.
 * MAppletMetaData data("filename.desktop");
 *
 * // Make sure that applet metadata is valid before using it.
 * if(data.isValid())
 * {
 *     // Access metadata..
 * }
 * \endcode
 *
 * \see \ref appletdevelopment
 */
class M_EXTENSIONS_EXPORT MAppletMetaData : public MDesktopEntry
{
public:
    /*!
     * Constructs a new instance of MAppletMetaData by reading the .desktop
     * file in that is given as a construction parameter.
     * \param filename Location of .desktop file to be read by constructed MAppletMetaData instance.
     */
    MAppletMetaData(const QString &filename);

    /*!
     * Destructor
     */
    virtual ~MAppletMetaData();

    /*!
     * Checks if the given metadata file contains all necessary data for an applet.
     * At least applet binary needs to be specified.
     *
     * Note that applet metadata file needs to be valid desktop file before applet metadata-specific
     * keys are investigated.
     * \see MDesktopEntry::isValid()
     */
    bool isValid() const;

    /*!
     * Returns absolute path of the applet runner binary. The applet runner absolute path is
     * effectively the APPLET_LIBS compile-time path definition appended by the applet runner binary
     * defined in the applet metadata.
     *
     * If runner binary is not specified the applet described by this metadata should be ran
     * in the host process.
     *
     * \return Path to the applet runner binary to be used for launching the applet.
     */
    QString runnerBinary() const;

    /*!
     * Get absolute path of the applet binary. The applet binary absolute path is
     * effectively the APPLET_LIBS compile-time path definition appended by the applet binary
     * defined in the applet metadata.
     */
    QString appletBinary() const;

    /*!
     * Get a resource identifier for the applet, used for specifying the
     * directory of applet resources.
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

    Q_DISABLE_COPY(MAppletMetaData)
};

#endif // MAPPLETMETADATA_H
