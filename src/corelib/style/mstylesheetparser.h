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

#ifndef MSTYLESHEETPARSER_H
#define MSTYLESHEETPARSER_H

#include "mexport.h"

#include <QString>
#include <QList>
#include <QVector>
#include <QHash>

class MStyleSheetParserPrivate;
class MStyleSheetSelector;
class QTextStream;
class QFile;
class MLogicalValues;

/*!
 * Parser class for .css style sheets
 *
 * This class extracts all the selectors from style sheet
 * and creates attributes into them.
 */
class M_EXPORT MStyleSheetParser
{
public:
    //! \internal
    struct StylesheetFileInfo {
        QString                         filename;
        QVector<QString>                includes;
        QList<MStyleSheetSelector *>   selectors;
        QList<MStyleSheetSelector *>   parentSelectors;
        QHash<QString, QString>         constants;
        uint                            time_t;
    };
    //! \internal_end

    /*!
     * Constructor for MStyleSheetParser.
     */
    MStyleSheetParser(const MLogicalValues *logicalValues = NULL);

    /*!
     * Destructor for MStyleSheetParser class.
     */
    virtual ~MStyleSheetParser();

    /*!
     * Appends data from other stylesheet to this stylesheet.
     * \param stylesheet Style sheet to be appended
     */
    void operator += (const MStyleSheetParser &stylesheet);

    /*!
     * Loads a .css style sheet file. The contents of the style sheet are
     * stored as a MStyleSheetSelector instances and all the attributes
     * are stored inside them.
     * \param filename Style sheet filename
     * \return true if the style sheet was successfully loaded, otherwise false.
     */
    bool load(const QString &filename);

    /*!
     * Sets binary file directory, which is used to read/write binary stylesheet files. By default, system tmp
     * directory is used.
     * \param directory Binary stylesheet directory
     */
    void setBinaryFileDirectory(const QString &directory);

    /*!
     * Sets binary file generation mode. If it's enabled the parser will use binary copies of stylesheet files to speed-up
     * file operations, trading disk space for speed.
     * \param enabled       If true, binary copies of the files will be used to speed-up the file operations.
     */
    void setBinaryFileGenerationEnabled(bool enabled);

    /*!
     * Returns all the information within the style sheet
     * \return A list of file information structures in this style sheet.
     */
    QList<MStyleSheetParser::StylesheetFileInfo *>& fileInfoList() const;

protected:
    static void outputParseError(const QString &filename, const QString &description, int lineNum);
    static void outputParseWarning(const QString &filename, const QString &description, int lineNum);

    static int getLineNum(const QString &filename, const qint64 &streamPos);
    static int getLineNum(QFile &stream, const qint64 &streamPos = -1);

private:
    Q_DISABLE_COPY(MStyleSheetParser)
    Q_DECLARE_PRIVATE(MStyleSheetParser)
protected:
    MStyleSheetParserPrivate *const d_ptr;

    friend class MStyleSheetAttribute;
};

#endif
