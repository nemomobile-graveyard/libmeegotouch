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

#include "duistylesheetparser.h"
#include "duistylesheetselector.h"
#include "duiclassfactory.h"
#include "duistylesheetattribute.h"
#include "duilogicalvalues.h"

#include <QFile>
#include <QTextStream>
#include <DuiDebug>
#include <QPair>
#include <QHash>
#include <QColor>
#include <QDir>
#include <QDateTime>

#ifndef Q_OS_WIN
#include <utime.h>
#endif

#include <sys/stat.h>


#define FILE_VERSION(major, minor) (int)((major<<16)|minor)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
class DuiStyleSheetParserPrivate
{
public:
    DuiStyleSheetParserPrivate(const DuiLogicalValues *logicalValues);

    QChar read(QFile &stream, const QString &delimeters, QString &out);

    bool load(const QString &filename, QHash<QString, QString>* constants);

    /**
     \brief Parse a stylesheet \a file.
     \param fileInfo a QFileInfo for the filename. This parameter is provided so it can be shared by other methods, to avoid unnecessary file IO.
     */
    bool parse(QFile &file, const QFileInfo &fileInfo);

    DuiStyleSheetSelector *parseSelector(QFile &stream, bool *error);
    QPair<QString, DuiStyleSheetAttribute *> parseAttribute(QFile &stream, QChar &endCharacter);
    bool parseAtToken(QFile &stream);
    bool importFile(const QString &importedFileName);
    bool hasConsts(const QString &value) const;
    void setupConsts(QString &value) const;

    /**
     \brief Get the \a file name for a binary stylesheet cache file corresponding to the
      original stylesheet file.
     \param fileInfo a QFileInfo for the filename, reused if possible to avoid unnecessary file IO.
     */
    QString createBinaryFilename(const QFileInfo &fileInfo) const;

    QList<DuiStyleSheetParser::StylesheetFileInfo *>     fileInfoList;
    DuiStyleSheetParser::StylesheetFileInfo            *privateFileInfo;

    /**
     \brief Load a previously-cached binary version of the style.
     \param filename The stylesheet file.
     \param binaryFilename The binary stylesheet file.
     */
    bool loadBinary(const QString &filename, const QString &binaryFilename);

    /**
     \brief Write the binary stylesheet data to a file on disk.
     \param info The parsed stylesheet data.
     \param binaryFilename The file to create.
     */
    bool dump(const DuiStyleSheetParser::StylesheetFileInfo &info, const QString &binaryFilename);

    DuiStyleSheetSelector *readSelector(const QString &file, QDataStream &stream);
    void writeSelector(DuiStyleSheetSelector *selector, QDataStream &stream);

    bool binaryFileMode;
    QString binaryDirectory;

    void debugOutput();

    static int getLineNum(QFile &stream, const qint64 &streamPos = -1);
    static void outputParseError(const QString &file, const QString &desc, int line);
    static void outputParseWarning(const QString &file, const QString &desc, int line);

    void skipWhiteSpace(QFile &file);

    qint64 startReadPos;
    QFileInfo parsedFileInfo;
    QString parsedFileName;
    const DuiLogicalValues *logicalValues;

    QHash<QString, QString>* globalConstants;

    bool validName(const QString &name);
    bool validValue(const QString &value);
};
#endif

DuiStyleSheetParserPrivate::DuiStyleSheetParserPrivate(const DuiLogicalValues *logicalValues) :
    logicalValues(logicalValues)
{
}

void DuiStyleSheetParserPrivate::skipWhiteSpace(QFile &file)
{
    char peek;

    while (file.peek(&peek, 1) != 0) {
        if (QChar(peek).isSpace()) {
            file.read(&peek, 1);
        } else break;
    }
}

void DuiStyleSheetParserPrivate::outputParseError(const QString &file, const QString &desc, int line)
{
    duiWarning("DuiStyleSheetParserPrivate") << "DuiCSSParser:" << file << ":" << line << ": error:" << desc;
}

void DuiStyleSheetParserPrivate::outputParseWarning(const QString &file, const QString &desc, int line)
{
    duiWarning("DuiStyleSheetParserPrivate") << "DuiCSSParser:" << file << ":" << line << ": warning:" << desc;
}

int DuiStyleSheetParserPrivate::getLineNum(QFile &stream, const qint64 &streamPos)
{
    qint64 realPos = stream.pos();
    qint64 pos = streamPos;
    if (pos == -1)
        pos = realPos;

    stream.seek(0);
    int l = 0;
    bool foundpos = false;
    while (stream.atEnd() == false && foundpos == false) {
        ++l;

        stream.readLine();
        if (pos < stream.pos()) {
            foundpos = true;
        }
    }

    stream.seek(realPos);

    if (foundpos)
        return l;
    else {
        return 0;
    }
}

