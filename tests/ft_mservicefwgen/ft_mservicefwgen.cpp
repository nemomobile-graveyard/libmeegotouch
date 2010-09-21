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

#include "ft_mservicefwgen.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTest>

namespace
{
    const QString scriptName("m-servicefwgen");
    const QString qdbusxml2cpp("/usr/bin/qdbusxml2cpp");
};

void Ft_MServiceFwGen::runMServiceFwGen(const QStringList &params)
{
    QProcess mServiceFwGen;
    mServiceFwGen.setWorkingDirectory(cwd);

    QString devTreePath(QDir::currentPath() + "/../../tools/" + scriptName);
    QString fullPathToScript(QFileInfo(devTreePath).canonicalFilePath());

    if (!QFileInfo(fullPathToScript).exists()) {
        qDebug() << devTreePath << "does not exist; trying to use installed one";
        fullPathToScript = "/usr/bin/" + scriptName;
        if (!QFileInfo(fullPathToScript).exists()) {
            qDebug() << fullPathToScript << "does not exist";
            skipTests = true;
        }
    }

    qDebug() << "running " << fullPathToScript + " " + params.join(" ");
    mServiceFwGen.start(fullPathToScript, params);
    if (!mServiceFwGen.waitForStarted()) {
        qCritical() << "starting failed:" << mServiceFwGen.error();
        skipTests = true;
    }
    if (!mServiceFwGen.waitForFinished()) {
        qCritical() << "did not finish";
        skipTests = true;
    }
}

void Ft_MServiceFwGen::initTestCase()
{
    skipTests = false;
    bool qdbusxml2cppNotExist = !QFileInfo(qdbusxml2cpp).exists();
    bool scriptNotExist = !QFileInfo("/usr/bin/" + scriptName).exists();

    if (qdbusxml2cppNotExist) {
        qCritical() << qdbusxml2cpp << "is missing.";
        skipTests = true;
        return;
    }

    if (scriptNotExist) {
        qCritical() << "/usr/bin/" + scriptName << "is missing.";
        skipTests = true;
        return;
    }

    // make temporary working directory
    cwd = QDir::tempPath()+"/ft_mservicefwgen";
    if ( !QFileInfo( cwd ).exists() && !QDir().mkdir( cwd ) ) {
        qDebug() << "Could not mkdir" << cwd;
    }
    qDebug() << "cwd=" << cwd;


    // get list of xml files in the QResource filessystem
    QStringList xmlFiles(QDir(":/").entryList(QStringList("com.nokia.FtMServiceFwGen[0-9].xml")));

    if (xmlFiles.size() == 0) {
        qCritical() << "Could not find any xml files.";
    }

    // convert the xml files to interface names
    // and copy the xml files from the QResource filesystem to the temporary directory
    // so that m-servicefwgen can process them
    QStringList interfaces;
    for (int i = 0; i < xmlFiles.size(); ++i) {
        QStringList files;
        files << ":/"+xmlFiles.at(i) << cwd+"/"+QFileInfo(xmlFiles.at(i)).fileName();
        QFile fromFile( files.at(0) );

        if ( !fromFile.copy( files.at(1) ) ) {
            qDebug() << "copy failed" << files;
            qDebug() << "error" << fromFile.error();
        }

        QString interfaceName = QFileInfo(xmlFiles.at(i)).completeBaseName();
        interfaces << interfaceName;
    }

    // generate proxy and adaptor files
    for (int i = 0; i < interfaces.size(); ++i) {
        QString thisInterface = interfaces.at(i);
        runMServiceFwGen(QStringList() << "-p" << thisInterface);
        runMServiceFwGen(QStringList() << "-a" << thisInterface);

        QString lowerBase = thisInterface.split(".").last().toLower();

        QString adaptorCpp = cwd + "/" + lowerBase + "adaptor.cpp";
        QString adaptorH   = cwd + "/" + lowerBase + "adaptor.h";
        QString proxyCpp   = cwd + "/" + lowerBase + "proxy.cpp";
        QString proxyH     = cwd + "/" + lowerBase + "proxy.h";
        QString wrapperCpp = cwd + "/" + lowerBase + ".cpp";
        QString wrapperH   = cwd + "/" + lowerBase + ".h";

        filesToCompare
                << adaptorCpp
                << adaptorH
                << proxyCpp
                << proxyH
                << wrapperCpp
                << wrapperH;
    }
}

