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

        - When pressing button down it starts to shrink the graphics so it
          looks like the button is actually pressed down.
          MButtonIconStyle::shrinkDuration and MButtonIconStyle::shrinkFactor
          attributes are used the define the outlook for the shrinking animation.
        - When releasing button it starts to glow and grow back to it's normal
          size. MButtonIconStyle::glowColor, MButtonIconStyle::glowDuration
          and MButtonIconStyle::glowRadius style attributes can be used to
          change the visualization of the glow.



 * MButtonIconView supports the following CSS features:
 *
 * glowColor (type: QColor e.g. #ff0000) : sets the color of the glow effect<br>
 * glowDuration (type: int) : sets the time in ms for the glow effect to last<br>
 * glowRadius (type: int) : sets the extent of the glow effect<br>
 * shrinkDuration (type: int) : time how long the shrink effect takes<br>
 * shrinkFactor (type: float) : how much smaller should the shrunk button be?<br>

    \section MButtonIconViewInteractions Interactions
        See \ref MButtonViewInteractions.

    \section MButtonIconViewOpenIssues Open issues
        - The outlook of the whole icon button: are glow and shrinking really
          used in this?

    \sa MButton MButtonView MButtonIconStyle
*/
class M_EXPORT MButtonIconView : public MButtonView
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
    //! \reimp_end

protected Q_SLOTS:

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private Q_SLOTS:

    /*!
        \brief Receives signals from scale timeline
     */
    void scaleValueChanged(qreal value);

    /*!
        \brief Receives signals from glow timeline
     */
    void glowValueChanged(qreal value);

private:
    Q_DISABLE_COPY(MButtonIconView)
    Q_DECLARE_PRIVATE(MButtonIconView)

#ifdef UNIT_TEST
    friend class Ut_MButtonIconView;
#endif

};

#endif
