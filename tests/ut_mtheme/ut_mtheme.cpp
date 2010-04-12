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

#include <assert.h>

#include <mpixmaploader_stub.h>
#include <mstylesheet_stub.h>
#include <mstylesheetparser_stub.h>
#include <mviewfactory_stub.h>
#include <mstyle_stub.h>
#include <mbuttoniconview_stub.h>
#include <mwidgetview_stub.h>
#include <mbuttonview_stub.h>
#include <msliderdotview_stub.h>
#include <mnavigationbarview_stub.h>
#include <mtoolbarview_stub.h>
#include <mapplicationmenuview_stub.h>

#include <mtheme.h>

#include "ut_mtheme.h"
#include <X11/Xlib.h>

QImage::Format gQImageFormat = QImage::Format_Invalid;
XImage gImage;
QImage gQImage;
unsigned int gDepth;
int gBitsPerUnit;


Status XGetGeometry(
    Display*        /* display */,
    Drawable        /* drawable */,
    Window*     /* root_return */,
    int*        /* x_return */,
    int*        /* y_return */,
    unsigned int*   /* width_return */,
    unsigned int*   /* height_return */,
    unsigned int*   /* border_width_return */,
    unsigned int   *depth_return
)
{
    *depth_return = gDepth;
    qDebug() << "XGetGeometry() - called. Setting " << *depth_return << " as bit depth.";
    return 1;
}

XImage *XGetImage(
    Display*        /* display */,
    Drawable        /* d */,
    int         /* x */,
    int         /* y */,
    unsigned int    /* width */,
    unsigned int    /* height */,
    unsigned long   /* plane_mask */,
    int         /* format */
)
{
    qDebug() << "XGetImage() - called.";
    return &gImage;
}


int XBitmapUnit(Display * /*display*/)
{
    return gBitsPerUnit;
}


QImage::QImage(const uchar *data, int width, int height, Format format)
{
    Q_UNUSED(data);
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(format);

    qDebug() << "QImage::QImage() - called.";
    gQImageFormat = format;
}

QImage::~QImage()
{

}

QImage &QImage::operator=(const QImage &)
{
    qDebug() << "QImage::operator = () - called.";
    return gQImage;
}

/*
QPixmap::QPixmap(const QPixmap &)
{
    qDebug() << "QPixmap::QPixmap() - called.";
}
*/

void Ut_MTheme::init()
{
    m_subject = MTheme::instance(); //new MTheme();
    gQImageFormat = QImage::Format_Invalid;
    gDepth = 0;
    gBitsPerUnit = 0;
}

void Ut_MTheme::cleanupTestCase()
{
}

void Ut_MTheme::initTestCase()
{
}

void Ut_MTheme::loadCSS()
{
    // set MStyleSheetParser::load() to return true
    gMStyleSheetParserStub->stubSetReturnValue<bool>("load", true);

    QString myFilename("myCSSFile");
    m_subject->loadCSS(myFilename);

    QCOMPARE(gMStyleSheetParserStub->stubLastParameters<const QString &>(0), myFilename);
}

#if QT_VERSION < 0x040500
void Ut_MTheme::testPixmapCreationWithAllBitDepths()
{
    QPixmap *pixmap;

    // Test pixmap from handle with 8 bit pixmap
    gDepth = 8;
    pixmap = m_subject->pixmapFromHandle(8);
    QCOMPARE(gQImageFormat, QImage::Format_Indexed8);

    // Test pixmap from handle with 16 bit pixmap
    gDepth = 16;
    pixmap = m_subject->pixmapFromHandle(16);
    QCOMPARE(gQImageFormat, QImage::Format_RGB16);

    // Test pixmap from handle with 24 bit pixmap, no padding
    gDepth = 24;
    gBitsPerUnit = 24;
    pixmap = m_subject->pixmapFromHandle(24);
    QCOMPARE(gQImageFormat, QImage::Format_RGB888);

    // Test pixmap from handle with 24 bit pixmap, padding
    gDepth = 24;
    gBitsPerUnit = 32;
    pixmap = m_subject->pixmapFromHandle(24);
    QCOMPARE(gQImageFormat, QImage::Format_RGB32);
    gBitsPerUnit = 0; // reset

    // Test pixmap from handle with 32 bit pixmap
    gDepth = 32;
    pixmap = m_subject->pixmapFromHandle(32);
    QCOMPARE(gQImageFormat, QImage::Format_ARGB32);
}
#endif // QT_VERSION >= 0x040500

void Ut_MTheme::cleanup()
{
    // Don't delete, it's a singleton!
    //delete m_subject;
    //m_subject = 0;
}

QTEST_MAIN(Ut_MTheme)
