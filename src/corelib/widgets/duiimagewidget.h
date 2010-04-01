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

#ifndef DUIIMAGEWIDGET_H
#define DUIIMAGEWIDGET_H

#include "duiwidgetcontroller.h"
#include "duiimagewidgetmodel.h"

class DuiImageWidgetPrivate;

/**
    \class DuiImageWidget
    \brief DuiImageWidget is a simple widget to show image.

    \ingroup widgets

    \section DuiImageWidgetOverview Overview
        Image allows the placement of images onto the UI. Images are generally non-interactive elements.
        Various single and multiple touch interactions can be added to an Image component if so wanted.

        Image supports scale, crop, zoom operations.

    \section DuiImageWidgetUseGuidelines Usage guidelines
        Images can be used by themselves to decorate the UI (background images, frames etc.)
        and also combined as part of other components (grids, lists, buttons etc.).

        If the Image has interactive elements, the touch area size of the interactive element needs to be specified.
        The area can occupy the entire Image, and/or it can also fall outside the Image:
        for instance in the case of an Image with a label, they are combined to form one interactive area.
        An Image can be on top or below of other images and other components (determined by the z-order).

        DuiImageWidget provides sizeHint(),
        default size will be the size which defined in CSS.
        If not defined in CSS, will be the size of image which added into DuiImage.

        DuiImageWidget can show border around the image, you can custom border in CSS.
        The border will work as a decoration and hint.
        Any cropped side will have no border.

    \section DuiImageWidgetExampleCodes Example codes

        DuiImageWidget can show image from theme:
        \code
        DuiImageWidget* image = new DuiImageWidget("icon");
        \endcode

        Also can get image with request size from theme:
        \code
        DuiImageWidget* image = new DuiImageWidget();
        image->setImage("icon", QSize(64,64));
        \endcode

        Or set image directly
        \code
        QImage img(path);
        DuiImageWidget* image = new DuiImageWidget(&img);
        \endcode

        \attention By default image will be shown full of the widget in Qt::KeepAspectRatio mode.
        You can change the mode use setAspectRatioMode(Qt::IgnoreAspectRatio)
        \code
        QImage img(path);
        DuiImageWidget* image = new DuiImageWidget(&img);
        image->setAspectRatioMode(Qt::IgnoreAspectRatio);
        \endcode

    \sa DuiImageWidgetModel DuiImageWidgetStyle

 */

class DUI_EXPORT DuiImageWidget : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiImageWidget)

    /**
       \property DuiImageWidget::crop
       \brief See DuiImageWidgetModel::crop
    */
    Q_PROPERTY(QRectF crop READ crop WRITE setCrop)

    /**
       \property DuiImage::aspectRatioMode
       \brief See DuiImageModel::aspectRatioMode
    */
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode)

    /**
       \property DuiImage::image
    */
    Q_PROPERTY(QString image READ image WRITE setImage)

public:

    /**
       \brief Constructs a image with a \a parent.
       \param parent Parent object.
     */
    DuiImageWidget(QGraphicsItem *parent = 0);

    /**
       \overload A convenient constructor for creating DuiImageWidget object with image id.
       \param image image id in theme system.
       \param parent Parent object.
     */
    explicit DuiImageWidget(const QString &imagename, QGraphicsItem *parent = 0);

    /**
       \overload A convenient constructor for creating DuiImageWidget object with QImage.
       \param image pointer to QImage which want to be shown
       \param parent Parent object.
     */
    explicit DuiImageWidget(const QImage *image, QGraphicsItem *parent = 0);

    /**
       \overload A convenient constructor for creating DuiImageWidget object with QPixmap.
       \param pixmap pointer to QPixmap which want to be shown
       \param parent Parent object.
     */
    explicit DuiImageWidget(const QPixmap *pixmap, QGraphicsItem *parent = 0);

    /**
       \brief Copy constructor
     */
    DuiImageWidget(const DuiImageWidget &);

    /**
       \brief Assigns other to this DuiImageWidget and returns a reference to this DuiImageWidget.
     */
    DuiImageWidget &operator=(const DuiImageWidget &);

    /**
       \brief Destroys the image.
     */
    virtual ~DuiImageWidget();

    /**
       \brief Set image id and size
       \param id, image id in theme system.
       \param s,  image size which will be used to get pixmap from theme system.
     */
    void setImage(const QString &id, const QSize &s);

    /**
       \brief Get the image id
     */
    QString image() const;

    /**
       \brief Get the origin size of the image.
     */
    QSize imageSize() const;

    /**
       \brief Get the pointer of pixmap.
     */
    const QPixmap *pixmap() const;

    /**
       \brief Set image two aspect ratio factors to one value
       \param factor image zoom factor
     */
    void setZoomFactor(qreal factor);

    /**
       \brief Set image width and height zoom factor
       \param fx image width zoom factor
       \param fy image height zoom factor
       \notice If fx not equals fy, will set zoom ratio mode to Qt::IgnoreAspectRatio automatic
     */
    void setZoomFactor(qreal fx, qreal fy);

    /**
       \brief This function get image zoom factor
       The parameters point to values stored in float. If any of the pointers is 0, that value will not be updated.
     */
    void zoomFactor(qreal *fx, qreal *fy) const;

    /**
       \brief Zoom in image.
     */
    void zoomIn();

    /**
       \brief Zoom out image.
     */
    void zoomOut();

    /**
       \brief Set image zoom AspectRatioMode
       \param mode defines what happens to the aspect ratio when zoom image.
     */
    void setAspectRatioMode(Qt::AspectRatioMode mode);

    /**
       \brief Get image aspect ratio mode.
     */
    Qt::AspectRatioMode aspectRatioMode() const;

    /**
       \brief Set image cropped to this recangle.
       \param size crop section rectangle
     */
    void setCrop(const QRectF &rect);

    /**
       \brief Get the crop rectangle.
     */
    QRectF crop() const;

public Q_SLOTS:

    /**
       \brief Set image id in theme
       \param id, image id in theme system.
     */
    void setImage(const QString &id);

    /**
       \brief Set a QImage into DuiImage
     */
    void setImage(const QImage &image);

    /**
       \brief Set a QPixmap into DuiImage
     */
    void setPixmap(const QPixmap &pixmap);

private:
    Q_DECLARE_PRIVATE(DuiImageWidget)

    friend class DuiImageWidgetView;
};

#endif
