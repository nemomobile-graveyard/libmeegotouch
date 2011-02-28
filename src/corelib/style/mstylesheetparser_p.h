#ifndef MSTYLESHEETPARSER_P_H
#define MSTYLESHEETPARSER_P_H

#include <mstylesheetselector.h>

#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QSystemSemaphore>
#include <QList>

class MLogicalValues;
class MStyleSheetSelectorTree;


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
    void readAllSelectors(char** buffer);
    void writeAllSelectors(char** buffer);
    void writeSelectorMap(const QMultiMap<MUniqueStringCache::Index, const MStyleSheetSelector *> &selectorMap, char **buffer);
    void writeSelectorsGroupedByObjectName(const QList<const MStyleSheetSelector *> &selectors, char **buffer);
    void writeSelectorList(const QList<const MStyleSheetSelector *> &selectors, char **buffer);

    void writeStylesheetFileInfo(QSharedPointer<MStyleSheetParser::StylesheetFileInfo> selector, char** buffer);
    QSharedPointer<MStyleSheetParser::StylesheetFileInfo> readStylesheetFileInfo(char** buffer);

    static const MStyleSheetSelector *readSelector(char** buffer);
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

    QList<const MStyleSheetSelectorTree*> selectorTrees;
    QList<const MStyleSheetSelectorTree*> parentSelectorTrees;
};

#endif // MSTYLESHEETPARSER_P_H