bool DuiStyleSheetParserPrivate::validName(const QString &name)
{
    static QRegExp regexp("[^()#*{}:;,\"]+");
    return regexp.exactMatch(name);
}

bool DuiStyleSheetParserPrivate::validValue(const QString &value)
{
    if (!value.isEmpty()) {
        //check if there is quotes
        if (value.count('\"') > 0) {
            //value is invalid if there is odd number of "
            if (value.count('\"') % 2 != 0) {
                return false;
            } else {
                //find invalid characters outside quotes,
                //quotes can contain any character
                static QRegExp regexp("((\"[^\"]*\")|([^\"*{}:;\n]+))+");
                return regexp.exactMatch(value);
            }
        } else {
            //find invalid characters
            static QRegExp regexp("[^*{}:;\n]+");
            return regexp.exactMatch(value);
        }
    }

    return true;
}

void outputSelector(DuiStyleSheetSelector *selector)
{
    duiDebug("DuiStyleSheetParserPrivate") << "  selector:"
                                           << selector->className()
                                           << selector->classType()
                                           << selector->objectName()
                                           << selector->orientation()
                                           << selector->mode()
                                           << selector->parentName();
    DuiAttributeList::const_iterator attributesEnd = selector->attributes()->constEnd();
    for (DuiAttributeList::const_iterator i = selector->attributes()->constBegin();
            i != attributesEnd;
            ++i) {
        duiDebug("DuiStyleShetParserPrivate") << "     attribute:" << i.value()->name << "=" << i.value()->value;
    }
}

void DuiStyleSheetParserPrivate::debugOutput()
{
    QList<DuiStyleSheetParser::StylesheetFileInfo *>::const_iterator fileInfoListEnd = fileInfoList.constEnd();

    for (QList<DuiStyleSheetParser::StylesheetFileInfo *>::const_iterator fi = fileInfoList.constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        duiDebug("DuiStyleSheetParserPrivate") << "file:" << (*fi)->filename;
        duiDebug("DuiStyleSheetParserPrivate") << "includes:" << (*fi)->includes;
        duiDebug("DuiStyleSheetParserPrivate") << "constants:" << (*fi)->constants;

        QList<DuiStyleSheetSelector *>::const_iterator selectorsEnd = (*fi)->selectors.constEnd();

        for (QList<DuiStyleSheetSelector *>::const_iterator iterator = (*fi)->selectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            outputSelector(*iterator);
        }

        selectorsEnd = (*fi)->parentSelectors.constEnd();
        for (QList<DuiStyleSheetSelector *>::const_iterator iterator = (*fi)->parentSelectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            outputSelector(*iterator);
        }
    }
}

QChar DuiStyleSheetParserPrivate::read(QFile &stream, const QString &delimeters, QString &out)
{
    QByteArray array;
    skipWhiteSpace(stream);

    startReadPos = stream.pos();

    char in[2];
    int quot = 0;
    while (stream.atEnd() == false) {
        // peek next character
        if (stream.peek(in, 2) == 2) {
            // Handle potential comments
            if (in[0] == '/' && in[1] == '/') {
                stream.read(in, 2);
                // Single line comment, read the rest of the line
                // and go back to read new characters
                stream.readLine();
                //duiDebug("DuiStyleSheetParserPrivate") << "Skipping comment:" << comment;
                skipWhiteSpace(stream);
                continue;
            } else if (in[0] == '/' && in[1] == '*') {
                stream.read(in, 2);
                // multi-line comment
                int count = 0;
                QString comment;

                // Read until we get "*/"
                while (stream.atEnd() == false) {
                    stream.read(in, 1);
                    if (count == 0 && in[0] == '*')
                        count++;
                    else if (count == 1 && in[0] == '/')
                        break;
                    else {
                        comment += in[0];
                        count = 0;
                    }
                }
                //duiDebug("DuiStyleSheetParserPrivate") << "Skipping comment:" << comment;
                skipWhiteSpace(stream);
                continue;
            }
        }
        // If there's no delimeters we'll stop right away,
        // in this case we'll just skip comments
        if (delimeters.length() == 0) {
            return QChar(0);
        }

        char character;
        stream.read(&character, 1);
        if (character == '"')
            quot++;

        //do not check delimeters inside quotes/string
        if (quot % 2 == 0 || character == '"') {
            // Keep checking if we have found a delimeter
            // character, specified by caller
            const int length = delimeters.length();
            for (int i = 0; i < length; ++i) {
                // Found a delimeter?
                if (character == delimeters[i]) {
                    // Trim the string and return
                    out = QString::fromUtf8(array.data()).trimmed();
                    return character;
                }
            }
        }
        // store to the out buffer
        array += character;
    }

    if (quot % 2 != 0)
        outputParseError(parsedFileName, "Missing terminating '\"' character", getLineNum(stream, startReadPos));

    // Didn't find the delimeter character
    if (array.length() > 0)
        outputParseError(parsedFileName, "Unknown token: " + QString(array.data()), getLineNum(stream, startReadPos));

    // EOF, return zero
    return QChar(0);
}