void Ft_MServiceFwGen::cleanupTestCase()
{
    if (!skipTests) {
        QDir tempDir( cwd );

        // remove all temporary files
        QStringList tempFilenames(tempDir.entryList(QStringList() << "*.xml", QDir::Files));
        foreach( QString tempFilename, tempFilenames ) {
            tempDir.remove( tempFilename );
            qDebug() << "removing" << tempFilename;
        }

        // remove temporary directory
        if ( QDir( cwd ).exists() ) {
            qDebug() << "removing" << cwd;
            if ( !QDir( cwd ).rmdir( cwd ) ) {
                qDebug() << "failed to remove" << cwd;
            }
        }
    }
}

void Ft_MServiceFwGen::compareFiles_data()
{
    QTest::addColumn<QString>("filename");

    for (int i = 0; i < filesToCompare.size(); ++i) {
        QString thisFilename = filesToCompare.at(i);
        QTest::newRow(qPrintable(thisFilename)) << thisFilename;
    }
}

void Ft_MServiceFwGen::compareFiles()
{
    if (skipTests) {
        QSKIP("Something wrong with the environment - skipping", SkipAll);
    } else {
        QFETCH(QString, filename);
        QString correctFilename(filename + ".correct");

        QString resourceFilename( ":/"+QFileInfo(correctFilename).fileName() );
        if (!filesAreTheSame(filename, resourceFilename)) {

            QFile(resourceFilename).copy(correctFilename);

            QStringList errorMessage;
            errorMessage
                    << "These files differ :"
                    << filename
                    << correctFilename;
            QFAIL(qPrintable(errorMessage.join(" ")));
        } else {
            QString thisFilename(filename);
            QFile thisFile(thisFilename);
            if (!thisFile.remove()) {
                qWarning() << "Could not remove" << thisFilename;
            }
        }
    }
}

bool Ft_MServiceFwGen::filesAreTheSame(const QString &filename, const QString &correctFilename) const
{
    bool filesAreTheSame = true;

    QFile newFile(filename);
    if (!newFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open" << newFile.fileName();
        return false;
    }
    QTextStream newIn(&newFile);

    QFile correctFile( correctFilename );
    if (!correctFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open" << correctFile.fileName();
        return false;
    }
    QTextStream correctIn(&correctFile);

    do {
        if (newIn.atEnd() || correctIn.atEnd()) {
            bool oneFileFinishedBeforeOther = (!newIn.atEnd() || !correctIn.atEnd());
            if (oneFileFinishedBeforeOther) {
                filesAreTheSame = false;
            }
            break;
        }

        QString newLine     = newIn.readLine();
        QString correctLine = correctIn.readLine();

        // skip exceptional lines
        QString headerGuard(QFileInfo(filename).fileName().toUpper().replace(".", "_"));
        bool lineIsExceptional =
            newLine.contains(headerGuard) ||
            newLine.contains("m-servicefwgen") ||
            newLine.contains("automatically generated with the command line") ||
            newLine.contains("qdbusxml2cpp") ||
            newLine.contains("ft_mservicefwgen") ||
            newLine.contains(scriptName);
        if (lineIsExceptional) {
            continue;
        }

        bool linesAreIdentical = (newLine == correctLine);

        filesAreTheSame = linesAreIdentical;
    } while (filesAreTheSame);

    correctFile.close();
    newFile.close();

    return filesAreTheSame;
}


QTEST_MAIN(Ft_MServiceFwGen)
