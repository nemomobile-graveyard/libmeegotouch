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

#include "ft_mmoc.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTest>

void Ft_MMoc::initTestCase()
{
    m_skipTests = false;

    // check for qt moc
    m_qtMocFound = QFileInfo( "/usr/bin/moc" ).exists();

    // check for mmoc perl script
    m_perlMmoc = "/usr/bin/mmoc.pl";
    m_perlMmocFound = QFileInfo( m_perlMmoc ).exists();

    // check for mmoc binary
    m_binaryMmoc = "/usr/bin/mmoc";
    m_binaryMmocFound = QFileInfo( m_binaryMmoc ).exists();

    if ( ( ! m_qtMocFound )
         || ( !m_binaryMmocFound && !m_perlMmocFound )
       ) {
        QSKIP( "need development environment", SkipAll );
    }
}

void Ft_MMoc::cleanupTestCase()
{
}

int Ft_MMoc::runProcess( const QString& process, const QStringList &params )
{
    QProcess p;
    p.setProcessChannelMode( QProcess::ForwardedChannels );

    p.start( process, params );
    if ( !p.waitForStarted() )
    {
        qCritical( "process start failed" );
        return -1;
    }

    if ( !p.waitForFinished() )
    {
        qCritical( "process finish failed" );
        return -2;
    }

    return p.exitCode();
}

void Ft_MMoc::doMMoc_data()
{
    QTest::addColumn<QString>("mocPath");
    QTest::addColumn<QString>("fileName");

    // test all .h files in samples subdir.
    QStringList files( QDir(QCoreApplication::applicationDirPath() + "/ft_mmoc-samples" ).
                       entryList( QStringList("*.h") ) );
    foreach ( QString file, files )
    {
        if ( m_qtMocFound && m_binaryMmocFound )
        {
            QTest::newRow( qPrintable( file + " using " + m_binaryMmoc ) )
              << m_binaryMmoc << file;
        }
        if ( m_qtMocFound && m_perlMmocFound )
        {
            QTest::newRow( qPrintable( file + " using " + m_perlMmoc ) )
              << m_perlMmoc << file;
        }
    }
}

void Ft_MMoc::doMMoc()
{
  QFETCH( QString, mocPath );
  QFETCH( QString, fileName );

  QString path = QCoreApplication::applicationDirPath() + "/ft_mmoc-samples/";

  qWarning( "testing: moc: %s file: %s",
            qPrintable( mocPath ),
            qPrintable( fileName ) );

  QString baseName = fileName;
  baseName.remove( ".h" );

  int result = runProcess( mocPath, QStringList()
                           << path + fileName
                           << "-o" << "/tmp/moc_" + baseName + ".cpp" );

  // check for successful return
  QVERIFY( result == 0 );

  // now compare files
  QVERIFY( compareFiles( "/tmp/moc_" + baseName + ".cpp",
                         path + "moc_" + baseName + ".cpp.correct" ) );


}


bool Ft_MMoc::compareFiles(const QString &filename, const QString &correctFilename) const
{
    bool filesAreTheSame = true;

    QFile newFile(filename);
    if (!newFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "a Could not open" << filename;
        return false;
    }
    QTextStream newIn(&newFile);

    QFile correctFile(correctFilename);
    if (!correctFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "b Could not open" << correctFilename;
        return false;
    }
    QTextStream correctIn(&correctFile);

    do {
        if (newIn.atEnd() || correctIn.atEnd()) {
            bool oneFileFinishedBeforeOther = (!newIn.atEnd() || !correctIn.atEnd());
            if (oneFileFinishedBeforeOther) {
                qDebug( "one file finished before the other" );
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
            newLine.contains("** Created: ") ||
            newLine.contains("**      by: The Qt Meta Object Compiler version ") ||
            newLine.contains("#error \"This file was generated using the moc from") ||
            newLine.contains("#include ") ||
            newLine.contains("** Meta object code from reading C++ file ") ||
            newLine.contains("ft_mservicefwgen")
          ;
        if (lineIsExceptional) {
            continue;
        }

        bool linesAreIdentical = (newLine == correctLine);
        if ( ! linesAreIdentical )
        {
          qDebug() << "got these different lines: new:\n"
                   << filename
                   << "\n"
                   << newLine
                   << "\nexpected:\n"
                   << correctFilename
                   << "\n"
                   << correctLine;
        }
        filesAreTheSame = linesAreIdentical;
    } while (filesAreTheSame);

    correctFile.close();
    newFile.close();

    return filesAreTheSame;
}

QTEST_MAIN(Ft_MMoc)