bool DuiStyleSheetParserPrivate::load(const QString &filename, QHash<QString, QString>* constants)
{
    globalConstants = constants;

    QFileInfo fileInfo(filename);
    const QString binaryFilename = createBinaryFilename(fileInfo);
    if (binaryFileMode) {
        // If binary file mode is enabled, we'll check if there's binary
        // file available instead of the ASCII css to speed-up the loading process

        if (fileInfo.exists() && loadBinary(filename, binaryFilename))
            return true;
    }


    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {

        //duiDebug("DuiStyleSheetParserPrivate") << "Loading ASCII css file" << filename;

        privateFileInfo = new DuiStyleSheetParser::StylesheetFileInfo;
        privateFileInfo->filename = filename;

        bool result = parse(file, fileInfo);
        file.close();

        if (result) {
            fileInfoList.push_back(privateFileInfo);

            // dump this file to the disk for faster access in future?
            if (binaryFileMode)
                dump(*privateFileInfo, binaryFilename);

            privateFileInfo = NULL;
            return true;
        } else {
            delete privateFileInfo;
            privateFileInfo = NULL;
            return false;
        }
    }

    duiWarning("DuiStyleSheetParserPrivate") << "Failed to open stylesheet file:" << filename;

    return false;
}

bool DuiStyleSheetParserPrivate::parse(QFile &file, const QFileInfo &fileInfo)
{
    bool result = true;

    parsedFileName = fileInfo.absoluteFilePath();
    parsedFileInfo = fileInfo;


    char peek;
    QString tmp;

    skipWhiteSpace(file);
    while (file.atEnd() == false) {

        // Skip comments
        read(file, "", tmp);
        skipWhiteSpace(file);

        // Get character from stream
        if (file.peek(&peek, 1) != 0) {

            // Include?
            if (peek == '@') {
                file.read(&peek, 1);
                parseAtToken(file);
                skipWhiteSpace(file);
            } else {

                // Try to parse selector
                bool error;
                DuiStyleSheetSelector *selector = parseSelector(file, &error);
                skipWhiteSpace(file);
                if (!selector && error) {
                    // It wasn't selector.. there was an error..
                    // we don't really know what it is then so fail.
                    result = false;
                    break;
                } else if (selector) {
                    if (selector->parentName().isEmpty()) {
                        privateFileInfo->selectors.push_back(selector);
                    } else {
                        privateFileInfo->parentSelectors.push_back(selector);
                    }
                }
            }
        }
    }

    return result;
}

