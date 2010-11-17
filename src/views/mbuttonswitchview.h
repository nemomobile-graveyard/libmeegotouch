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

#ifndef MBUTTONSWITCHVIEW_H
#define MBUTTONSWITCHVIEW_H

#include <mbuttonview.h>
#include <mbuttonmodel.h>
#include <mbuttonswitchstyle.h>

class MButtonSwitchViewPrivate;
class MButton;

/*!
    \class MButtonSwitchView
    \brief View class for switch button.

    \ingroup views

    \section MButtonSwitchViewOverview Overview

        <TABLE border="0">
        <TR>
            <TD> \image html switch_button_off.png </TD>
            <TD> \image html switch_button_on.png </TD>
            <TD> \image html switch_button_drag.png </TD>
        </TR>
        </TABLE>

        This type of button acts as a visual "toggle switch" that can be turned
        on or off.

        A Switch differs from a push button and an icon button visually: the
        button looks like a switch, communicating that pressing this button
        will not go to another view or will not perform any other actions except
        to toggle the state of the button. In addition to pressing, Switch state
        can be toggled also by dragging.

        A Switch consists always of a graphic image representing a "physical switch",
        with an optional text label. If a label is needed, it should be placed
        next to the Switch button.

        A Switch button supports two states:
          - Function off: The function is off, and it can be turned on.
          - Function on: The function is on, and it can be turned off.

        The Switch has its own haptic feedback: switching button from off to on
        feels different than when switching from on to off.

    \section MButtonSwitchViewInteractions Interactions
        Users can change the state of switch button by tapping on it or dragging
        the handle/thumb to different position.

          - <b>Press down</b>: Highlights the switch button.
          - <b>Tap</b> (button down + button up): Toggles the state of
          the switch.
          - <b>Tap and drag</b>: Toggles the state of the switch.
          - <b>Tap down and drag finger out of the switch, release finger</b>:
          Cancels the press and does not change the state.
          - <b>Tap down and drag finger to scroll UI</b>: Cancels the press
          and does not change the state.


    \section MButtonSwitchViewOpenIssues Open issues

    \sa MButton MButtonView MButtonSwitchStyle
*/
class M_VIEWS_EXPORT MButtonSwitchView : public MButtonView
{
    Q_OBJECT
    M_VIEW(MButtonModel, MButtonSwitchStyle)

public:
    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    MButtonSwitchView(MButton *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MButtonSwitchView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void setupModel();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_PRIVATE_SLOT(d_func(), void _q_toggleCheck())

private:
    Q_DISABLE_COPY(MButtonSwitchView)
    Q_DECLARE_PRIVATE(MButtonSwitchView)
    
#ifdef UNIT_TEST
    friend class Ut_MButtonSwitchView;
#endif
};

#endif
