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

#ifndef MSETTINGSLANGUAGEPARSER_H
#define MSETTINGSLANGUAGEPARSER_H

#include <MExport>
#include <QString>
#include <QDomDocument>
#include <QStringList>
#include "msettingslanguagebinary.h"

class MSettingsLanguageParserPrivate;

/*!
 * \brief Parser class for the settings language.
 *
 * This class can be used to read in a settings language description
 * and transfer it into a binary representation.
 */
class M_SETTINGS_EXPORT MSettingsLanguageParser
{
public:
    /*!
     * Constructs a parser.
     */
    MSettingsLanguageParser();

    /*!
     * Destructor.
     */
    virtual ~MSettingsLanguageParser();

    /*!
     * Reads an XML file from the provided device. Returns \c true on
     * success, \c false on failure.
     * \param device the device to read from.
     */
    bool readFrom(QIODevice &device);

    /*!
     * Constructs and returns a settings binary representation from
     * the parsed language source. Returns \c NULL on error.
     *
     * \note The ownership of the returned binary is transfered to the caller.
     * \return a settigns binary representation.
     */
    MSettingsLanguageBinary *createSettingsBinary();

protected:
    //! A pointer to the private class
    MSettingsLanguageParserPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MSettingsLanguageParser)
    Q_DECLARE_PRIVATE(MSettingsLanguageParser)
};

#endif // MSETTINGSLANGUAGEPARSER_H