bool DuiStyleSheetParserPrivate::parseAtToken(QFile &stream)
{
    // Skip whitespaces and get a char from stream
    skipWhiteSpace(stream);

    char in;

    if (stream.read(&in, 1) == 0)
        return false;

    QString filename;

    //TODO this should be removed at some point
    //old style include
    //@(filename)
    if (in == '(') {
        if (read(stream, ")", filename) == 0) {
            outputParseWarning(parsedFileName, "Old style include, use @import instead.", getLineNum(stream, startReadPos));
            outputParseError(parsedFileName, "Invalid old style include token.", getLineNum(stream, startReadPos));
            return false;
        }
        outputParseWarning(parsedFileName, "Old style include, use @import instead.", getLineNum(stream, startReadPos));

        if (!importFile(filename)) {
            outputParseError(parsedFileName, "Failed to import file: \"" + filename + "\"", getLineNum(stream, startReadPos));
            return false;
        }
    }
    //@import "filename";
    //@import url("filename");
    //@const CONSTANT: value(s);
    else {
        //read the keyword
        QString word;
        if (read(stream, " \n", word) != ' ') {
            outputParseError(parsedFileName, "Invalid @ token.", getLineNum(stream, startReadPos));
            return false;
        }

        //add the character we already parsed
        word = in + word;

        //@import "filename";
        //@import url("filename");
        if (word == "import") {
            skipWhiteSpace(stream);
            QString import;
            if (read(stream, ";", import) != 0) {

                int idx0 = import.indexOf('\"');
                int idx1 = import.lastIndexOf('\"');

                if (idx0 == idx1 || idx0 == -1 || idx1 == -1) {
                    outputParseError(parsedFileName, "Invalid import token, the imported file must be inside \"quotes\".", getLineNum(stream, startReadPos));
                    return false;
                }

                filename = import.mid(idx0 + 1, idx1 - idx0 - 1);

                /*if( uri == "url(" || uri.isEmpty() ) {
                    if (read(stream, ";\n", filename) != ';') {
                        outputParseError(parsedFileName, "Invalid import token, ';' is missing from the end.", getLineNum(stream, startReadPos));
                        return false;
                    }

                    int idx = filename.lastIndexOf('\"');
                    if( idx == -1 ) {
                        outputParseError(parsedFileName, "Invalid import token, the imported file must be inside \"quotes\".", getLineNum(stream, startReadPos));
                        return false;
                    }

                    filename = filename.left(idx);
                }
                else {
                    outputParseError(parsedFileName, "Invalid import token, the imported file must be inside \"quotes\".", getLineNum(stream, startReadPos));
                    return false;
                }*/
            } else {
                outputParseError(parsedFileName, "Invalid import token, ';' is missing from the end.", getLineNum(stream, startReadPos));
                return false;
            }

            if (!importFile(filename)) {
                outputParseError(parsedFileName, "Failed to import file: \"" + filename + "\"", getLineNum(stream, startReadPos));
                return false;
            }
        }
        //@const CONSTANT: value(s);
        else if (word == "const") {
            skipWhiteSpace(stream);
            QString name;
            if (read(stream, ":\n", name) == ':' && validName(name)) {
                skipWhiteSpace(stream);
                QString value;
                if (read(stream, ";\n", value) == ';' && validValue(value)) {
                    //add the constant to this file's constant list
                    if (privateFileInfo->constants.contains(name)) {
                        outputParseWarning(parsedFileName, "Multiple definition of constant \"" + name + "\". Ignoring redefinition.", getLineNum(stream, startReadPos));
                    } else {
                        privateFileInfo->constants.insert(name, value);
                    }

                    //add the constant to the list of global constants as well
                    if (globalConstants->contains(name)) {
                        outputParseWarning(parsedFileName, "Multiple definition of constant \"" + name + "\". Ignoring redefinition.", getLineNum(stream, startReadPos));
                    } else {
                        globalConstants->insert(name, value);
                    }
                } else {
                    outputParseError(parsedFileName, "Invalid constant definition, ';' is missing after constant value.", getLineNum(stream, startReadPos));
                    return false;
                }
            } else {
                outputParseError(parsedFileName, "Invalid constant definition, ':' is missing after constant name.", getLineNum(stream, startReadPos));
                return false;
            }
        } else {
            outputParseError(parsedFileName, "Unsupported @ token: " + word, getLineNum(stream, startReadPos));
            return false;
        }
    }

    return true;
}

bool DuiStyleSheetParserPrivate::importFile(const QString &filename)
{
    // add imported file to include list
    privateFileInfo->includes.push_back(parsedFileInfo.absolutePath() + QDir::separator() + filename);

    // parse the file
    DuiStyleSheetParserPrivate parser(logicalValues);
    parser.binaryDirectory = binaryDirectory;
    parser.binaryFileMode = binaryFileMode;
    if (parser.load(parsedFileInfo.absolutePath() + QDir::separator() + filename, globalConstants)) {
        // add all the new file infos into the list of parsed files
        while (parser.fileInfoList.count() > 0) {
            DuiStyleSheetParser::StylesheetFileInfo *fileInfo = parser.fileInfoList.front();
            fileInfoList.push_back(fileInfo);
            parser.fileInfoList.removeFirst();
        }
        return true;
    } else {
        return false;
    }
}

bool DuiStyleSheetParserPrivate::hasConsts(const QString &value) const
{
    if (value.isEmpty())
        return false;

    //if value is not string and it contains at least one '$'
    if (value[0] != '\"' &&  value.indexOf('$') != -1)
        return true;
    return false;
}

void DuiStyleSheetParserPrivate::setupConsts(QString &value) const
{
    if (value.isEmpty())
        return;

    //constants references inside string values are not allowed
    if (value[0] == '\"') {
        return;
    }

    bool result = false;
    int idx1 = 0;
    while ((idx1 = value.indexOf('$')) != -1) {
        int idx2 = value.indexOf(QRegExp("[ ,()]"), idx1);
        if (idx2 == -1)
            idx2 = value.count();
        idx2 -= 1;
        int num = idx2 - idx1;
        QString constName = value.mid(idx1 + 1, num);

        QHash<QString, QString>::const_iterator iterator = globalConstants->constFind(constName);
        QString constValue = "";
        if (iterator != globalConstants->constEnd()) {
            constValue = (*iterator);
        } else {
            // try to search from logical values
            if (logicalValues) {
                QString group;
                logicalValues->findKey(constName, group, constValue);
            }

            if (constValue.isEmpty()) {
                outputParseWarning(parsedFileName, "Unknown constant variable: " + constName, 0);
            }
        }

        value.replace(idx1, num + 1, constValue);
        result = true;
    }

    // we replaced something, it may be another const string, so we need to process it again
    if (result == true) {
        setupConsts(value);
    }
}

