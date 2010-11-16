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

#ifndef MBUTTONICONVIEW_H
#define MBUTTONICONVIEW_H

#include "mbuttonview.h"
#include <mbuttonmodel.h>
#include <mbuttoniconstyle.h>

class MButtonIconViewPrivate;
class MButton;

/*!
    \class MButtonIconView
    \brief View class for icon button.

    \ingroup views

    \section MButtonIconViewOverview Overview
        MButtonIconView draws an icon and text similarly as MButtonView
        using the same styling attributes. However this view adds some animations
        when interacting with a button:

        - When pressing button down the content is drawn with style defined opacity.
        - When releasing button the icon and label opacity is reset.

 * MButtonIconView supports the following CSS features:
 *
 * content-opacity (type: qreal e.g. 0.75) : sets the opacity of the icon and text label<br>

    \section MButtonIconViewInteractions Interactions
        See \ref MButtonViewInteractions.

    \sa MButton MButtonView MButtonIconStyle
*/
class M_VIEWS_EXPORT MButtonIconView : public MButtonView
{
    Q_OBJECT
    M_VIEW(MButtonModel, MButtonIconStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    MButtonIconView(MButton *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MButtonIconView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MButtonIconView)
    Q_DECLARE_PRIVATE(MButtonIconView)

#ifdef UNIT_TEST
    friend class Ut_MButtonIconView;
#endif

};

#endif
