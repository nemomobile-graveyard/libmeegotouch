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

#include "imagepage.h"
#include <MImageWidget>
#include <MLabel>
#include <MSceneManager>
#include <MContainer>
#include <MLocale>
#include <MApplication>
#include <MApplicationWindow>
#include <MComboBox>
#include <MSlider>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MDebug>
#include <QGraphicsGridLayout>
#include <QPinchGesture>
#include <QGestureEvent>

#include "utils.h"

ImagePage::ImagePage() :
    TemplatePage(),
    propertiesLabel(NULL),
    propertiesComboBox(NULL),
    visual(NULL),
    image(NULL),
    slider(NULL),
    originalScaleFactor(10)
{
    gid = TemplatePage::LayoutsAndVisuals;

    // Initial values for image property sliders
    sliderValues[ImageZoom] = 10;
    sliderValues[ImageTransparency] = 0;
    sliderValues[ImageCrop] = 0;

    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);
}

ImagePage::~ImagePage()
{
}

QString ImagePage::timedemoTitle()
{
    return "Image";
}

bool ImagePage::event(QEvent *e)
{
    if (e->type() == QEvent::TouchBegin) {
        e->setAccepted(true);
        return true;
    }
    return TemplatePage::event(e);
}

void ImagePage::createContent()
{
    MApplicationPage::createContent();

    createLayout();
    containerLayout = new MLayout(container);

    containerPolicy = new MLinearLayoutPolicy(containerLayout, Qt::Vertical);
    containerLayout->setPolicy(containerPolicy);

    // Image properties label
    propertiesLabel = new MLabel();

    // Image properties comboBox
    propertiesComboBox = new MComboBox();
    propertiesComboBox->setIconID("Icon-pictures");

    // Image property slider
    slider = new MSlider();
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);

    // Visual is a common parent for Image and Animation
    visual = new MWidget();

    QGraphicsGridLayout *gridLayout = new QGraphicsGridLayout();
    gridLayout->setContentsMargins(0, 0, 0, 0);
    visual->setLayout(gridLayout);

    QString imagesDir = Utils::imagesDir();

    // Image
    QString fname = imagesDir + '/' + QString("grape.jpg");
    QImage img(fname);
    image = new MImageWidget(&img);
    image->setVisible(true);
    gridLayout->addItem(image, 0, 0);

    QSize s = MApplication::activeApplicationWindow()->visibleSceneSize();

    containerPolicy->addItem(propertiesLabel);
    containerPolicy->addItem(propertiesComboBox);
    containerPolicy->addItem(visual);
    containerPolicy->addItem(slider);

    // Connect signals
    connect(propertiesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(operationChanged(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    retranslateUi();
}

void ImagePage::retranslateUi()
{
    //% "Image"
    setTitle(qtTrId("xx_Image"));
    if (!isContentCreated())
        return;
    //% "Image properties"
    propertiesLabel->setText(qtTrId("xx_Image properties"));
    int oldIndex = propertiesComboBox->currentIndex();
    propertiesComboBox->clear();

    QStringList itemList;
    //% "Zoom"
    itemList << qtTrId("xx_Zoom");
    //% "Transparency"
    itemList << qtTrId("xx_Transparency");
    //% "Crop"
    itemList << qtTrId("xx_Crop");
    propertiesComboBox->addItems(itemList);

    if (oldIndex == -1)
        propertiesComboBox->setCurrentIndex(0);
    else
        propertiesComboBox->setCurrentIndex(oldIndex);
    //% "Image operations"
    propertiesComboBox->setTitle(qtTrId("xx_image_operations"));

    //% "Image allows the placement of images onto the UI. "
    //% "Images are generally non-interactive elements. "
    //% "Various single and multiple touch interactions can be added to an "
    //% "Image component if desired. As an example, pinch gesture can be "
    //% "used in this page to zoom the image."
    infoLabel->setText("<a></a>" + qtTrId("xx_image_page_info"));

}

void ImagePage::operationChanged(int index)
{
    // Set slider to selected property
    slider->setValue(sliderValues[index]);
}

void ImagePage::sliderValueChanged(int value)
{
    // Store new value of the current property
    int index = propertiesComboBox->currentIndex();

    if (index >= ImageZoom && index < ImageLastItem)
        sliderValues[index] = value;
    else
        mWarning("ImagePage") << "combo box index out of range:" << index;

    // Set property value to image
    switch (index) {
    case 0: // Zoom: 0..100 to 0.0..10.0
        setImageZoom((float) value / 10);
        break;
    case 1: // Transparency: 0..100 to 1.0..0.0
        setImageOpacity(1 - ((float) value / 100.0));
        break;
    case 2: // Crop: 0..100 to 0..100% width and 0..100% height
        setImageCrop(
            image->size().width() - (image->size().width() / 100.0 * value),
            image->size().height() - (image->size().height() / 100.0 * value));
        break;
    default:
        break;
    }

}

void ImagePage::setImageZoom(float zoom)
{
    if (zoom != 0.0)
        image->setZoomFactor(zoom, zoom);

    // FIXME: why is this needed ?
    image->update();
}

void ImagePage::setImageOpacity(float opacity)
{
    image->setOpacity(opacity);
}

void ImagePage::setImageCrop(float width, float height)
{
    image->setCrop(QRectF(QPointF(0.0, 0.0), QSizeF(width, height)));

    // FIXME: why is this needed ?
    image->update();
}

void ImagePage::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
    if (gesture->state() == Qt::GestureStarted) {
	propertiesComboBox->setCurrentIndex(0);
        originalScaleFactor = slider->value();

        //If the current scale factor is 0, the we would not be scaling anything.
        if (originalScaleFactor == 0) originalScaleFactor = 1;
    }

    slider->setValue(originalScaleFactor * gesture->scaleFactor());

    event->accept(gesture);

}