DuiStyleSheetSelector *DuiStyleSheetParserPrivate::parseSelector(QFile &stream, bool *error)
{
    QString parentName,
            className,
            classType,
            objectName,
            orientation,
            mode,
            tmp;

    int flags = 0;

    if (error) {
        *error = true;
    }
    skipWhiteSpace(stream);

    QChar result = read(stream, " >[#.:{", className);
    if (result != 0) {
        if (result == '>') {
            flags |= DuiStyleSheetSelector::Child;
        } else if (result == ' ') {
            // we may have space in front of '>'
            skipWhiteSpace(stream);
            char peek;
            if (!stream.peek(&peek, 1))
                return NULL;
            if (peek == '>') {
                // read it away
                stream.read(&peek, 1);
                flags |= DuiStyleSheetSelector::Child;
            }

        }

        if ((result == ' ') || (flags & DuiStyleSheetSelector::Child)) {
            result = read(stream, "[#.:{", tmp);
            if (tmp.length() > 0) {
                parentName = className;
                className = tmp;
            }
        }

        if ((className.length() == 0) && (result == '.')) {
            // Starts with '.', read the actual name and combine it with the dot
            result = read(stream, "[#.:{", className);
            if (result == 0) {
                outputParseError(parsedFileName, "Invalid class name.", getLineNum(stream, startReadPos));
                return 0;
            }

            className.insert(0, '.');
        }

        if (className.length() > 0) {
            if (className.startsWith('.')) {
                className.remove(0, 1);
                flags |= DuiStyleSheetSelector::ExactClassMatch;
            }
        }

        if ((className.length() != 0) && result == '[') {
            result = read(stream, "]", classType);
            if (result == 0) {
                outputParseError(parsedFileName, "Invalid class name.", getLineNum(stream, startReadPos));
                return 0;
            }

            result = read(stream, "#.:{", tmp);
            if (!tmp.isEmpty()) {
                outputParseError(parsedFileName, "Invalid class name.", getLineNum(stream, startReadPos));
                return 0;
            }
        }

        // Do we have an object name?
        if (result == '#') {
            // read object name
            result = read(stream, ".:{", objectName);
            if (result == 0) {
                //duiWarning("DuiStyleSheetParserPrivate") << "failed to find block starting bracket { in selector: " << className;
                outputParseError(parsedFileName, "Failed to read object name. '.', ':', or '{' is missing after selector name.", getLineNum(stream, startReadPos));
                return NULL;
            }
        }

        // Do we have an orientation defined?
        if (result == '.') {
            // read orientation name
            result = read(stream, ":{", orientation);
            if (result == 0) {
                //duiWarning("DuiStyleSheetParserPrivate") << "failed to find block starting bracket { in selector: " << className;
                outputParseError(parsedFileName, "Failed to read orientation information. ':' or '{' is missing after selector name.", getLineNum(stream, startReadPos));
                return NULL;
            }
        }

        // Do we have a mode defined?
        if (result == ':') {
            // read mode name
            result = read(stream, "{", mode);
            if (result == 0) {
                //duiWarning("DuiStyleSheetParserPrivate") << "failed to find block starting bracket { in selector: " << className;
                outputParseError(parsedFileName, "Failed to read mode information, '{' is missing after selector name.", getLineNum(stream, startReadPos));
                return NULL;
            }
        }

        qint64 startSelector = startReadPos;


        DuiStyleSheetSelector *selector = new DuiStyleSheetSelector(objectName,
                className,
                classType,
                orientation,
                mode,
                parsedFileName,
                parentName,
                (DuiStyleSheetSelector::Flags) flags);

//        duiDebug("DuiStyleSheetParserPrivate") << "selector found: " << selector->className() << selector->objectName();
        char peek;
        while (stream.atEnd() == false) {
            // Check whether the block is going to end, first skip comments
            read(stream, "", tmp);

            if (stream.peek(&peek, 1) == 0) {
                duiWarning("DuiStyleSheetParserPrivate") << "EOF in selector: " <<
                        selector->className() + '[' + selector->classType() + "]#" + selector->objectName();
                delete selector;
                return NULL;
            }

            if (peek == '}') {
                stream.read(&peek, 1);
                if (error) {
                    *error = false;
                }
                return selector;
            }

            // Parse attribute, if it fails, terminate
            QChar character;
            QPair<QString, DuiStyleSheetAttribute *> result = parseAttribute(stream, character);
            if (!result.second) {
                duiWarning("DuiStyleSheetParserPrivate") << "Attribute read failed in selector: " <<
                        selector->className() + '[' + selector->classType() + "]#" + selector->objectName();
                delete selector;
                return NULL;
            }

            // Store and parse next
            selector->attributes()->insert(result.first, result.second);

            // last character was closing the whole selector -> we're done
            if (character == '}') {
                return selector;
            }
        }

        //the end bracket of the selector was not found
        outputParseError(parsedFileName, "Invalid selector, '}' is missing from the end.", getLineNum(stream, startSelector));
        delete selector;
        return NULL;
    } else if (className.length() == 0) {
        // We got comment or something similar,
        // -> not an error but not an selector either
        if (error) {
            *error = false;
        }
        return NULL;
    }

    if (className.length() > 0) {
        outputParseError(parsedFileName, "Invalid class name: " + className + '.', getLineNum(stream, startReadPos));
    }

    return NULL;
}

