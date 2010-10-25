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

#ifndef UT_MTHEME_H
#define UT_MTHEME_H

#include <QObject>

class MApplication;
class MTheme;
class QSize;

class Ut_MTheme : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStyle();
    void testThemeChangeCompleted();

    void testPixmap();
    void testPixmapWithSize();
    void testUnknownPixmap();

    void testPixmapCopy();
    void testPixmapCopyWithSize();
    void testUnknownPixmapCopy();

    void testPixmapCaching();

    void testApplicationPixmapDirs();

    void testScalableImage();
    void testView();

private:
    /**
     * @return True, if the given icon has been cached in the theme daemon.
     *
     * @param id   Identifier of the icon that has been requested with
     *             MTheme::pixmap() or pixmapCopy()
     * @param size Size that has been requested (not the actual size of the pixmap!).
     */
    bool isIconCached(const QString &id, const QSize &size) const;

    int cachedIconCount() const;

    /**
     * Waits until the signal pixmapRequestsFinished() has been emitted.
     */
    void waitForPendingThemeRequests();

    MTheme *m_theme;
    MApplication *m_app;
};
#endif
