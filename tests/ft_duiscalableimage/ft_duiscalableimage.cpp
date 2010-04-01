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

#include "ft_duiscalableimage.h"
#include "duiapplication.h"
#include <corelib/theme/duitheme.h>
#include <corelib/painting/duiscalableimage.h>
#include <QtTest>
#include <QPainter>

QVector<QPixmap *> pixmaps;

const int BLOCK_SIZE = 10;
QString COLOR_TOPLEFT("#00ff00");
QString COLOR_TOP("#ff0000");
QString COLOR_TOPRIGHT("#00ff00");
QString COLOR_LEFT("#ff0000");
QString COLOR_CENTER("#0000ff");
QString COLOR_RIGHT("#ff0000");
QString COLOR_BOTTOMLEFT("#00ff00");
QString COLOR_BOTTOM("#ff0000");
QString COLOR_BOTTOMRIGHT("#00ff00");

// Override duitheme pixmap, to provide our own test pixmaps
const QPixmap *createPixmap(const QString &id, const QSize &size)
{
    if (id == "single_image") {
        QSize image_size = size.isValid() ? size : QSize(512, 512);
        QPixmap *pixmap = new QPixmap(image_size);

        int center_width = image_size.width() - 2 * BLOCK_SIZE;
        int center_height = image_size.height() - 2 * BLOCK_SIZE;

        QPainter painter(pixmap);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
        painter.fillRect(0, 0, BLOCK_SIZE, BLOCK_SIZE, QColor(COLOR_TOPLEFT));
        painter.fillRect(BLOCK_SIZE, 0, center_width, BLOCK_SIZE, QColor(COLOR_TOP));
        painter.fillRect(BLOCK_SIZE + center_width, 0, BLOCK_SIZE, BLOCK_SIZE, QColor(COLOR_TOPRIGHT));
        painter.fillRect(0, BLOCK_SIZE, BLOCK_SIZE, center_height, QColor(COLOR_LEFT));
        painter.fillRect(BLOCK_SIZE, BLOCK_SIZE, center_width, center_height, QColor(COLOR_CENTER));
        painter.fillRect(BLOCK_SIZE + center_width, BLOCK_SIZE, BLOCK_SIZE, center_height, QColor(COLOR_RIGHT));
        painter.fillRect(0, BLOCK_SIZE + center_height, BLOCK_SIZE, BLOCK_SIZE, QColor(COLOR_BOTTOMLEFT));
        painter.fillRect(BLOCK_SIZE, BLOCK_SIZE + center_height, center_width, BLOCK_SIZE, QColor(COLOR_BOTTOM));
        painter.fillRect(BLOCK_SIZE + center_width, BLOCK_SIZE + center_height, BLOCK_SIZE, BLOCK_SIZE, QColor(COLOR_BOTTOMRIGHT));

        // debug
        //pixmap->save("test2.png");
        return pixmap;
    } else {
        QPixmap *pixmap = new QPixmap(size.isValid() ? size : QSize(BLOCK_SIZE, BLOCK_SIZE));
        pixmap->fill(QColor(id));
        pixmaps.push_back(pixmap);
        return pixmap;
    }
}

Ft_DuiScalableImage::Ft_DuiScalableImage()
{
}

void Ft_DuiScalableImage::init()
{
    m_subject = new DuiScalableImage();
}

void Ft_DuiScalableImage::cleanup()
{
    for (int i = 0; i < pixmaps.size(); i++) {
        delete pixmaps[i];
    }
    pixmaps.clear();

    delete m_subject;
}

DuiApplication *app;

void Ft_DuiScalableImage::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ft_DuiScalableImage" };
    app = new DuiApplication(argc, app_name);
}

void Ft_DuiScalableImage::cleanupTestCase()
{
    delete app;
}

bool isImageBlockColor(const QImage &image, const QRect &rect, const QString &colorid)
{
    QColor color(colorid);
    QImage copy = image.copy(rect);
    for (int y = 0; y < copy.height(); y++) {
        for (int x = 0; x < copy.width(); x++) {
            if (copy.pixel(x, y) != color.rgb()) {
                return false;
            }
        }
    }

    return true;
}

void Ft_DuiScalableImage::test_construction()
{
    // topleft, topright, bottomleft, bottomright, top, left, bottom, right, center
    const QPixmap *pm = createPixmap("single_image", QSize(-1, -1));

    m_subject->setPixmap(pm);
    m_subject->setBorders(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

    // adjust these to draw different size images to different location
    QSize image_size    = QSize(512, 512);

    QPoint paint_offset = QPoint(6, 6);
    QSize paint_size    = QSize(256, 256);

    QPixmap canvas(image_size);
    canvas.fill(Qt::black);

    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    // draw from image-offset to the edge of the canvas
    m_subject->draw(paint_offset, paint_size, &painter);
    // save, debug
    //canvas.save("/tmp/test.png");

    // check that the image looks proper,
    // clip only the part that should contain the drawn data
    QRect fillRegion(paint_offset, paint_size);
    QImage image = canvas.toImage().copy(fillRegion);

    // calculate the size for the center block
    int center_width = paint_size.width() - 2 * BLOCK_SIZE;
    int center_height = paint_size.height() - 2 * BLOCK_SIZE;

    // corners
    QCOMPARE(isImageBlockColor(image, QRect(0, 0, BLOCK_SIZE, BLOCK_SIZE), COLOR_TOPLEFT), true);
    QCOMPARE(isImageBlockColor(image, QRect(BLOCK_SIZE + center_width, 0, BLOCK_SIZE, BLOCK_SIZE), COLOR_TOPRIGHT), true);
    QCOMPARE(isImageBlockColor(image, QRect(0, BLOCK_SIZE + center_height, BLOCK_SIZE, BLOCK_SIZE), COLOR_BOTTOMLEFT), true);
    QCOMPARE(isImageBlockColor(image, QRect(BLOCK_SIZE + center_width, BLOCK_SIZE + center_height, BLOCK_SIZE, BLOCK_SIZE), COLOR_BOTTOMRIGHT), true);

    // edges
    QCOMPARE(isImageBlockColor(image, QRect(BLOCK_SIZE, 0, center_width, BLOCK_SIZE), COLOR_TOP), true);
    QCOMPARE(isImageBlockColor(image, QRect(0, BLOCK_SIZE, BLOCK_SIZE, center_height), COLOR_LEFT), true);
    QCOMPARE(isImageBlockColor(image, QRect(BLOCK_SIZE, BLOCK_SIZE + center_height, center_width, BLOCK_SIZE), COLOR_BOTTOM), true);
    QCOMPARE(isImageBlockColor(image, QRect(BLOCK_SIZE + center_width, BLOCK_SIZE, BLOCK_SIZE, center_height), COLOR_RIGHT), true);

    // center
    QCOMPARE(isImageBlockColor(image, QRect(BLOCK_SIZE, BLOCK_SIZE, center_width, center_height), COLOR_CENTER), true);
}

QTEST_APPLESS_MAIN(Ft_DuiScalableImage)