QPair<QString, DuiStyleSheetAttribute *> DuiStyleSheetParserPrivate::parseAttribute(QFile &stream, QChar &character)
{
    QString name;
    QString value;

    skipWhiteSpace(stream);

    if (read(stream, ":", name) != 0 && validName(name)) {

        // Check out the attribute value
        character = read(stream, ";}", value);

        if (((character == ';') || (character == '}'))  && validValue(value)) {
            DuiStyleSheetAttribute *result = new DuiStyleSheetAttribute;
            result->name = name;
            result->value = value;
            result->constValue = "";
            result->position = startReadPos;

            if (hasConsts(result->value)) {
                //if value contains const references save the original value
                //string before replacing const references with real values
                result->constValue = result->value;
                setupConsts(result->value);
                if (!validValue(result->value)) {
                    outputParseError(parsedFileName, "Invalid constant reference in value: " + value, getLineNum(stream, startReadPos));
                }
            }

            return QPair<QString, DuiStyleSheetAttribute *> (name, result);
        } else {
            outputParseError(parsedFileName, "Parse attribute failed, ';' or '}' is missing after value or the value is invalid. Multiline attributes are not supported.", getLineNum(stream, startReadPos));
        }
    } else {
        outputParseError(parsedFileName, "Parse attribute failed, ':' is missing after name or the name is invalid.", getLineNum(stream, startReadPos));
    }

    return QPair<QString, DuiStyleSheetAttribute *> ("", NULL);
}

QString DuiStyleSheetParserPrivate::createBinaryFilename(const QFileInfo &fileInfo) const
{
    QString binaryFilename(binaryDirectory);
    QString absoluteFilePathEncoded(fileInfo.absoluteFilePath());
    absoluteFilePathEncoded.replace('_', "__");
    absoluteFilePathEncoded.replace('/', "_.");
    binaryFilename += absoluteFilePathEncoded;
    binaryFilename += fileInfo.lastModified().toString("-yyMMdd-hhmmss");
    return binaryFilename;
}

DuiStyleSheetParser::DuiStyleSheetParser(const DuiLogicalValues *logicalValues) :
    d_ptr(new DuiStyleSheetParserPrivate(logicalValues))
{
    Q_D(DuiStyleSheetParser);
    d->privateFileInfo = 0;
    d->binaryDirectory = QDir::tempPath() + QDir::separator();
    d->binaryFileMode = true;
}

DuiStyleSheetParser::~DuiStyleSheetParser()
{
    Q_D(DuiStyleSheetParser);

    QList<StylesheetFileInfo *>::iterator fileInfoListEnd = d->fileInfoList.end();

    for (QList<StylesheetFileInfo *>::iterator fi = d->fileInfoList.begin();
            fi != fileInfoListEnd;
            ++fi) {
        QList<DuiStyleSheetSelector *>::iterator selectorsEnd = (*fi)->selectors.end();

        for (QList<DuiStyleSheetSelector *>::iterator iterator = (*fi)->selectors.begin();
                iterator != selectorsEnd;
                ++iterator) {

            delete *iterator;
        }

        selectorsEnd = (*fi)->parentSelectors.end();
        for (QList<DuiStyleSheetSelector *>::iterator iterator = (*fi)->parentSelectors.begin();
                iterator != selectorsEnd;
                ++iterator) {

            delete *iterator;
        }
        delete *fi;
    }

    d->fileInfoList.clear();

    delete d->privateFileInfo;
    delete d_ptr;
}

bool DuiStyleSheetParser::load(const QString &filename)
{
    Q_D(DuiStyleSheetParser);

    QHash<QString, QString> constants;
    bool result = d->load(filename, &constants);
    // uncomment for debug output
    //d->debugOutput();
    return result;
}

