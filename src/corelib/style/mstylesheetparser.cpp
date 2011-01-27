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
#include "msystemdirectories.h"

#include <QFile>
#include <QTextStream>
#include <MDebug>
#include <QPair>
#include <QHash>
#include <QColor>
#include <QDir>
#include <QDateTime>
#include <QList>
#include <QSystemSemaphore>
#include <QScopedArrayPointer>

#ifndef Q_OS_WIN
#include <utime.h>
#endif

#include <sys/stat.h>
#include <sys/file.h>

namespace {
    const unsigned int FILE_VERSION = 21;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

MStyleSheetParser::StylesheetFileInfo::StylesheetFileInfo() :
    time_t(0),
    fromMapedMemory(false)
{
}

MStyleSheetParser::StylesheetFileInfo::~StylesheetFileInfo()
{
    if (!fromMapedMemory) {
        qDeleteAll(selectors);
        qDeleteAll(parentSelectors);
    }
    selectors.clear();
    parentSelectors.clear();
}

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
    MStyleSheetAttribute* parseAttribute(QFile &stream, QChar &endCharacter, bool validateOnly);
    bool parseAtToken(QFile &stream, bool validateOnly = false);
    bool importFile(const QByteArray &importedFileName, bool validateOnly = false);
    int getMatchIndex(const QByteArray &str, const QByteArray &charList, int from = 0) const;
    bool replaceConsts(QByteArray &value) const;
    MStyleSheetSelector* createSelector(
             const QByteArray& objectName, const QByteArray& className, const QByteArray& classType, const QByteArray& orientation,
             const QByteArray& mode, const QByteArray& parentName, const QByteArray& parentObjectName, int flags,
             const QMap<MUniqueStringCache::Index, MStyleSheetAttribute*>& attributeList);

    /**
     \brief Convert an absolute path into a filename by replacing
     line separators and such.
     */
    QString createBinaryFilename(const QString &filename) const;

    /**
      * \brief Return the newest binary CSS cache file for a given
      * CSS file.
      */
    QString currentBinaryCacheFile(const QString &filename) const;

    /**
      * \brief When we have to update an existing binary cache file we
      * may not overwride existing files as they may be mapped by another
      * application. Instead we increase the number at the end of the cache
      * filename. This function calculates the proper name for a new cache file.
      */
    QString nextBinaryCacheFile(const QString &filename) const;

    QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> > fileInfoList;
    QSharedPointer<MStyleSheetParser::StylesheetFileInfo> privateFileInfo;

    /**
     \brief Load a previously-cached binary version of the style.
     \param binaryFilename The binary stylesheet file.
     */
    bool loadBinary(const QString &binaryFilename);

    /**
      * Returns the approximated size of the current stylesheet. The returned number is
      * equal to or larger than the actual size. It can be used to allocate a buffer to
      * store the serialized stylesheet in.
      */
    qint64 approximateSize();

    /**
     \brief Write all binary stylesheet data to a file on disk.
     \param binaryFilename The file to create.
     */
    bool dump(const QString &binaryFilename);

    void writeStylesheetFileInfo(QSharedPointer<MStyleSheetParser::StylesheetFileInfo> selector, char** buffer);
    QSharedPointer<MStyleSheetParser::StylesheetFileInfo> readStylesheetFileInfo(char** buffer);

    const MStyleSheetSelector *readSelector(char** buffer);
    void writeSelector(const MStyleSheetSelector *selector, char** buffer) const;

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
    QSystemSemaphore cssSemaphore;
    QList<QSharedPointer<QFile> > mappedFiles;
    QHash<QByteArray, QByteArray>* globalConstants;

    bool validName(const QByteArray &name);
    bool validValue(const QByteArray &value);
};

