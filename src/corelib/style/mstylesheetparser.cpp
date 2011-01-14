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

#include "mstylesheetparser.h"
#include "mstylesheetselector.h"
#include "mclassfactory.h"
#include "mstylesheetattribute.h"
#include "mlogicalvalues.h"

#include <QFile>
#include <QTextStream>
#include <MDebug>
#include <QPair>
#include <QHash>
#include <QColor>
#include <QDir>
#include <QDateTime>

#ifndef Q_OS_WIN
#include <utime.h>
#endif

#include <sys/stat.h>

namespace {
    const unsigned int FILE_VERSION = 17;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
class MStyleSheetParserPrivate
{
public:
    MStyleSheetParserPrivate(const MLogicalValues *logicalValues);

    QChar read(QFile &stream, const QByteArray &delimeters, QByteArray &out);
    QChar peek(QFile &stream, const QByteArray &delimeters, QByteArray &out);

    bool load(const QString &filename, QHash<QByteArray, QByteArray>* constants, bool toplevel);

    /**
     \brief Parse a stylesheet \a file.
     \param fileInfo a QFileInfo for the filename. This parameter is provided so it can be shared by other methods, to avoid unnecessary file IO.
     */
    bool parse(QFile &file, const QFileInfo &fileInfo, bool validateOnly = false);

    MStyleSheetSelector *parseSelector(QFile &stream, bool *error, bool validateOnly);
    QPair<QByteArray, MStyleSheetAttribute *> parseAttribute(QFile &stream, QChar &endCharacter, bool validateOnly);
    bool parseAtToken(QFile &stream, bool validateOnly = false);
    bool importFile(const QByteArray &importedFileName, bool validateOnly = false);
    int getMatchIndex(const QByteArray &str, const QByteArray &charList, int from = 0) const;
    bool replaceConsts(QByteArray &value) const;

    /**
     \brief Get the \a file name for a binary stylesheet cache file corresponding to the
      original stylesheet file.
     */
    QString createBinaryFilename(const QString &filename) const;

    QList<MStyleSheetParser::StylesheetFileInfo *>     fileInfoList;
    MStyleSheetParser::StylesheetFileInfo            *privateFileInfo;

    /**
     \brief Load a previously-cached binary version of the style.
     \param binaryFilename The binary stylesheet file.
     */
    bool loadBinary(const QString &binaryFilename);

    /**
     \brief Write all binary stylesheet data to a file on disk.
     \param binaryFilename The file to create.
     */
    bool dump(const QString &binaryFilename);

    void writeStylesheetFileInfo(MStyleSheetParser::StylesheetFileInfo *selector, QDataStream &stream);
    MStyleSheetParser::StylesheetFileInfo * readStylesheetFileInfo(QDataStream &stream);

    MStyleSheetSelector *readSelector(const QByteArray &file, QDataStream &stream);
    void writeSelector(MStyleSheetSelector *selector, QDataStream &stream);

    static QString getOrientationName(MStyleSheetSelector::Orientation orientation);
    static MStyleSheetSelector::Orientation getOrientationFromName(const QString &name);

    bool binaryFileMode;
    QString binaryDirectory;
    MStyleSheetParser::SyntaxMode syntaxMode;

    void debugOutput();

    static int getLineNum(QFile &stream, const qint64 &streamPos = -1);
    static void outputParseError(const QString &file, const QString &desc, int line);
    static void outputParseWarning(const QString &file, const QString &desc, int line);

    void skipWhiteSpace(QFile &file);

    qint64 startReadPos;
    QFileInfo parsedFileInfo;
    QByteArray parsedFileName;
    const MLogicalValues *logicalValues;

    QHash<QByteArray, QByteArray>* globalConstants;

    bool validName(const QByteArray &name);
    bool validValue(const QByteArray &value);
    