QList<DuiStyleSheetParser::StylesheetFileInfo *>& DuiStyleSheetParser::fileInfoList() const
{
    return d_ptr->fileInfoList;
}

void DuiStyleSheetParser::setBinaryFileDirectory(const QString &directory)
{
    Q_D(DuiStyleSheetParser);
    d->binaryDirectory = directory;
    if (!d->binaryDirectory.isEmpty())
        if (d->binaryDirectory[d->binaryDirectory.length()-1] != QDir::separator())
            d->binaryDirectory += QDir::separator();
}

void DuiStyleSheetParser::setBinaryFileGenerationEnabled(bool enabled)
{
    Q_D(DuiStyleSheetParser);
    d->binaryFileMode = enabled;
}

bool DuiStyleSheetParserPrivate::loadBinary(const QString &filename, const QString &binaryFilename)
{
    // Check that the file exists:
    if (!QFile::exists(binaryFilename))
        return false;

    // all ok, proceed to loading phase
    QFile file(binaryFilename);
    if (file.open(QFile::ReadOnly)) {

        //duiDebug("DuiStyleSheetParserPrivate") << "Loading binary css file" << binaryFilename;
        bool result = false;

        // Create readable datastream
        QDataStream stream(&file);

        // read version number (32bit, 16 major, 16 minor)
        int file_version;
        stream >> file_version;

        // This is how we read v0.9 files
        if (file_version == FILE_VERSION(0, 9)) {
            // read fileinfo
            DuiStyleSheetParser::StylesheetFileInfo *fileinfo = new DuiStyleSheetParser::StylesheetFileInfo;

            stream >> fileinfo->filename;
            stream >> fileinfo->includes;
            stream >> fileinfo->constants;

            // load includes
            const int includesCount = fileinfo->includes.count();
            for (int i = 0; i < includesCount; ++i) {
                load(fileinfo->includes[i], globalConstants);
            }

            QHashIterator<QString, QString> i(fileinfo->constants);
            while (i.hasNext()) {
                i.next();
                if (globalConstants->contains(i.key())) {
                    outputParseWarning(fileinfo->filename, "Multiple definition of constant \"" + i.key() + "\". Ignoring redefinition.", 0);
                } else {
                    globalConstants->insert(i.key(), i.value());
                }
            }

            // parse the rest of the file
            int selectorCount;

            // read all selectors
            stream >> selectorCount;
            for (int i = 0; i < selectorCount; ++i) {
                DuiStyleSheetSelector *selector = readSelector(fileinfo->filename, stream);
                fileinfo->selectors.push_back(selector);
            }

            // read all selectors with parent
            stream >> selectorCount;
            for (int i = 0; i < selectorCount; ++i) {
                DuiStyleSheetSelector *selector = readSelector(fileinfo->filename, stream);
                fileinfo->parentSelectors.push_back(selector);
            }

            fileInfoList.push_back(fileinfo);

            result = true;
        }
        file.close();

        return result;
    }

    duiWarning("DuiStyleSheetParserPrivate") << "Failed to load binary stylesheet file:" << filename;

    return false;
}

bool DuiStyleSheetParserPrivate::dump(const DuiStyleSheetParser::StylesheetFileInfo &info, const QString &binaryFilename)
{
    QFile file(binaryFilename);

    if (file.open(QFile::WriteOnly)) {

        // Create writable datastream
        QDataStream stream(&file);

        // write version number (32bit, 16 major, 16 minor)
        stream << (int) FILE_VERSION(0, 9);
        stream << info.filename;
        stream << info.includes;
        stream << info.constants;

        // write number of selectors
        stream << info.selectors.count();

        // write all selectors
        QList<DuiStyleSheetSelector *>::const_iterator infoSelectorsEnd = info.selectors.constEnd();
        for (QList<DuiStyleSheetSelector *>::const_iterator iterator = info.selectors.constBegin();
                iterator != infoSelectorsEnd;
                ++iterator) {
            writeSelector(*iterator, stream);
        }

        // write all selectors with parent
        stream << info.parentSelectors.count();
        infoSelectorsEnd = info.parentSelectors.constEnd();
        for (QList<DuiStyleSheetSelector *>::const_iterator iterator = info.parentSelectors.constBegin();
                iterator != infoSelectorsEnd;
                ++iterator) {
            writeSelector(*iterator, stream);
        }

        file.close();
        return true;
    }

    duiWarning("DuiStyleSheetParserPrivate") << "Failed to dump stylesheet file:" << info.filename;

    return false;
}

