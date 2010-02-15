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

#ifndef PT_MINIMALDUIAPPLICATION_H
#define PT_MINIMALDUIAPPLICATION_H

#include <QObject>

class DuiLocale;
class DuiApplicationWindow;
class DuiApplicationPage;

/**
  Benchmark runtime of a simple DUI application.
  All elements are benchmarked individually and the whole application is benchmarked at once.
 */
class Pt_minimalduiapplication : public QObject
{
    Q_OBJECT
public:
    Pt_minimalduiapplication();

    /**
      * Execute a whole simple DUI application by calling the slots from below.
      */
    int executeAll();

    QString appearType;
private slots:
    void overallRuntime();
    void overallRuntimeAppearNow();
    void createDuiLocale();
    void createDuiApplicationWindow();
    void windowShow();
    void createDuiApplicationPage();
    void pageAppear();

private:
    void executeSelf(const QString &param);
    bool noBenchmark;

    DuiLocale *locale;
    DuiApplicationWindow *window;
    DuiApplicationPage *page;
};

#endif // PT_MINIMALDUIAPPLICATION_H