    //cache for frequently used byte arrays to reduce dynamic memory allocations made
    //by byte arrays in stylesheet (attributeName, attributeValue, className, selectorName etc.)
    static QSet<QByteArray> stringCache;
    const QByteArray& cachedString(const QByteArray& str) const;

};

// faster than QFileInfo::lastModified().toTime_t()
time_t modificationTime(const char* filename)
{
    struct stat fileInfo;
    int result = stat(filename, &fileInfo);
    if (result != 0) {
        return 0;
    } else {
        return fileInfo.st_mtime;
    }
}

QSet<QByteArray> MStyleSheetParserPrivate::stringCache;
#endif

MStyleSheetParserPrivate::MStyleSheetParserPrivate(const MLogicalValues *logicalValues) :
    privateFileInfo(0), binaryFileMode(true), syntaxMode(MStyleSheetParser::StrictSyntax),
    startReadPos(0), logicalValues(logicalValues), globalConstants(NULL)
{
}

void MStyleSheetParserPrivate::skipWhiteSpace(QFile &file)
{
    char peek;

    while (file.peek(&peek, 1) != 0) {
        if (QChar(peek).isSpace()) {
            file.read(&peek, 1);
        } else break;
    }
}

void MStyleSheetParserPrivate::outputParseError(const QString &file, const QString &desc, int line)
{
    mWarning("MStyleSheetParserPrivate") << "MCSSParser:" << file << ":" << line << ": error:" << desc;
}

void MStyleSheetParserPrivate::outputParseWarning(const QString &file, const QString &desc, int line)
{
    mWarning("MStyleSheetParserPrivate") << "MCSSParser:" << file << ":" << line << ": warning:" << desc;
}

int MStyleSheetParserPrivate::getLineNum(QFile &stream, const qint64 &streamPos)
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

bool MStyleSheetParserPrivate::validName(const QByteArray &name)
{
    static QRegExp regexp("[^()#*{}:;,\"]+");
    return regexp.exactMatch(name);
}

bool MStyleSheetParserPrivate::validValue(const QByteArray &value)
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

const QByteArray& MStyleSheetParserPrivate::cachedString(const QByteArray& str) const
{
    //insert the byte array to cache and return the cached one
    QSet<QByteArray>::const_iterator i = stringCache.insert(str);
    return (i != stringCache.constEnd()) ? *i : str;
}


void outputSelector(MStyleSheetSelector *selector)
{
    mDebug("MStyleSheetParserPrivate") << "  selector:"
                                           << selector->className()
                                           << selector->classType()
                                           << selector->objectName()
                                           << MStyleSheetParserPrivate::getOrientationName(selector->orientation())
                                           << selector->mode()
                                           << selector->parentName()
                                           << selector->parentObjectName();
    MAttributeList::const_iterator attributesEnd = selector->attributes()->constEnd();
    for (MAttributeList::const_iterator i = selector->attributes()->constBegin();
            i != attributesEnd;
            ++i) {
        mDebug("MStyleShetParserPrivate") << "     attribute:" << i.value()->getName() << "=" << i.value()->getValue();
    }
}

void MStyleSheetParserPrivate::debugOutput()
{
    QList<MStyleSheetParser::StylesheetFileInfo *>::const_iterator fileInfoListEnd = fileInfoList.constEnd();

    for (QList<MStyleSheetParser::StylesheetFileInfo *>::const_iterator fi = fileInfoList.constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        mDebug("MStyleSheetParserPrivate") << "file:" << (*fi)->filename;
        mDebug("MStyleSheetParserPrivate") << "includes:" << (*fi)->includes;
        mDebug("MStyleSheetParserPrivate") << "constants:" << (*fi)->constants;

        QList<MStyleSheetSelector *>::const_iterator selectorsEnd = (*fi)->selectors.constEnd();

        for (QList<MStyleSheetSelector *>::const_iterator iterator = (*fi)->selectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            outputSelector(*iterator);
        }

        selectorsEnd = (*fi)->parentSelectors.constEnd();
        for (QList<MStyleSheetSelector *>::const_iterator iterator = (*fi)->parentSelectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            outputSelector(*iterator);
        }
    }
}

QChar MStyleSheetParserPrivate::read(QFile &stream, const QByteArray &delimeters, QByteArray &out)
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
                //mDebug("MStyleSheetParserPrivate") << "Skipping comment:" << comment;
                skipWhiteSpace(stream);
                continue;
            } else if (in[0] == '/' && in[1] == '*') {
                stream.read(in, 2);
                in[1] = 0;
                // multi-line comment
                QByteArray comment;

                // Read until we get "*/"
                while (stream.atEnd() == false) {
                    stream.read(in, 1);
                    if (in[0] == '/' && in[1] == '*') {
                        break;
                    } else {
                        if(in[1] == '*') {
                            comment += in[1];
                        }
                        if(in[0] == '*') {
                            in[1] = '*';
                        } else {
                            in[1] = 0;
                            comment += in[0];
                        }
                    }
                }
                //mDebug("MStyleSheetParserPrivate") << "Skipping comment:" << comment;
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
                    out = array.trimmed();
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

QChar MStyleSheetParserPrivate::peek(QFile &stream, const QByteArray &delimeters, QByteArray &out)
{
    qint64 startPos = stream.pos();
    QChar result = read(stream, delimeters, out);
    stream.seek(startPos);

    return result;
}

bool MStyleSheetParserPrivate::load(const QString &filename, QHash<QByteArray, QByteArray>* constants, bool toplevel)
{
    globalConstants = constants;

    const QString binaryFilename = createBinaryFilename(filename);
    if (binaryFileMode) {
        // If binary file mode is enabled, we'll check if there's binary
        // file available instead of the ASCII css to speed-up the loading process
        if (toplevel && loadBinary(binaryFilename)) {
            return true;
        }
    }

    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {

        //mDebug("MStyleSheetParserPrivate") << "Loading ASCII css file" << filename;

        privateFileInfo = new MStyleSheetParser::StylesheetFileInfo;
        privateFileInfo->filename = qPrintable(filename);

        bool result = parse(file, filename);
        file.close();

        if (result || syntaxMode == MStyleSheetParser::RelaxedSyntax) {
            fileInfoList.push_back(privateFileInfo);

            // dump this file to the disk for faster access in future?
            if (binaryFileMode) {
                privateFileInfo->time_t = modificationTime(qPrintable(filename));
                if (toplevel) {
                    dump(binaryFilename);
                }
            }

            privateFileInfo = NULL;
            return true;
        } else {
            delete privateFileInfo;
            privateFileInfo = NULL;
            return false;
        }
    }

    mWarning("MStyleSheetParserPrivate") << "Failed to open stylesheet file:" << filename;

    return false;
}

bool MStyleSheetParserPrivate::parse(QFile &file, const QFileInfo &fileInfo, bool validateOnly)
{
    bool result = true;

    parsedFileName = qPrintable(fileInfo.absoluteFilePath());
    parsedFileInfo = fileInfo;


    char peek;
    QByteArray tmp;

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
                parseAtToken(file, validateOnly);
                skipWhiteSpace(file);
            } else {

                // Try to parse selector
                bool error;
                MStyleSheetSelector *selector = parseSelector(file, &error, validateOnly);
                skipWhiteSpace(file);
		if (selector && validateOnly) {
		    delete selector;
		    selector = 0;
		}
                if (!selector && error) {
                    // It wasn't selector.. there was an error..
                    // we don't really know what it is then so fail.
                    result = false;
                    if (syntaxMode == MStyleSheetParser::StrictSyntax)
                        break;
                }
                if (selector) {
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

bool MStyleSheetParserPrivate::parseAtToken(QFile &stream, bool validateOnly)
{
    // Skip whitespaces and get a char from stream
    skipWhiteSpace(stream);

    char in;

    if (stream.read(&in, 1) == 0)
        return false;

    QByteArray filename;

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

        if (!importFile(filename, validateOnly)) {
            outputParseError(parsedFileName, "Failed to import file: \"" + filename + "\"", getLineNum(stream, startReadPos));
            return false;
        }
    }
    //@import "filename";
    //@import url("filename");
    //@const CONSTANT: value(s);
    else {
        //read the keyword
        QByteArray word;
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
            QByteArray import;
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

            if (!importFile(filename, validateOnly)) {
                outputParseError(parsedFileName, "Failed to import file: \"" + filename + "\"", getLineNum(stream, startReadPos));
                return false;
            }
        }
        //@const CONSTANT: value(s);
        else if (word == "const") {
            skipWhiteSpace(stream);
            QByteArray name;
            if (read(stream, ":\n", name) == ':' && validName(name)) {
                skipWhiteSpace(stream);
                QByteArray value;
                if (read(stream, ";\n", value) == ';' && validValue(value)) {
                    if (validateOnly) {
                        return true;
                    }
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

bool MStyleSheetParserPrivate::importFile(const QByteArray &filename, bool validateOnly)
{
    if (validateOnly) {
        return MStyleSheetParser::validate(parsedFileInfo.absolutePath() + QDir::separator() + filename);
    }
    // add imported file to include list
    QByteArray includeFileName = parsedFileInfo.absolutePath().toAscii() + QDir::separator().toAscii() + filename;
    uint lastModified = modificationTime(includeFileName.constData());
    privateFileInfo->includes.push_back(QPair<QByteArray, uint>(includeFileName, lastModified));

    // parse the file
    MStyleSheetParserPrivate parser(logicalValues);
    parser.binaryDirectory = binaryDirectory;
    parser.binaryFileMode = binaryFileMode;
    if (parser.load(parsedFileInfo.absolutePath() + QDir::separator() + filename, globalConstants, false) ||
        syntaxMode == MStyleSheetParser::RelaxedSyntax) {
        // add all the new file infos into the list of parsed files
        while (parser.fileInfoList.count() > 0) {
            MStyleSheetParser::StylesheetFileInfo *fileInfo = parser.fileInfoList.front();
            fileInfoList.push_back(fileInfo);
            parser.fileInfoList.removeFirst();
        }
        return true;
    } else {
        return false;
    }
}

int MStyleSheetParserPrivate::getMatchIndex(const QByteArray &str,
                                            const QByteArray &charList,
                                            int from) const
{
    int minIndex = -1;
    foreach(const char c, charList) {
        int index = str.indexOf(c, from);
        if (minIndex == -1 || (index < minIndex && index != -1))
            minIndex = index;
    }
    return minIndex;
}

bool MStyleSheetParserPrivate::replaceConsts(QByteArray &value) const
{
    if (value.isEmpty())
        return false;

    const QByteArray charsToMatch = " ,()\"";
    int idx1 = 0;
    bool replacementHappened = false;
    while ((idx1 = value.indexOf('$'), idx1) != -1) {
        int idx2 = getMatchIndex(value, charsToMatch, idx1);
        if (idx2 == -1)
            idx2 = value.count();
        idx2 -= 1;
        int num = idx2 - idx1;
        QByteArray constName = value.mid(idx1 + 1, num);

        QHash<QByteArray, QByteArray>::const_iterator iterator = globalConstants->constFind(constName);
        QByteArray constValue = "";
        if (iterator != globalConstants->constEnd()) {
            constValue = (*iterator);
        } else {
            // try to search from logical values
            if (logicalValues) {
                QByteArray group;
                logicalValues->findKey(constName, group, constValue);
            }

            if (constValue.isEmpty()) {
                outputParseWarning(parsedFileName, "Unknown constant variable: " + constName, 0);
            }
        }
        replacementHappened = true;
        value.replace(idx1, num + 1, constValue);
    }

    return replacementHappened;
}

MStyleSheetSelector *MStyleSheetParserPrivate::parseSelector(QFile &stream, bool *error, bool validateOnly)
{
    QByteArray parentName,
            parentObjectName,
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
            flags |= MStyleSheetSelector::Child;
        } else if (result == ' ') {
            // we may have space in front of '>'
            skipWhiteSpace(stream);
            char peek;
            if (!stream.peek(&peek, 1))
                return NULL;
            if (peek == '>') {
                // read it away
                stream.read(&peek, 1);
                flags |= MStyleSheetSelector::Child;
            }
        } else if (result == '#' && className.length() > 0) {
            // we may have the parent object name
            // so we peek and check it
            QByteArray tmp;
            peek(stream, "{", tmp);

            if (tmp.contains(' '))
                flags |= MStyleSheetSelector::Child;
        }

        if ((result == ' ') || (flags & MStyleSheetSelector::Child)) {
            if (result == ' ') {
                result = read(stream, "[#.:{", tmp);
                if (tmp.length() > 0) {
                    parentName = className;
                    className = tmp;
                }
            } else if (result == '#') {
                result = read(stream, " ", tmp);
                if (tmp.length() > 0) {
                    parentName = className;
                    parentObjectName = tmp;
                }
                result = read(stream, "[#.:{", tmp);
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
                flags |= MStyleSheetSelector::ExactClassMatch;
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
                //mWarning("MStyleSheetParserPrivate") << "failed to find block starting bracket { in selector: " << className;
                outputParseError(parsedFileName, "Failed to read object name. '.', ':', or '{' is missing after selector name.", getLineNum(stream, startReadPos));
                return NULL;
            }
        }

        // Do we have an orientation defined?
        if (result == '.') {
            // read orientation name
            result = read(stream, ":{", orientation);
            if (result == 0) {
                //mWarning("MStyleSheetParserPrivate") << "failed to find block starting bracket { in selector: " << className;
                outputParseError(parsedFileName, "Failed to read orientation information. ':' or '{' is missing after selector name.", getLineNum(stream, startReadPos));
                return NULL;
            }
        }

        // Do we have a mode defined?
        if (result == ':') {
            // read mode name
            result = read(stream, "{", mode);
            if (result == 0) {
                //mWarning("MStyleSheetParserPrivate") << "failed to find block starting bracket { in selector: " << className;
                outputParseError(parsedFileName, "Failed to read mode information, '{' is missing after selector name.", getLineNum(stream, startReadPos));
                return NULL;
            }
        }

        qint64 startSelector = startReadPos;


        MStyleSheetSelector *selector = new MStyleSheetSelector(cachedString(objectName),
                cachedString(className),
                cachedString(classType),
                getOrientationFromName(cachedString(orientation)),
                cachedString(mode),
                cachedString(parsedFileName),
                cachedString(parentName),
                parentObjectName,
                (MStyleSheetSelector::Flags) flags);

//        mDebug("MStyleSheetParserPrivate") << "selector found: " << selector->className() << selector->objectName();
        char peek;
        while (stream.atEnd() == false) {
            // Check whether the block is going to end, first skip comments
            read(stream, "", tmp);

            if (stream.peek(&peek, 1) == 0) {
                mWarning("MStyleSheetParserPrivate") << "EOF in selector: " <<
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
            QPair<QByteArray, MStyleSheetAttribute *> result = parseAttribute(stream, character, validateOnly);
            if (!result.second) {
                mWarning("MStyleSheetParserPrivate") << "Attribute read failed in selector: " <<
                        selector->className() + '[' + selector->classType() + "]#" + selector->objectName();

                if (syntaxMode == MStyleSheetParser::RelaxedSyntax)
                    return selector;

                delete selector;
                return NULL;
            }

	    if (validateOnly) {
		delete result.second;
	    }
	    else {
		// Store and parse next
		selector->attributes()->insert(result.first, result.second);
	    }

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

QPair<QByteArray, MStyleSheetAttribute *> MStyleSheetParserPrivate::parseAttribute(QFile &stream, QChar &character, bool validateOnly)
{
    QByteArray name;
    QByteArray value;

    skipWhiteSpace(stream);

    if (read(stream, ":", name) != 0 && validName(name)) {

        // Check out the attribute value
        character = read(stream, ";}", value);

        if (((character == ';') || (character == '}'))  && validValue(value)) {
            MStyleSheetAttribute *result = new MStyleSheetAttribute(
                cachedString(MStyleSheetAttribute::attributeNameToPropertyName(name)),
                cachedString(value),
                startReadPos);

            //if value contains const references save the original value
            //string before replacing const references with real values
            if (!validateOnly && replaceConsts(result->value)) {
                result->value = cachedString(result->value);
                if (!validValue(result->value)) {
                    outputParseError(parsedFileName, "Invalid constant reference in value: " + value, getLineNum(stream, startReadPos));
                }
            }

            return QPair<QByteArray, MStyleSheetAttribute *>(result->name, result);
        } else {
            outputParseError(parsedFileName, "Parse attribute failed, ';' or '}' is missing after value or the value is invalid. Multiline attributes are not supported.", getLineNum(stream, startReadPos));
        }
    } else {
        outputParseError(parsedFileName, "Parse attribute failed, ':' is missing after name or the name is invalid.", getLineNum(stream, startReadPos));
    }

    return QPair<QByteArray, MStyleSheetAttribute *> ("", NULL);
}

QString MStyleSheetParserPrivate::createBinaryFilename(const QString &filename) const
{
    QString binaryFilename(binaryDirectory);
    QString absoluteFilePathEncoded(filename);
    absoluteFilePathEncoded.replace('_', "__");
    absoluteFilePathEncoded.replace('/', "_.");
    binaryFilename += absoluteFilePathEncoded;
    return binaryFilename;
}

MStyleSheetParser::MStyleSheetParser(const MLogicalValues *logicalValues) :
    d_ptr(new MStyleSheetParserPrivate(logicalValues))
{
    Q_D(MStyleSheetParser);
    d->privateFileInfo = 0;
#ifdef Q_WS_X11
    d->binaryDirectory = QString(CACHEDIR) + "/css/";
#else
    d->binaryDirectory = QDir::tempPath() + QDir::separator();
#endif
    d->binaryFileMode = true;

    setSyntaxMode(StrictSyntax);
}

MStyleSheetParser::~MStyleSheetParser()
{
    Q_D(MStyleSheetParser);

    QList<StylesheetFileInfo *>::iterator fileInfoListEnd = d->fileInfoList.end();

    for (QList<StylesheetFileInfo *>::iterator fi = d->fileInfoList.begin();
            fi != fileInfoListEnd;
            ++fi) {
        QList<MStyleSheetSelector *>::iterator selectorsEnd = (*fi)->selectors.end();

        for (QList<MStyleSheetSelector *>::iterator iterator = (*fi)->selectors.begin();
                iterator != selectorsEnd;
                ++iterator) {

            delete *iterator;
        }

        selectorsEnd = (*fi)->parentSelectors.end();
        for (QList<MStyleSheetSelector *>::iterator iterator = (*fi)->parentSelectors.begin();
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

bool MStyleSheetParser::load(const QString &filename)
{
    Q_D(MStyleSheetParser);

    QHash<QByteArray, QByteArray> constants;
    bool result = d->load(filename, &constants, true);
    // uncomment for debug output
    //d->debugOutput();
    return result;
}

QList<MStyleSheetParser::StylesheetFileInfo *>& MStyleSheetParser::fileInfoList() const
{
    return d_ptr->fileInfoList;
}

void MStyleSheetParser::setBinaryFileDirectory(const QString &directory)
{
    Q_D(MStyleSheetParser);
    d->binaryDirectory = directory;
    if (!d->binaryDirectory.isEmpty())
        if (d->binaryDirectory[d->binaryDirectory.length()-1] != QDir::separator())
            d->binaryDirectory += QDir::separator();
}

void MStyleSheetParser::setBinaryFileGenerationEnabled(bool enabled)
{
    Q_D(MStyleSheetParser);
    d->binaryFileMode = enabled;
}

void MStyleSheetParser::setSyntaxMode(SyntaxMode syntaxMode)
{
    Q_D(MStyleSheetParser);
    d->syntaxMode = syntaxMode;
}

MStyleSheetParser::SyntaxMode MStyleSheetParser::syntaxMode() const
{
    Q_D(const MStyleSheetParser);
    return d->syntaxMode;
}

bool MStyleSheetParserPrivate::loadBinary(const QString &binaryFilename)
{
    if (!QFile::exists(binaryFilename))
        return false;

    QFile file(binaryFilename);
    if (file.open(QFile::ReadOnly)) {
        QDataStream stream(&file);

        unsigned int file_version;
        stream >> file_version;

        if (file_version == FILE_VERSION) {
            // read fileinfo
            QList<QPair<QByteArray, qint64> > timestamps;
            stream >> timestamps;
            QListIterator<QPair<QByteArray, qint64> >  it(timestamps);

            while (it.hasNext()) {
                QPair<QByteArray, qint64> ts = it.next();

                time_t current = modificationTime(ts.first.constData());
                if (current != ts.second) {
                    mDebug("MStyleSheetParserPrivate") << "Timestamp for" << ts.first << "changed. Recreating" << binaryFilename;
                    return false;
                }
            }

            QList<uint> logicalTimestamps;
            stream >> logicalTimestamps;
            if (logicalValues && logicalTimestamps != logicalValues->timestamps()) {
                // the logical values have been updated. our constants may have changed
                mDebug("MStyleSheetParserPrivate") << "Recreating" << binaryFilename << "as constants changed";
                return false;
            }

            int nrOfFiles;
            stream >> nrOfFiles;
            for (int i = 0; i < nrOfFiles; ++i) {
                MStyleSheetParser::StylesheetFileInfo *fi = readStylesheetFileInfo(stream);
                fileInfoList.append(fi);
            }
            return true;
        }
        file.close();
        return false;
    }

    mWarning("MStyleSheetParserPrivate") << "Failed to load binary stylesheet file:" << binaryFilename;
    return false;
}

bool MStyleSheetParserPrivate::dump(const QString &binaryFilename)
{
    QFile file(binaryFilename);

    if (!file.open(QFile::WriteOnly)) {
        //Maybe it failed because the directory doesn't exist
        QDir().mkpath(QFileInfo(binaryFilename).absolutePath() );
        if (!file.open(QFile::WriteOnly)) {
            mDebug("MStyleSheetParserPrivate") << "Failed to dump stylesheet file:" << binaryFilename;
            return false;
        }
    }

    // collect timestamps
    QList<QPair<QByteArray, qint64> > timestamps;
    QList<MStyleSheetParser::StylesheetFileInfo *>::const_iterator fileInfoListEnd = fileInfoList.constEnd();
    for (QList<MStyleSheetParser::StylesheetFileInfo *>::const_iterator fi = fileInfoList.constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        timestamps.append(QPair<QByteArray, qint64>((*fi)->filename, (*fi)->time_t));
    }

    QDataStream stream(&file);
    stream << FILE_VERSION;
    stream << timestamps;
    if (logicalValues) {
        stream << logicalValues->timestamps();
    } else {
        stream << QList<uint>();
    }

    stream << fileInfoList.count();

    for (QList<MStyleSheetParser::StylesheetFileInfo *>::const_iterator fi = fileInfoList.constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        writeStylesheetFileInfo(*fi, stream);
    }

    return true;
}

void MStyleSheetParserPrivate::writeStylesheetFileInfo(MStyleSheetParser::StylesheetFileInfo *fi, QDataStream &stream)
{
    stream << fi->filename;
    stream << fi->time_t;

    stream << fi->selectors.count();
    // write all selectors without parent
    QList<MStyleSheetSelector *>::const_iterator infoSelectorsEnd = fi->selectors.constEnd();
    for (QList<MStyleSheetSelector *>::const_iterator iterator = fi->selectors.constBegin();
            iterator != infoSelectorsEnd;
            ++iterator) {
        writeSelector(*iterator, stream);
    }

    // write all selectors with parent
    stream << fi->parentSelectors.count();
    infoSelectorsEnd = fi->parentSelectors.constEnd();
    for (QList<MStyleSheetSelector *>::const_iterator iterator = fi->parentSelectors.constBegin();
            iterator != infoSelectorsEnd;
            ++iterator) {
        writeSelector(*iterator, stream);
    }
}


MStyleSheetParser::StylesheetFileInfo * MStyleSheetParserPrivate::readStylesheetFileInfo(QDataStream &stream)
{
    MStyleSheetParser::StylesheetFileInfo *fi = new MStyleSheetParser::StylesheetFileInfo;

    stream >> fi->filename;
    stream >> fi->time_t;

    int selectorCount;
    stream >> selectorCount;
    // read all selectors without parent
    for (int i = 0; i < selectorCount; ++i) {
        MStyleSheetSelector *selector = readSelector(fi->filename, stream);
        fi->selectors.push_back(selector);
    }

    // read all selectors with parent
    stream >> selectorCount;
    for (int i = 0; i < selectorCount; ++i) {
        MStyleSheetSelector *selector = readSelector(fi->filename, stream);
        fi->parentSelectors.push_back(selector);
    }

    return fi;
}

MStyleSheetSelector *MStyleSheetParserPrivate::readSelector(const QByteArray &file, QDataStream &stream)
{
    QByteArray parentName, parentObjectName, objectName, className, classType, mode;
    int flags, orientation;

    stream >> parentName;
    stream >> parentObjectName;
    stream >> flags;
    stream >> objectName;
    stream >> className;
    stream >> classType;
    stream >> orientation;
    stream >> mode;

    parentName = cachedString(parentName);
    objectName = cachedString(objectName);
    className = cachedString(className);
    classType = cachedString(classType);
    mode = cachedString(mode);

    MStyleSheetSelector *selector = new MStyleSheetSelector(objectName,
            className,
            classType,
            (MStyleSheetSelector::Orientation) orientation,
            mode,
            file,
            parentName,
            parentObjectName,
            (MStyleSheetSelector::Flags) flags);


    // read number of attributes
    int attributeCount;
    stream >> attributeCount;

    // read attributes one by one
    for (int attributeIndex = 0; attributeIndex < attributeCount; ++attributeIndex) {
        QByteArray name;
        stream >> name;
        QByteArray value;
        stream >> value;
        qint64 position;
        stream >> position;

        MStyleSheetAttribute *attribute = new MStyleSheetAttribute(
            cachedString(name),
            cachedString(value),
            position);

        selector->attributes()->insert(attribute->name, attribute);
    }

    return selector;
}

void MStyleSheetParserPrivate::writeSelector(MStyleSheetSelector *selector, QDataStream &stream)
{
    stream << selector->parentName();
    stream << selector->parentObjectName();
    stream << (int) selector->flags();
    stream << selector->objectName();
    stream << selector->className();
    stream << selector->classType();
    stream << (int) selector->orientation();
    stream << selector->mode();

    stream << selector->attributes()->count();

    if (selector->attributes()->count() == 0) {
        mWarning("MStyleSheetParserPrivate") << "Warning:"
                << "Style sheet selector has no attributes:"
                << "objectName:" << selector->objectName()
                << "className:" << selector->className() << "file:"
                << __FILE__ << "line:" << __LINE__ << "function:"
                << __PRETTY_FUNCTION__;
    }

    MAttributeList::const_iterator attributesEnd = selector->attributes()->constEnd();

    for (MAttributeList::const_iterator attributeIterator = selector->attributes()->constBegin();
            attributeIterator != attributesEnd;
            ++attributeIterator) {

        stream << attributeIterator.value()->name;
        stream << attributeIterator.value()->value;
        stream << attributeIterator.value()->position;
    }
}

QString MStyleSheetParserPrivate::getOrientationName(MStyleSheetSelector::Orientation orientation)
{
    if (orientation == MStyleSheetSelector::LandscapeOrientation)
	return "Landscape";
    if (orientation == MStyleSheetSelector::PortraitOrientation)
	return "Portrait";
    return "";
}

MStyleSheetSelector::Orientation MStyleSheetParserPrivate::getOrientationFromName(const QString &name)
{
    if (name == "Landscape")
	return MStyleSheetSelector::LandscapeOrientation;
    if (name == "Portrait")
	return MStyleSheetSelector::PortraitOrientation;
    return MStyleSheetSelector::UndefinedOrientation;
}

void MStyleSheetParser::operator += (const MStyleSheetParser &stylesheet)
{
    Q_D(MStyleSheetParser);

    QList<StylesheetFileInfo *>::const_iterator fileInfoListEnd = stylesheet.fileInfoList().constEnd();

    for (QList<StylesheetFileInfo *>::const_iterator fi = stylesheet.fileInfoList().constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        StylesheetFileInfo *fileInfo = new StylesheetFileInfo;

        fileInfo->filename = (*fi)->filename;
        fileInfo->includes = (*fi)->includes;

        // copy all selectors from other stylesheet to this fileInfo
        QList<MStyleSheetSelector *>::const_iterator selectorsEnd = (*fi)->selectors.constEnd();
        for (QList<MStyleSheetSelector *>::const_iterator iterator = (*fi)->selectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            // selector to copy
            MStyleSheetSelector *selector = (*iterator);
            // append copy of selector
            fileInfo->selectors.push_back(new MStyleSheetSelector(*selector));
        }

        // copy all selectors with parent
        selectorsEnd = (*fi)->parentSelectors.constEnd();
        for (QList<MStyleSheetSelector *>::const_iterator iterator = (*fi)->parentSelectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            // selector to copy
            MStyleSheetSelector *selector = (*iterator);
            // append copy of selector
            fileInfo->parentSelectors.push_back(new MStyleSheetSelector(*selector));
        }
        d->fileInfoList.push_back(fileInfo);
    }
}

int MStyleSheetParser::getLineNum(QFile &stream, const qint64 &streamPos)
{
    return MStyleSheetParserPrivate::getLineNum(stream, streamPos);
}

int MStyleSheetParser::getLineNum(const QString &filename, const qint64 &streamPos)
{
    QFile file(filename);
    if ( file.open(QFile::ReadOnly) ) {
        return getLineNum(file, streamPos);
    } else {
        return 0;
    }
}


void MStyleSheetParser::outputParseError(const QString &filename, const QString &description, int lineNum)
{
    MStyleSheetParserPrivate::outputParseError(filename, description, lineNum);
}

void MStyleSheetParser::outputParseWarning(const QString &filename, const QString &description, int lineNum)
{
    MStyleSheetParserPrivate::outputParseWarning(filename, description, lineNum);
}

void MStyleSheetParser::cleanup()
{
    //remove strings with only one reference from the cache
    QSet<QByteArray>::iterator i = MStyleSheetParserPrivate::stringCache.begin();
    while( i !=  MStyleSheetParserPrivate::stringCache.end() ) {
        if( (*i).isDetached() ) {
            i = MStyleSheetParserPrivate::stringCache.erase(i);
        }
        else
            ++i;
    }
}

bool MStyleSheetParser::validate(const QString &filename)
{
    MStyleSheetParserPrivate *p = new MStyleSheetParserPrivate(0);

    bool result = false;
    QFile file(filename);
    QFileInfo fileInfo(filename);
    if (file.open(QFile::ReadOnly)) {
	MStyleSheetParser::StylesheetFileInfo *privateFileInfo = new MStyleSheetParser::StylesheetFileInfo;
        privateFileInfo->filename = filename.toAscii();

        result = p->parse(file, fileInfo, true);
        file.close();
	delete privateFileInfo;
    }
    return result;
}