DuiStyleSheetSelector *DuiStyleSheetParserPrivate::readSelector(const QString &file, QDataStream &stream)
{
    QString parentName, objectName, className, classType, orientation, mode;
    int flags;

    stream >> parentName;
    stream >> flags;
    stream >> objectName;
    stream >> className;
    stream >> classType;
    stream >> orientation;
    stream >> mode;

    DuiStyleSheetSelector *selector = new DuiStyleSheetSelector(objectName,
            className,
            classType,
            orientation,
            mode,
            file,
            parentName,
            (DuiStyleSheetSelector::Flags) flags);


    // read number of attributes
    int attributeCount;
    stream >> attributeCount;

    // read attributes one by one
    for (int attributeIndex = 0; attributeIndex < attributeCount; ++attributeIndex) {

        DuiStyleSheetAttribute *attribute = new DuiStyleSheetAttribute;
        stream >> attribute->name;
        stream >> attribute->value;
        stream >> attribute->constValue;
        stream >> attribute->position;

        //check if the value has used constants
        //the constants might have changed so
        //we need to recreate them
        if (!attribute->constValue.isEmpty()) {
            attribute->value = attribute->constValue;
            setupConsts(attribute->value);
            //if( !validValue(attribute->value) )
            //    outputParseError(fileinfo->filename, "Invalid constant reference in value: " + attribute->value, 0);
        }

        selector->attributes()->insert(attribute->name, attribute);
    }

    return selector;
}

void DuiStyleSheetParserPrivate::writeSelector(DuiStyleSheetSelector *selector, QDataStream &stream)
{
    stream << selector->parentName();
    stream << (int) selector->flags();
    stream << selector->objectName();
    stream << selector->className();
    stream << selector->classType();
    stream << selector->orientation();
    stream << selector->mode();

    stream << selector->attributes()->count();

    if (selector->attributes()->count() == 0) {
        duiWarning("DuiStyleSheetParserPrivate") << "Warning:"
                << "Style sheet selector has no attributes:"
                << "objectName:" << selector->objectName()
                << "className:" << selector->className() << "file:"
                << __FILE__ << "line:" << __LINE__ << "function:"
                << __PRETTY_FUNCTION__;
    }

    DuiAttributeList::const_iterator attributesEnd = selector->attributes()->constEnd();

    for (DuiAttributeList::const_iterator attributeIterator = selector->attributes()->constBegin();
            attributeIterator != attributesEnd;
            ++attributeIterator) {

        stream << attributeIterator.value()->name;
        stream << attributeIterator.value()->value;
        stream << attributeIterator.value()->constValue;
        stream << attributeIterator.value()->position;
    }
}


void DuiStyleSheetParser::operator += (const DuiStyleSheetParser &stylesheet)
{
    Q_D(DuiStyleSheetParser);

    QList<StylesheetFileInfo *>::const_iterator fileInfoListEnd = stylesheet.fileInfoList().constEnd();

    for (QList<StylesheetFileInfo *>::const_iterator fi = stylesheet.fileInfoList().constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        StylesheetFileInfo *fileInfo = new StylesheetFileInfo;

        fileInfo->filename = (*fi)->filename;
        fileInfo->includes = (*fi)->includes;

        // copy all selectors from other stylesheet to this fileInfo
        QList<DuiStyleSheetSelector *>::const_iterator selectorsEnd = (*fi)->selectors.constEnd();
        for (QList<DuiStyleSheetSelector *>::const_iterator iterator = (*fi)->selectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            // selector to copy
            DuiStyleSheetSelector *selector = (*iterator);
            // append copy of selector
            fileInfo->selectors.push_back(new DuiStyleSheetSelector(*selector));
        }

        // copy all selectors with parent
        selectorsEnd = (*fi)->parentSelectors.constEnd();
        for (QList<DuiStyleSheetSelector *>::const_iterator iterator = (*fi)->parentSelectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            // selector to copy
            DuiStyleSheetSelector *selector = (*iterator);
            // append copy of selector
            fileInfo->parentSelectors.push_back(new DuiStyleSheetSelector(*selector));
        }
        d->fileInfoList.push_back(fileInfo);
    }
}

int DuiStyleSheetParser::getLineNum(QFile &stream, const qint64 &streamPos)
{
    return DuiStyleSheetParserPrivate::getLineNum(stream, streamPos);
}

int DuiStyleSheetParser::getLineNum(const QString &filename, const qint64 &streamPos)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    return getLineNum(file, streamPos);
}


void DuiStyleSheetParser::outputParseError(const QString &filename, const QString &description, int lineNum)
{
    DuiStyleSheetParserPrivate::outputParseError(filename, description, lineNum);
}

void DuiStyleSheetParser::outputParseWarning(const QString &filename, const QString &description, int lineNum)
{
    DuiStyleSheetParserPrivate::outputParseWarning(filename, description, lineNum);
}

