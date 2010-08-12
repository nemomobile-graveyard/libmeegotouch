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

#ifndef PT_MTHEME_H
#define PT_MTHEME_H

#include <QObject>

class MComponentData;
class MWidgetView;

/**
  * This test benchmarks the MTheme creation. Most of the time when creating
  * a MTheme instance is spend with loading stylesheets. Check pt_mstylesheet
  * for getting more details.
  */
class Pt_MTheme : public QObject
{
    Q_OBJECT

private slots:
    /**
     * Test the performance of the MTheme constructor.
     */
    void uncachedConstructor();

    /**
     * Execute the constructor a second time to evaluate caching possibilities.
     */
    void cachedConstructor();
};

#endif
