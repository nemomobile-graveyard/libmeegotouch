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

#ifndef FT_MMOC_H
#define FT_MMOC_H

#include <QObject>
#include <QtTest/QtTest>

class Ft_MMoc : public QObject
{
    Q_OBJECT
public:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void doMMoc_data();
    void doMMoc();

private:
    int runProcess( const QString& process, const QStringList &params );
    bool compareFiles(const QString &filename, const QString &correctFilename) const;

    QStringList m_filesToCompare;
    QString m_cwd;
    bool m_skipTests;

    bool m_qtMocFound;
    bool m_perlMmocFound;
    bool m_binaryMmocFound;

    QString m_binaryMmoc;
    QString m_perlMmoc;
};

#endif
