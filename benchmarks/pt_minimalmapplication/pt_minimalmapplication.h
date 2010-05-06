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

#ifndef PT_MINIMALMAPPLICATION_H
#define PT_MINIMALMAPPLICATION_H

#include <QObject>

class MLocale;
class MApplicationWindow;
class MApplicationPage;

/**
  Benchmark runtime of a simple M application.
  All elements are benchmarked individually and the whole application is benchmarked at once.
 */
class Pt_minimalmapplication : public QObject
{
    Q_OBJECT
public:
    Pt_minimalmapplication();

    /**
      * Execute a whole simple M application by calling the slots from below.
      */
    int executeAll();

    QString appearType;
private slots:
    void overallRuntime();
    void overallRuntimeAppearNow();
    void createMLocale();
    void createMApplicationWindow();
    void windowShow();
    void createMApplicationPage();
    void pageAppear();

private:
    void executeSelf(const QString &param);
    bool noBenchmark;

    MLocale *locale;
    MApplicationWindow *window;
    MApplicationPage *page;
};

#endif // PT_MINIMALMAPPLICATION_H
