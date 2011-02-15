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

#ifndef MIMAGEWIDGETVIEW_H
#define MIMAGEWIDGETVIEW_H

#include "mexport.h"
#include "mwidgetview.h"
#include "mimagewidgetmodel.h"
#include "mimagewidgetstyle.h"

class MImageWidgetViewPrivate;
class MImageWidget;

/*!
    \class MImageWidgetView
    \brief View class for standard m images.

    \ingroup views

    MImageWidgetView is used to visualize image.

    \section MImageWidgetView Border
        MImageWidgetView draws a fixed width and color border surround the image.
        When image is cropped, the cropped edge will have no border.

        The border of image can be changed using the styling attributes defined
        in MImageWidgetStyle. MImageWidgetStyle::borderTop, MImageWidgetStyle::borderLeft,
        MImageWidgetStyle::borderRight, MImageWidgetStyle::borderBottom,
        MImageWidgetStyle::borderColor and MImageWidgetStyle::borderOpacity

 */

class M_VIEWS_EXPORT MImageWidgetView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MImageWidgetModel, MImageWidgetStyle)

public:
    /*!
       \brief Constructor
       \param controller Pointer to the MImageWidget
     */
    MImageWidgetView(MImageWidget *controller);

    /*!
       \brief Destructor
     */
    virtual ~MImageWidgetView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setGeometry(const QRectF &rect);
    virtual void applyStyle();
    virtual void updateData(const QList<const char *> &modifications);
    void updateGeometry();
    //! \reimp_end

    //! \internal
    MImageWidgetView(MImageWidgetViewPrivate &dd, MImageWidget *controller);
    //! \internal_end

private:
    QSize imageDataSize();

    Q_DECLARE_PRIVATE(MImageWidgetView)
};

#endif
