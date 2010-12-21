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

#ifndef MSCALABLEIMAGE_H
#define MSCALABLEIMAGE_H

#include <QObject>
#include <QMetaType>
#include "mexport.h"

class MScalableImagePrivate;
class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;
class QPainter;
class QPixmap;


/*!
  \class MScalableImage
  \brief MScalableImage is a class for drawing scaled pixmaps into screen
         without breaking the boundaries of the image.

  The image is constructed from 9 blocks and it is scalable in every direction.

            &lt;corner&gt; &lt;h_edge&gt; &lt;corner&gt;
            &lt;v_edge&gt; &lt;center&gt; &lt;v_edge&gt;
            &lt;corner&gt; &lt;h_edge&gt; &lt;corner&gt;

  The scalable area of the image is defined using the left, right, top and bottom
  border parameters. The borders are defined as pixels and they cannot be larger
  the used source pixmap.

  The corner blocks are not scaled at all. The horizontal edges (h_edge) are
  scaled only horizontally and the vertical edges (v_edge) only vertically. The
  center block is scaled vertically and horizontally if needed. The size inputted
  into one the draw() methods cannot be smaller than the defined borders.
*/
class M_CORE_EXPORT MScalableImage : public QObject
{
    Q_OBJECT

public:

    /*!
      \brief Default constructor. Initiates pixmap and borders to 0.
    */
    MScalableImage();

    /*!
      \brief Constructor. Initiates pixmap and borders to inputted values.
      \param pixmapId - optional pixmapId associated with a scalable image which will be included into warnings
        if MScalableImage happen to find those
    */
    MScalableImage(const QPixmap *pixmap, int left, int right, int top, int bottom, const QString &pixmapId = "");

    /*!
      \brief Destructor.
     */
    virtual ~MScalableImage();

    /*!
      \brief Get borders for the image.

      The borders are defined as pixels. See ::createScalable() for
      description about how the borders affect to the image drawing/scaling.
     */
    void borders(int *left, int *right, int *top, int *bottom) const;

    /*!
      \brief Set borders for the image.

      The borders are defined as pixels. See ::createScalable() for
      description about how the borders affect to the image drawing/scaling.
     */
    void setBorders(int left, int right, int top, int bottom);

    /*!
      \brief Set/change the drawn pixmap.

      The ownership of \a pixmap is not changed by this method, the caller
      of the method is still responsible for releasing the pixmap after
      done using it.
     */
    void setPixmap(const QPixmap *pixmap);

    //init pixmap from theme, and release it in the destructor or when
    //another pixmap is set.
    //void setPixmap(const QString& pixmapId);

    /*!
      \brief Returns the pixmap that was given as parameter into the ::createScalable()
             or ::setPixmap() methods.
     */
    const QPixmap *pixmap() const;

    /*!
      \brief Draws scalable image.

      The image is drawn starting from the topleft corner defined by the
      \a x and \a y coordinates. The image is scaled to fit the \a w and
      \a h. Scaling follows the rules defined in the ::createScalable() method.

      If -1 is defined as \a w or \a h appropriate value from the image is
      used so no scaling takes place.
     */
    void draw(int x, int y, int w, int h, QPainter *painter) const;

    /*!
      \brief Draws scalable image.
      \overload
     */
    void draw(const QPoint &pos, const QSize &size, QPainter *painter) const;

    /*!
      \brief Draws scalable image.
      \overload
     */
    void draw(const QRect &rect, QPainter *painter) const;

    /*!
      \brief Draws scalable image.
      \overload
     */
    void draw(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const;

    /*!
      \brief Draws scalable image.
      \overload
     */
    void draw(const QPointF &pos, const QSizeF &size, QPainter *painter) const;

    /*!
      \brief Draws scalable image.
      \overload
     */
    void draw(const QRectF &rect, QPainter *painter) const;

    /*!
      \brief Enable/disable optimized rendering.

      If \a enable is true, the image patches are packed into one vertex array
      and it is rendered with one glDrawArray call. If \a enable is false,
      the image patches are rendered separately.

      \note This method is only for benchmarking purposes and may be removed
            in the future.
      \deprecated            
     */
    void enableOptimizedRendering(bool enable);

    /*!
      \brief Draws the scalable image using color channel from pixmap and alpha channel from scalable image.
      \param rect           Target rectangle, where scalable image is drawn into.
      \param pixmapOffset   Start sampling offset of the pixmap to be filled.
      \param pixmap         Pixmap to be used for filling the opaque areas of the scalable image, this can be NULL.
      \param painter        Painter to be used for drawing.
     */
    void draw(const QRect &rect, const QPoint& pixmapOffset, const QPixmap* pixmap, QPainter *painter) const;

protected:
    MScalableImagePrivate *const d_ptr;
    MScalableImage(MScalableImagePrivate *dd);

private:
    Q_DECLARE_PRIVATE(MScalableImage)
    Q_DISABLE_COPY(MScalableImage)
};

Q_DECLARE_METATYPE(const MScalableImage *)

#endif