namespace {
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

void fixAlignement(char** buffer, int size)
{
    size_t remainder = (size_t)*buffer % size;
    if (remainder != 0) {
        *buffer += size - remainder;
    }
}

void incrementBuffer(char** buffer, int length)
{
    *buffer += length;
}

template <typename T>
T readInteger(char** buffer)
{
    fixAlignement(buffer, sizeof(T));
    T number = *reinterpret_cast<T*>(*buffer);
    incrementBuffer(buffer, sizeof(T));
    return number;
}

template <typename T>
void writeInteger(T number, char** buffer)
{
    fixAlignement(buffer, sizeof(T));
    *reinterpret_cast<T*>(*buffer) = number;
    incrementBuffer(buffer, sizeof(T));
}

class MFileLocker {
public:
    static bool readLock(const QFile& file)
    {
        bool result = (flock(file.handle(), LOCK_SH) == 0);
        if (!result) {
            mWarning("MFileLocker") << "Could not read lock" << file.fileName();
        }
        return result;
    }

    static bool writeLock(const QFile& file)
    {
        bool result = (flock(file.handle(), LOCK_EX) == 0);
        if (!result) {
            mWarning("MFileLocker") << "Could not write lock" << file.fileName();
        }
        return result;
    }

    static bool readLockNB(const QFile& file)
    {
        bool result = (flock(file.handle(), LOCK_SH | LOCK_NB) == 0);
        return result;
    }

    static bool writeLockNB(const QFile& file)
    {
        bool result = (flock(file.handle(), LOCK_EX | LOCK_NB) == 0);
        return result;
    }

    static bool unlock(const QFile& file)
    {
        bool result = flock(file.handle(), LOCK_UN);
        if (!result) {
            mWarning("MFileLocker") << "Could not unlock" << file.fileName();
        }
        return result;
    }
};
}
#endif

MStyleSheetParserPrivate::MStyleSheetParserPrivate(const MLogicalValues *logicalValues) :
    privateFileInfo(0), binaryFileMode(true), syntaxMode(MStyleSheetParser::StrictSyntax),
    startReadPos(0), logicalValues(logicalValues),
    cssSemaphore("MSTYLESHEET_PARSER_CSS_GUARD", 1),
    globalConstants(NULL)
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

void outputSelector(const MStyleSheetSelector *selector)
{
    mDebug("MStyleSheetParserPrivate") << "  selector:"
                                           << selector->className()
                                           << selector->classType()
                                           << selector->objectName()
                                           << MStyleSheetParserPrivate::getOrientationName(selector->orientation())
                                           << selector->mode()
                                           << selector->parentName()
                                           << selector->parentObjectName();
    const MStyleSheetAttribute *attributes = selector->attributeList();
    for (int i = 0; i < selector->attributeCount(); ++i) {
        mDebug("MStyleShetParserPrivate") << "     attribute:" <<  attributes[i].getName() << "=" << attributes[i].getValue();
    }
}

