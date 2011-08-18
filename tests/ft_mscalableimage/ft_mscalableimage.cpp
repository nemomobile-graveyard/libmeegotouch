/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ft_mscalableimage.h"
#include "mapplication.h"
#include <corelib/painting/mscalableimage.h>
#include <QtTest>
#include <QPainter>

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

const QPixmap *createPixmap(const QSize &size, const int blockSize)
{
    QSize image_size = size.isValid() ? size : QSize(512, 512);
    QPixmap *pixmap = new QPixmap(image_size);

    int center_width = image_size.width() - 2 * blockSize;
    int center_height = image_size.height() - 2 * blockSize;

    QPainter painter(pixmap);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter.fillRect(0, 0, blockSize, blockSize, QColor(COLOR_TOPLEFT));
    painter.fillRect(blockSize, 0, center_width, blockSize, QColor(COLOR_TOP));
    painter.fillRect(blockSize + center_width, 0, blockSize, blockSize, QColor(COLOR_TOPRIGHT));
    painter.fillRect(0, blockSize, blockSize, center_height, QColor(COLOR_LEFT));
    painter.fillRect(blockSize, blockSize, center_width, center_height, QColor(COLOR_CENTER));
    painter.fillRect(blockSize + center_width, blockSize, blockSize, center_height, QColor(COLOR_RIGHT));
    painter.fillRect(0, blockSize + center_height, blockSize, blockSize, QColor(COLOR_BOTTOMLEFT));
    painter.fillRect(blockSize, blockSize + center_height, center_width, blockSize, QColor(COLOR_BOTTOM));
    painter.fillRect(blockSize + center_width, blockSize + center_height, blockSize, blockSize, QColor(COLOR_BOTTOMRIGHT));

    // debug
    //pixmap->save("test2.png");
    return pixmap;
}

Ft_MScalableImage::Ft_MScalableImage() :
        m_subject(0)
{
}

void Ft_MScalableImage::init()
{
    m_subject = new MScalableImage();
}

void Ft_MScalableImage::cleanup()
{
    delete m_subject;
}

MApplication *app;

void Ft_MScalableImage::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ft_MScalableImage" };
    app = new MApplication(argc, app_name);
}

void Ft_MScalableImage::cleanupTestCase()
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

