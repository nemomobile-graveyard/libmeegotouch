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

#ifndef DUIIMAGEWIDGETVIEW_H
#define DUIIMAGEWIDGETVIEW_H

#include "duiexport.h"
#include "duiwidgetview.h"
#include "duiimagewidgetmodel.h"
#include "duiimagewidgetstyle.h"

class DuiImageWidgetViewPrivate;
class DuiImageWidget;

/*!
    \class DuiImageWidgetView
    \brief View class for standard dui images.

    \ingroup views

    DuiImageWidgetView is used to visualize image.

    \section DuiImageWidgetView Border
        DuiImageWidgetView draws a fixed width and color border surround the image.
        When image is cropped, the cropped edge will have no border.

        The border of image can be changed using the styling attributes defined
        in DuiImageWidgetStyle. DuiImageWidgetStyle::borderTop, DuiImageWidgetStyle::borderLeft,
        DuiImageWidgetStyle::borderRight, DuiImageWidgetStyle::borderBottom,
        DuiImageWidgetStyle::borderColor and DuiImageWidgetStyle::borderOpacity

 */

class DUI_EXPORT DuiImageWidgetView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiImageWidgetModel, DuiImageWidgetStyle)

public:
    /*!
       \brief Constructor
       \param controller Pointer to the DuiImageWidget
     */
    DuiImageWidgetView(DuiImageWidget *controller);

    /*!
       \brief Destructor
     */
    virtual ~DuiImageWidgetView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

    //! \cond
    DuiImageWidgetView(DuiImageWidgetViewPrivate &dd, DuiImageWidget *controller);
    //! \endcond

private:
    Q_DECLARE_PRIVATE(DuiImageWidgetView)
};

#endif