void MStyleSheetParserPrivate::debugOutput()
{
    QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::const_iterator fileInfoListEnd = fileInfoList.constEnd();

    for (QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::const_iterator fi = fileInfoList.constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        mDebug("MStyleSheetParserPrivate") << "file:" << (*fi)->filename;
        mDebug("MStyleSheetParserPrivate") << "includes:" << (*fi)->includes;
        mDebug("MStyleSheetParserPrivate") << "constants:" << (*fi)->constants;

        QList<const MStyleSheetSelector *>::const_iterator selectorsEnd = (*fi)->selectors.constEnd();

        for (QList<const MStyleSheetSelector *>::const_iterator iterator = (*fi)->selectors.constBegin();
                iterator != selectorsEnd;
                ++iterator) {
            outputSelector(*iterator);
        }

        selectorsEnd = (*fi)->parentSelectors.constEnd();
        for (QList<const MStyleSheetSelector *>::const_iterator iterator = (*fi)->parentSelectors.constBegin();
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

    const QString binaryFilename = currentBinaryCacheFile(filename);
    if (toplevel && binaryFileMode) {
        // If binary file mode is enabled, we'll check if there's binary
        // file available instead of the ASCII css to speed-up the loading process
        if (!binaryFilename.isEmpty() && loadBinary(binaryFilename)) {
            return true;
        }
    }

    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QScopedPointer<MUniqueStringCacheLocker> locker1;
        QScopedPointer<MUniqueStringCacheLocker> locker2;
        if (toplevel) {
            // we lock the caches while parsing to avoid the need
            // to lock/unlock it for every string we add
            locker1.reset(new MUniqueStringCacheLocker(MStyleSheetParser::stringCacheWithoutReverseLookup()));
            if (!locker1->isLocked()) {
                return false;
            }
            locker2.reset(new MUniqueStringCacheLocker(MStyleSheetParser::stringCacheWithReverseLookup()));
            if (!locker2->isLocked()) {
                return false;
            }
        }


        //mDebug("MStyleSheetParserPrivate") << "Loading ASCII css file" << filename;

        privateFileInfo = QSharedPointer<MStyleSheetParser::StylesheetFileInfo>(new MStyleSheetParser::StylesheetFileInfo);
        privateFileInfo->filename = qPrintable(filename);

        bool result = parse(file, filename);
        file.close();

        if (result || syntaxMode == MStyleSheetParser::RelaxedSyntax) {
            fileInfoList.push_back(privateFileInfo);

            // dump this file to the disk for faster access in future?
            if (binaryFileMode) {
                privateFileInfo->time_t = modificationTime(qPrintable(filename));
                if (toplevel) {
                    // make sure two applications try to write cache files at once
                    if (!cssSemaphore.acquire()) {
                        mWarning("MStyleSheetParser::load") << "Failed to acquire css guard semaphore.";
                        return false;
                    }
                    dump(nextBinaryCacheFile(filename));
                    cssSemaphore.release();
                    // now clear the stylesheets we just created and load the from the cache file
                    // to make use of mmaping
                    fileInfoList.clear();
                    privateFileInfo.clear();
                    return loadBinary(currentBinaryCacheFile(filename));
                }
            }

            privateFileInfo.clear();
            return true;
        } else {
            privateFileInfo.clear();
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
            QSharedPointer<MStyleSheetParser::StylesheetFileInfo> fileInfo = parser.fileInfoList.front();
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

        QMap<MUniqueStringCache::Index, MStyleSheetAttribute*> attributeList;
        char peek;
        while (stream.atEnd() == false) {
            // Check whether the block is going to end, first skip comments
            read(stream, "", tmp);

            if (stream.peek(&peek, 1) == 0) {
                mWarning("MStyleSheetParserPrivate") << "EOF in selector: " <<
                        className + '[' + classType + "]#" + objectName;
                return NULL;
            }

            if (peek == '}') {
                stream.read(&peek, 1);
                if (error) {
                    *error = false;
                }
                return createSelector(objectName, className, classType, orientation, mode, parentName, parentObjectName, flags, attributeList);
            }

            // Parse attribute, if it fails, terminate
            QChar character;
            MStyleSheetAttribute* result = parseAttribute(stream, character, validateOnly);
            if (!result) {
                mWarning("MStyleSheetParserPrivate") << "Attribute read failed in selector: " <<
                        className + '[' + classType + "]#" + objectName;

                if (syntaxMode == MStyleSheetParser::RelaxedSyntax) {
                    return createSelector(objectName, className, classType, orientation, mode, parentName, parentObjectName, flags, attributeList);
                } else {
                    return NULL;
                }
            }

            if (validateOnly) {
                delete result;
            }
            else {
                // Store and parse next
                attributeList.insert(result->name, result);
            }

            // last character was closing the whole selector -> we're done
            if (character == '}') {
                return createSelector(objectName, className, classType, orientation, mode, parentName, parentObjectName, flags, attributeList);
            }
        }

        //the end bracket of the selector was not found
        outputParseError(parsedFileName, "Invalid selector, '}' is missing from the end.", getLineNum(stream, startSelector));
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

MStyleSheetSelector* MStyleSheetParserPrivate::createSelector(
         const QByteArray& objectName, const QByteArray& className, const QByteArray& classType, const QByteArray& orientation,
         const QByteArray& mode, const QByteArray& parentName, const QByteArray& parentObjectName, int flags,
         const QMap<MUniqueStringCache::Index, MStyleSheetAttribute*>& attributeList)
 {
     // convert the map into a sorted array
     MStyleSheetAttribute *attributeArray = new MStyleSheetAttribute[attributeList.size()];
     int counter = 0;
     QMapIterator<MUniqueStringCache::Index, MStyleSheetAttribute *>iter(attributeList);
     while (iter.hasNext()) {
         iter.next();
         attributeArray[counter] = MStyleSheetAttribute(*iter.value());
         ++counter;
     }

     MStyleSheetSelector *selector = new MStyleSheetSelector(
             attributeArray,
             attributeList.size(),
             MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(objectName),
             MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(className),
             MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(classType),
             getOrientationFromName(orientation),
             MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(mode),
             MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(parentName),
             MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(parentObjectName),
             (MStyleSheetSelector::Flags) flags
             );

     return selector;
 }

MStyleSheetAttribute* MStyleSheetParserPrivate::parseAttribute(QFile &stream, QChar &character, bool validateOnly)
{
    QByteArray name;
    QByteArray value;

    skipWhiteSpace(stream);

    if (read(stream, ":", name) != 0 && validName(name)) {

        // Check out the attribute value
        character = read(stream, ";}", value);

        if (((character == ';') || (character == '}'))  && validValue(value)) {
            QByteArray valueWithoutConsts = value;
            if (!validateOnly && replaceConsts(valueWithoutConsts)) {
                if (!validValue(valueWithoutConsts)) {
                    outputParseError(parsedFileName, "Invalid constant reference in value: " + value, getLineNum(stream, startReadPos));
                }
            }

            MStyleSheetAttribute *result = new MStyleSheetAttribute(
                MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName(name)),
                MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(valueWithoutConsts),
                startReadPos);

            return result;
        } else {
            outputParseError(parsedFileName, "Parse attribute failed, ';' or '}' is missing after value or the value is invalid. Multiline attributes are not supported.", getLineNum(stream, startReadPos));
        }
    } else {
        outputParseError(parsedFileName, "Parse attribute failed, ':' is missing after name or the name is invalid.", getLineNum(stream, startReadPos));
    }

    return 0;
}

QString MStyleSheetParserPrivate::createBinaryFilename(const QString &filename) const
{
    QString absoluteFilePathEncoded(filename);
    absoluteFilePathEncoded.replace(QLatin1Char('_'), QLatin1String("__"));
    absoluteFilePathEncoded.replace(QLatin1Char('/'), QLatin1String("_."));
    // also replace windows directory separators and the drive letter separator
    absoluteFilePathEncoded.replace(QLatin1Char('\\'), QLatin1String("_."));
    absoluteFilePathEncoded.replace(QLatin1Char(':'), QLatin1String("_."));
    absoluteFilePathEncoded.append(QLatin1Char('#'));
    return absoluteFilePathEncoded;
}

QString MStyleSheetParserPrivate::currentBinaryCacheFile(const QString &filename) const
{
    const QString filter = createBinaryFilename(filename).append(QLatin1Char('*'));
    const QDir cacheDir(binaryDirectory, filter, QDir::Name, QDir::Files);
    const QStringList files = cacheDir.entryList();
    if (files.isEmpty()) {
        return QString();
    } else {
        // we extract the index from all cache files and return the cache file with the
        // highest index to make sure we are sorting by number and not alphanumeric.
        // we want #1, #2, #10, #20 and not #1, #10, #2, #20
        int highestIndex  = -1;
        QString currentFile;
        foreach (const QString &file, files) {
            int indexOfHash = file.lastIndexOf(QLatin1Char('#'));
            int index = file.right(file.size() - indexOfHash - 1).toInt();
            if (index > highestIndex) {
                highestIndex = index;
                currentFile = file;
            }
        }

        return binaryDirectory + currentFile;
    }
}

QString MStyleSheetParserPrivate::nextBinaryCacheFile(const QString &filename) const
{
    const QString binaryFilename = createBinaryFilename(filename);
    const QString currentFile = currentBinaryCacheFile(filename);
    if (currentFile.isEmpty()) {
        return binaryDirectory + binaryFilename + QLatin1String("000");
    } else {
        int indexOfHash = currentFile.lastIndexOf(QLatin1Char('#'));
        int currentIndex = currentFile.right(currentFile.size() - indexOfHash - 1).toInt();
        QString newIndex;
        newIndex.setNum(currentIndex + 1);
        return binaryDirectory + binaryFilename + newIndex.rightJustified(3, QLatin1Char('0'));
    }
}

MStyleSheetParser::MStyleSheetParser(const MLogicalValues *logicalValues) :
    d_ptr(new MStyleSheetParserPrivate(logicalValues))
{
    Q_D(MStyleSheetParser);
    d->binaryDirectory = MSystemDirectories::cacheDirectory() + QLatin1String("css") + QDir::separator();
    d->binaryFileMode = true;

    setSyntaxMode(StrictSyntax);
}

MStyleSheetParser::~MStyleSheetParser()
{
    Q_D(MStyleSheetParser);

    d->fileInfoList.clear();

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

QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >& MStyleSheetParser::fileInfoList() const
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

    QSharedPointer<QFile> file(new QFile(binaryFilename));
    if (file->open(QFile::ReadOnly)) {
        if (!MFileLocker::readLock(*file)) {
            return false;
        }
        if (file->size() == 0) {
            // we managed to win a race here. another process is trying to fill the cache file
            // while we managed to acquire the lock before the writing process could acquire the lock.
            // as this should nearly never happen we simply return false for simplicity.
            return false;
        }

        const char* buffer = reinterpret_cast<const char*>(file->map(0, file->size()));
        if (!buffer) {
            mWarning("MStyleSheetParserPrivate::loadBinary") << "Could not map" << file->fileName();
            return false;
        }
        mappedFiles.append(file);
        file->close();

        char *bufferIterator = const_cast<char*>(buffer);
        unsigned int file_version = readInteger<unsigned int>(&bufferIterator);

        if (file_version == FILE_VERSION) {
            quint32 timestampCount = readInteger<quint32>(&bufferIterator);
            for (quint32 i = 0; i < timestampCount; ++i) {
                MUniqueStringCache::Index fileNameIndex = readInteger<MUniqueStringCache::Index>(&bufferIterator);
                QByteArray fileName = MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(fileNameIndex);

                qint64 timestamp = readInteger<qint64>(&bufferIterator);

                time_t current = modificationTime(fileName);
                if (current != timestamp) {
                    mDebug("MStyleSheetParserPrivate::loadBinary") << fileName << "changed. Recreating cache file";
                    return false;
                }
            }

            QList<uint> logicalTimestamps;
            quint32 logicalTimestampCount = readInteger<int>(&bufferIterator);
            for (quint32 i = 0; i < logicalTimestampCount; ++i) {
                uint ts = readInteger<quint32>(&bufferIterator);
                logicalTimestamps.append(ts);
            }

            if (logicalValues && logicalTimestamps != logicalValues->timestamps()) {
                mDebug("MStyleSheetParserPrivate") << "Recreating" << binaryFilename << "as constants changed";
                return false;
            }

            int nrOfFiles = readInteger<int>(&bufferIterator);
            for (int i = 0; i < nrOfFiles; ++i) {
                QSharedPointer<MStyleSheetParser::StylesheetFileInfo> fi = readStylesheetFileInfo(&bufferIterator);
                fileInfoList.append(fi);
            }
            return true;
        }

        return false;
    }

    mWarning("MStyleSheetParserPrivate") << "Failed to load binary stylesheet file:" << binaryFilename;
    return false;
}

qint64 MStyleSheetParserPrivate::approximateSize()
{
    qint64 size = 0;

    // file version
    size += sizeof(unsigned int);

    //file info count + data + alignement
    size += sizeof(int) + fileInfoList.count() * (2*sizeof(int) + sizeof(qint64));

    // logical window timestamps
    size += sizeof(int);
    if (logicalValues) {
        size += logicalValues->timestamps().count() * sizeof(uint);
    }

    // file info count
    size += sizeof(int);

    QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::const_iterator fileInfoListEnd = fileInfoList.constEnd();
    for (QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::const_iterator fi = fileInfoList.constBegin();
    fi != fileInfoListEnd;
    ++fi) {
        // filename
        size += sizeof(uint);

        // timestamp
        size += sizeof(uint);

        // selector count
        size += sizeof(int);

        QList<const MStyleSheetSelector *>::const_iterator infoSelectorsEnd = (*fi)->selectors.constEnd();
        for (QList<const MStyleSheetSelector *>::const_iterator iterator = (*fi)->selectors.constBegin();
        iterator != infoSelectorsEnd;
        ++iterator) {
            size += sizeof(MStyleSheetSelector);
            // alignement
            size += 4;
            size += (*iterator)->attributeCount() * sizeof(MStyleSheetAttribute);
        }

        // parent selector count
        size += sizeof(int);
        infoSelectorsEnd = (*fi)->parentSelectors.constEnd();
        for (QList<const MStyleSheetSelector *>::const_iterator iterator = (*fi)->parentSelectors.constBegin();
        iterator != infoSelectorsEnd;
        ++iterator) {
            size += sizeof(MStyleSheetSelector);
            // alignement
            size += 4;
            size += (*iterator)->attributeCount() * sizeof(MStyleSheetAttribute);
        }
    }
    return size;
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

    if (!MFileLocker::writeLockNB(file)) {
        mDebug("MStyleSheetParserPrivate") << "Failed to write lock stylesheet file:" << binaryFilename;
        return false;
    }

    mDebug("MStyleSheetParserPrivate::dump") << file.fileName();

    qint64 approximatedSize = approximateSize();
    QScopedArrayPointer<const char> buffer(new char[approximatedSize]);
    char *bufferIterator = const_cast<char*>(buffer.data());

    writeInteger<unsigned int>(FILE_VERSION, &bufferIterator);

    writeInteger<int>(fileInfoList.count(), &bufferIterator);
    QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::const_iterator fileInfoListEnd = fileInfoList.constEnd();
    for (QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::const_iterator fi = fileInfoList.constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        writeInteger(MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex((*fi)->filename), &bufferIterator);
        writeInteger<qint64>((*fi)->time_t, &bufferIterator);
    }

    int timestampCount = 0;
    if (logicalValues) {
        timestampCount = logicalValues->timestamps().count();
    }
    writeInteger<int>(timestampCount, &bufferIterator);
    for (int i = 0; i < timestampCount; ++i) {
        writeInteger<uint>(logicalValues->timestamps().at(i), &bufferIterator);
    }

    writeInteger<int>(fileInfoList.count(), &bufferIterator);
    for (QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::const_iterator fi = fileInfoList.constBegin();
            fi != fileInfoListEnd;
            ++fi) {
        writeStylesheetFileInfo(*fi, &bufferIterator);
    }

    qint64 bytesToWrite = bufferIterator - buffer.data();
    if (approximatedSize < bytesToWrite) {
        // if this did not crash anyway we need to abort here. normally this should never happen
        qFatal("Tried to write %lld bytes but only allocated memory for %lld bytes. approximateSize() must be fixed.", bytesToWrite, approximatedSize);
    }
    file.resize(bytesToWrite);
    qint64 writtenBytes = file.write(buffer.data(), bytesToWrite);
    if (bytesToWrite != writtenBytes) {
        mWarning("MStyleSheetParserPrivate::dump") << "Could only write" << writtenBytes << "of" << bytesToWrite << "bytes to" << file.fileName();
        qWarning() << file.errorString();
        file.remove();
    } else {
        file.close();
    }

    return true;
}

void MStyleSheetParserPrivate::writeStylesheetFileInfo(QSharedPointer<MStyleSheetParser::StylesheetFileInfo> fi, char** buffer)
{
    writeInteger<MUniqueStringCache::Index>(MStyleSheetParser::stringCacheWithoutReverseLookup()->stringToIndex(fi->filename), buffer);
    writeInteger<uint>(fi->time_t, buffer);

    writeInteger<int>(fi->selectors.count(), buffer);
    // write all selectors without parent
    QList<const MStyleSheetSelector *>::const_iterator infoSelectorsEnd = fi->selectors.constEnd();
    for (QList<const MStyleSheetSelector *>::const_iterator iterator = fi->selectors.constBegin();
            iterator != infoSelectorsEnd;
            ++iterator) {
        writeSelector(*iterator, buffer);
    }

    // write all selectors with parent
    writeInteger<int>(fi->parentSelectors.count(), buffer);
    infoSelectorsEnd = fi->parentSelectors.constEnd();
    for (QList<const MStyleSheetSelector *>::const_iterator iterator = fi->parentSelectors.constBegin();
            iterator != infoSelectorsEnd;
            ++iterator) {
        writeSelector(*iterator, buffer);
    }
}

QSharedPointer<MStyleSheetParser::StylesheetFileInfo> MStyleSheetParserPrivate::readStylesheetFileInfo(char** buffer)
{
    QSharedPointer<MStyleSheetParser::StylesheetFileInfo> fi(new MStyleSheetParser::StylesheetFileInfo);

    fi->fromMapedMemory = true;
    fi->filename = MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(readInteger<MUniqueStringCache::Index>(buffer));
    fi->time_t = readInteger<quint32>(buffer);

    int selectorCount;
    selectorCount = readInteger<int>(buffer);
    // read all selectors without parent
    for (int i = 0; i < selectorCount; ++i) {
        const MStyleSheetSelector *selector = readSelector(buffer);
        fi->selectors.push_back(selector);
    }

    // read all selectors with parent
    selectorCount = readInteger<int>(buffer);
    for (int i = 0; i < selectorCount; ++i) {
        const MStyleSheetSelector *selector = readSelector(buffer);
        fi->parentSelectors.push_back(selector);
    }

    return fi;
}

const MStyleSheetSelector *MStyleSheetParserPrivate::readSelector(char** buffer)
{
    fixAlignement(buffer, 4);
    MStyleSheetSelector *selector = reinterpret_cast<MStyleSheetSelector*>(*buffer);
    incrementBuffer(buffer, sizeof(MStyleSheetSelector));

    // we need to be aligned to 8 byte. this way the two integers and the qint64 are correctly aligned.
    // ATTENTION: this may need to be changed if MStyleSheetAttribute changes
    fixAlignement(buffer, 8);
    incrementBuffer(buffer, selector->attributeCount() * sizeof(MStyleSheetAttribute));

    return selector;
}

void MStyleSheetParserPrivate::writeSelector(const MStyleSheetSelector *selector, char** buffer) const
{
    fixAlignement(buffer, 4);
    size_t sizeOfSelector = sizeof(MStyleSheetSelector);
    const_cast<MStyleSheetSelector*>(selector)->fromMappedMemory = true;
    memcpy(*buffer, selector, sizeOfSelector);
    const_cast<MStyleSheetSelector*>(selector)->fromMappedMemory = false;
    incrementBuffer(buffer, sizeOfSelector);

    if (selector->attributeCount() == 0) {
        mWarning("MStyleSheetParserPrivate") << "Warning:"
                << "Style sheet selector has no attributes:"
                << "objectName:" << selector->objectName()
                << "className:" << selector->className() << "file:"
                << __FILE__ << "line:" << __LINE__ << "function:"
                << __PRETTY_FUNCTION__;
    }

    // we need to be aligned to 8 byte. this way the two integers and the qint64 are correctly aligned.
    // ATTENTION: this may need to be changed if MStyleSheetAttribute changes
    fixAlignement(buffer, 8);
    size_t sizeOfAttributes = selector->attributeCount() * sizeof(MStyleSheetAttribute);
    memcpy(*buffer, selector->attributeList(), sizeOfAttributes);
    incrementBuffer(buffer, sizeOfAttributes);
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

    d->fileInfoList.append(stylesheet.fileInfoList());
    d->mappedFiles.append(stylesheet.d_ptr->mappedFiles);
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

Q_GLOBAL_STATIC_WITH_ARGS(MUniqueStringCache, _stringCacheWithoutReverseLookup, (MSystemDirectories::cacheDirectory() + QLatin1String("/css/no_reverse_lookup_string_cache")))
MUniqueStringCache* MStyleSheetParser::stringCacheWithoutReverseLookup()
{
    return _stringCacheWithoutReverseLookup();
}

Q_GLOBAL_STATIC_WITH_ARGS(MUniqueStringCache, _stringCacheWithReverseLookup, (MSystemDirectories::cacheDirectory() + QLatin1String("/css/reverse_lookup_string_cache")))
MUniqueStringCache* MStyleSheetParser::stringCacheWithReverseLookup()
{
    return _stringCacheWithReverseLookup();
}