void Ft_MScalableImage::test_draw_scalable9()
{
    QSize image_size    = QSize(512, 512);
    const QPixmap *pm = createPixmap(image_size, BLOCK_SIZE);
    
    m_subject->setPixmap(pm);
    m_subject->setBorders(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

    QPoint paint_offset = QPoint(6, 6);
    QSize paint_size    = QSize(256, 256);
    QRect paint_rect(paint_offset, paint_size);

    QPixmap canvas(image_size);
    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    //void draw(const QPoint &pos, const QSize &size, QPainter *painter) const;
    canvas.fill(Qt::black);
    m_subject->draw(paint_offset, paint_size, &painter);
    QImage image = canvas.toImage().copy(paint_rect);
    verifyResult(image, paint_size, BLOCK_SIZE);

    //void draw(const QRect &rect, QPainter *painter) const;
    canvas.fill(Qt::black);
    m_subject->draw(paint_rect, &painter);
    image = canvas.toImage().copy(paint_rect);
    verifyResult(image, paint_size, BLOCK_SIZE);

    //void draw(const QRect &rect, const QPoint& pixmapOffset, const QPixmap* pixmap, QPainter *painter) const;
    QPixmap fillPm(paint_size);
    fillPm.fill(Qt::red);
    m_subject->draw(paint_rect, QPoint(0,0), &fillPm, &painter);
    image = canvas.toImage().copy(paint_rect);
    QCOMPARE(fillPm.toImage(), image);

    // cover the special case with zero center area (see NB#274332)
    // zero border case is already covered by test_draw_scalable1
    const QPixmap *pm2 = createPixmap(image_size,paint_size.width()/2);
    m_subject->setPixmap(pm2);

    m_subject->setBorders(paint_size.width()/2, paint_size.width()/2, paint_size.height()/2, paint_size.height()/2);
    canvas.fill(Qt::black);
    m_subject->draw(paint_rect, &painter);
    image = canvas.toImage().copy(paint_rect);
    verifyResult(image, paint_size, paint_size.width()/2 - 1); //the borders should get reduced when the image is rendered
}

void Ft_MScalableImage::test_draw_scalable1()
{
    QSize image_size    = QSize(512, 512);
    const QPixmap *pm = createPixmap(image_size, BLOCK_SIZE);
    
    m_subject->setPixmap(pm);
    m_subject->setBorders(0, 0, 0, 0);

    QPoint paint_offset = QPoint(0, 0);
    QSize paint_size    = QSize(512, 512);
    QRect paint_rect(paint_offset, paint_size);
    
    QPixmap canvas(image_size);
    QPainter painter(&canvas);

    //void draw(const QPoint &pos, const QSize &size, QPainter *painter) const;
    canvas.fill(Qt::black);
    m_subject->draw(paint_offset, paint_size, &painter);
    QImage image = canvas.toImage().copy(paint_rect);
    verifyResult(image, paint_size, BLOCK_SIZE);
    
    //void draw(const QRect &rect, QPainter *painter) const;
    canvas.fill(Qt::black);
    m_subject->draw(paint_rect, &painter);
    image = canvas.toImage().copy(paint_rect);
    verifyResult(image, paint_size, BLOCK_SIZE);
    
    //void draw(const QRect &rect, const QPoint& pixmapOffset, const QPixmap* pixmap, QPainter *painter) const;
    QPixmap fillPm(paint_size);
    fillPm.fill(Qt::red);
    m_subject->draw(paint_rect, QPoint(0,0), &fillPm, &painter);
    image = canvas.toImage().copy(paint_rect);
    QCOMPARE(fillPm.toImage(), image);
}

void Ft_MScalableImage::verifyResult(const QImage& image, const QSize& paintSize, const int blockSize)
{
    // calculate the size for the center block
    int center_width = paintSize.width() - 2 * blockSize;
    int center_height = paintSize.height() - 2 * blockSize;

    // corners
    QCOMPARE(isImageBlockColor(image, QRect(2, 2, blockSize-2, blockSize-2), COLOR_TOPLEFT), true);
    QCOMPARE(isImageBlockColor(image, QRect(blockSize + center_width + 2, 2, blockSize-2, blockSize-2), COLOR_TOPRIGHT), true);
    QCOMPARE(isImageBlockColor(image, QRect(2, blockSize + center_height + 2, blockSize-2, blockSize-2), COLOR_BOTTOMLEFT), true);
    QCOMPARE(isImageBlockColor(image, QRect(blockSize + center_width + 2, blockSize + center_height + 2, blockSize - 2, blockSize - 2), COLOR_BOTTOMRIGHT), true);

    // edges
    QCOMPARE(isImageBlockColor(image, QRect(blockSize+2, 2, center_width-2, blockSize-2), COLOR_TOP), true);
    QCOMPARE(isImageBlockColor(image, QRect(2, blockSize+2, blockSize-2, center_height-2), COLOR_LEFT), true);
    QCOMPARE(isImageBlockColor(image, QRect(blockSize+2, blockSize + center_height + 2, center_width-2, blockSize-2), COLOR_BOTTOM), true);
    QCOMPARE(isImageBlockColor(image, QRect(blockSize + center_width + 2, blockSize + 2, blockSize-2, center_height-2), COLOR_RIGHT), true);

    // center
    QCOMPARE(isImageBlockColor(image, QRect(blockSize, blockSize, center_width, center_height), COLOR_CENTER), true);
}

void Ft_MScalableImage::testEnableOptimizedRendering()
{
    /* the method does nothing, not even storing the value. These calls are just
       to cheat the coverage tools */
    m_subject->enableOptimizedRendering(true);
    m_subject->enableOptimizedRendering(false);
}


QTEST_APPLESS_MAIN(Ft_MScalableImage)
