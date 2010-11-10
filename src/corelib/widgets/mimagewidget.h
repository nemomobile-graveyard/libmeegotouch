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

#ifndef MIMAGEWIDGET_H
#define MIMAGEWIDGET_H

#include "mwidgetcontroller.h"
#include "mimagewidgetmodel.h"

class MImageWidgetPrivate;

/**
    \class MImageWidget
    \brief MImageWidget is a simple widget to show image.

    \ingroup widgets

    \section MImageWidgetOverview Overview
        Image allows the placement of images onto the UI. Images are generally non-interactive elements.
        Various single and multiple touch interactions can be added to an Image component if so wanted.

        Image supports scale, crop, zoom operations.

    \section MImageWidgetUseGuidelines Usage guidelines
        NOTE: The images passed to MImageWidget are converted internally to OpenGL textures, thus their
        maximum size is limited by the GPU driver. Consult the documentation of the GPU unit for the actual
        maximum texture size. If the image exceeds the maximum supported size, it shall not be
        drawn.

        Images can be used by themselves to decorate the UI (background images, frames etc.)
        and also combined as part of other components (grids, lists, buttons etc.).

        If the Image has interactive elements, the touch area size of the interactive element needs to be specified.
        The area can occupy the entire Image, and/or it can also fall outside the Image:
        for instance in the case of an Image with a label, they are combined to form one interactive area.
        An Image can be on top or below of other images and other components (determined by the z-order).

        MImageWidget provides sizeHint(),
        default size will be the size which defined in CSS.
        If not defined in CSS, will be the size of image which added into MImage.

        MImageWidget can show border around the image, you can custom border in CSS.
        The border will work as a decoration and hint.
        Any cropped side will have no border.

    \section MImageWidgetExampleCodes Example codes

        MImageWidget can show image from theme:
        \code
        MImageWidget* image = new MImageWidget("icon");
        \endcode

        Also can get image with request size from theme:
        \code
        MImageWidget* image = new MImageWidget();
        image->setImage("icon", QSize(64,64));
        \endcode

        Or set image directly
        \code
        QImage img(path);
        MImageWidget* image = new MImageWidget(&img);
        \endcode

        \attention By default image will be shown full of the widget in Qt::KeepAspectRatio mode.
        You can change the mode use setAspectRatioMode(Qt::IgnoreAspectRatio)
        \code
        QImage img(path);
        MImageWidget* image = new MImageWidget(&img);
        image->setAspectRatioMode(Qt::IgnoreAspectRatio);
        \endcode

    \sa MImageWidgetModel MImageWidgetStyle

 */

class M_CORE_EXPORT MImageWidget : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MImageWidget)

    /**
       \property MImageWidget::crop
       \brief See MImageWidgetModel::crop
    */
    Q_PROPERTY(QRectF crop READ crop WRITE setCrop)

    /**
       \property MImage::aspectRatioMode
       \brief See MImageModel::aspectRatioMode
    */
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode)

    /**
       \property MImage::image
    */
    Q_PROPERTY(QString image READ image WRITE setImage)

public:

    /**
       \brief Constructs a image with a \a parent.
       \param parent Parent object.
     */
    MImageWidget(QGraphicsItem *parent = 0);

    /**
       \overload A convenient constructor for creating MImageWidget object with image id.
       \param id image id in theme system.
       \param parent Parent object.
     */
    explicit MImageWidget(const QString &id, QGraphicsItem *parent = 0);

    /**
       \overload A convenient constructor for creating MImageWidget object with QImage.
       \param image pointer to QImage which want to be shown
       \param parent Parent object.
     */
    explicit MImageWidget(const QImage *image, QGraphicsItem *parent = 0);

    /**
       \overload A convenient constructor for creating MImageWidget object with QPixmap.
       \param pixmap pointer to QPixmap which want to be shown
       \param parent Parent object.
     */
    explicit MImageWidget(const QPixmap *pixmap, QGraphicsItem *parent = 0);

    /**
       \brief Copy constructor
     */
    MImageWidget(const MImageWidget &);

    /**
       \brief Assigns other to this MImageWidget and returns a reference to this MImageWidget.
     */
    MImageWidget &operator=(const MImageWidget &);

    /**
       \brief Destroys the image.
     */
    virtual ~MImageWidget();

    /*!
       Sets the \ref logicalid "logical ID" associated with this image to \a id.

      The graphics specified by the ID will be exactly of the specified \a size, and will
      stretch to fill the image widget.
     */
    void setImage(const QString &id, const QSize &size);

    /**
       \brief Get the image id.
       \deprecated Use the MImageWidget::imageId();
     */
    QString image() const;

    /**
       \brief Get the image id
    */
    QString imageId() const;

    /**
       \brief Get the origin size of the image.
     */
    QSize imageSize() const;

    /**
       \brief Get the pointer of pixmap.
       \deprecated
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
       \deprecated There is no replacing functionality
     */
    void zoomFactor(qreal *fx, qreal *fy) const;

    /**
       \brief Zoom in image.
       \deprecated There is no replacing functionality
     */
    void zoomIn();

    /**
       \brief Zoom out image.
       \deprecated There is no replacing functionality
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
       \deprecated There is no replacing functionality
     */
    void setCrop(const QRectF &rect);

    /**
       \brief Get the crop rectangle.
       \deprecated There is no replacing functionality
     */
    QRectF crop() const;

public Q_SLOTS:

    /*!
      Sets the \ref logicalid "logical ID" associated with this image to \a id.

      Whatever size the graphics happens to be in the theme will be used. The
      graphics is then scaled to fit the image widget. Unless you know that the
      themed graphic is exactly of the right size, you must use the
      setImage(const QString &id, const QSize &size) method instead.
     */
    void setImage(const QString &id);

    /**
       \brief Set a QImage into MImage
     */
    void setImage(const QImage &image);

    /**
       \brief Set a QPixmap into MImage
     */
    void setPixmap(const QPixmap &pixmap);

private:
    Q_DECLARE_PRIVATE(MImageWidget)

    friend class MImageWidgetView;
};

#endif
