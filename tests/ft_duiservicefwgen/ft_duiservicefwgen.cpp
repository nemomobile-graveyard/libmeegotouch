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

#include "ft_duiservicefwgen.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTest>

namespace
{
    const QString scriptName("dui-servicefwgen");
    const QString qdbusxml2cpp("/usr/bin/qdbusxml2cpp");
};

void Ft_DuiServiceFwGen::runDuiServiceFwGen(const QStringList &params)
{
    QProcess duiServiceFwGen;
    duiServiceFwGen.setWorkingDirectory(cwd);

    QString devTreePath(cwd + "/../../tools/" + scriptName);
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
    duiServiceFwGen.start(fullPathToScript, params);
    if (!duiServiceFwGen.waitForStarted()) {
        qCritical() << "starting failed:" << duiServiceFwGen.error();
        skipTests = true;
    }
    if (!duiServiceFwGen.waitForFinished()) {
        qCritical() << "did not finish";
        skipTests = true;
    }
}

void Ft_DuiServiceFwGen::initTestCase()
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

    // had big trouble getting correct application path on arm
    // so don't delete this lightly
    QString argv0 = QCoreApplication::arguments().at(0);
    bool absolutePath = argv0.split("/").at(0).isEmpty();
    if (absolutePath) {
        cwd = QFileInfo(argv0).canonicalPath();
    } else {
        // relative path
        QString pwdEnv = QProcess::systemEnvironment().filter(QRegExp("^PWD=")).at(0).split("=").at(1);
        qDebug() << "pwdEnv=" << pwdEnv;
        qDebug() << "argv0=" << argv0;
        cwd = QFileInfo(pwdEnv + "/" + argv0).canonicalPath();
    }
    qDebug() << "cwd=" << cwd;

    // get list of xml files in the current working directory
    QStringList xmlFiles(QDir(cwd).entryList(QStringList("com.nokia.FtDuiServiceFwGen[0-9].xml")));

    if (xmlFiles.size() == 0) {
        qCritical() << "Could not find any xml files.";
    }

    // convert them to interface names
    QStringList interfaces;
    for (int i = 0; i < xmlFiles.size(); ++i) {
        QString interfaceName = QFileInfo(xmlFiles.at(i)).completeBaseName();;
        interfaces << cwd + "/" + interfaceName;
    }

    // generate proxy and adaptor files
    for (int i = 0; i < interfaces.size(); ++i) {
        QString thisInterface = interfaces.at(i);
        runDuiServiceFwGen(QStringList() << "-p" << thisInterface);
        runDuiServiceFwGen(QStringList() << "-a" << thisInterface);

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

void Ft_DuiServiceFwGen::cleanupTestCase()
{
}

void Ft_DuiServiceFwGen::compareFiles_data()
{
    QTest::addColumn<QString>("filename");

    for (int i = 0; i < filesToCompare.size(); ++i) {
        QString thisFilename = filesToCompare.at(i);
        QTest::newRow(qPrintable(thisFilename)) << thisFilename;
    }
}

void Ft_DuiServiceFwGen::compareFiles()
{
    if (skipTests) {
        QSKIP("Something wrong with the environment - skipping", SkipAll);
    } else {
        QFETCH(QString, filename);
        QString correctFilename(filename + ".correct");

        if (!filesAreTheSame(filename, correctFilename)) {
            QStringList errorMessage;
            errorMessage
                    << "These files differ :"
                    << filename
                    << correctFilename;
            QWARN(qPrintable(errorMessage.join(" ")));
            QFAIL("Verify new file contents against correct file and if they are correct, overwrite the correct file with the new one.");
        } else {
            QString thisFilename(filename);
            QFile thisFile(thisFilename);
            if (!thisFile.remove()) {
                qWarning() << "Could not remove" << thisFilename;
            }
        }
    }
}

bool Ft_DuiServiceFwGen::filesAreTheSame(const QString &filename, const QString &correctFilename) const
{
    bool filesAreTheSame = true;

    QFile newFile(filename);
    if (!newFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open" << filename;
        return false;
    }
    QTextStream newIn(&newFile);

    QFile correctFile(correctFilename);
    if (!correctFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open" << filename;
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
            newLine.contains("qdbusxml2cpp") ||
            newLine.contains("ft_duiservicefwgen") ||
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


QTEST_MAIN(Ft_DuiServiceFwGen)
