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

#ifndef PT_WIDGETSGALLERY_H
#define PT_WIDGETSGALLERY_H

#include <QObject>

class Pt_WidgetsGallery : public QObject
{
    Q_OBJECT
private slots:

    /**
      * Benchmark startup time of widgetsgallery
      */
    void startupTime();

    /**
      * Benchmark startup time of widgetsgallery with enforced windows style.
      * Allows us to see if the Maemo6 style slows down application startup.
      */
    void startupTimeWindowsStyle();

private:
    void executeWidgetsgallery(const QStringList& arguments);
};

#endif // PT_WIDGETSGALLERY_H
