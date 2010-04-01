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

#ifndef DUICHECKBOXVIEW_H
#define DUICHECKBOXVIEW_H

#include <duibuttonview.h>
#include <duibuttonmodel.h>
#include <duicheckboxstyle.h>

class DuiCheckboxViewPrivate;

/*!
    \class DuiCheckboxView
    \brief View class for a checkbox button.

    \ingroup views

    \section DuiCheckboxViewOverview Overview

        <TABLE border="0">
        <TR>
            <TD> \image html checkbox_off.png </TD>
            <TD> \image html checkbox_on.png </TD>
            <TD> \image html checkbox_on_down.png </TD>
        </TR>
        </TABLE>

        Checkbox is a variant of button that allows users to set a state of a
        variable or setting which has two values, On and Off. Checkbox essentially
        does the same thing as the Switch variant.

    \section DuiCheckboxViewInteractions Interactions
        Users can change the state of checkbox by tapping on it (or the accompanying
        text label). If the state of the checkbox is On, it is changed to Off.
        If the state of the checkbox is On, it is changed to Off. If the checkbox
        is disabled the tapping has not effect.

          - <b>Press down</b>: Highlights the checkbox.
          - <b>Tap</b> (button down + button up): Toggles the state of
          the checkbox.
          - <b>Tap down and drag finger out of the checkbox and release finger</b>:
          Cancels the press and does not change the state.
          - <b>Tap down and drag finger to scroll UI</b>: Cancels the press
          and does not change the state.

    \section DuiCheckboxViewOpenIssues Open issues
        - Checkbox should include a text label describing the setting or variable
        in question. This label has the same interactions as the Checkbox button itself.

    \sa DuiButton DuiButtonView DuiCheckboxStyle
*/
class DUI_EXPORT DuiCheckboxView : public DuiButtonView
{
    Q_OBJECT
    DUI_VIEW(DuiButtonModel, DuiCheckboxStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    DuiCheckboxView(DuiButton *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~DuiCheckboxView();

protected:

    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

private:

    Q_DISABLE_COPY(DuiCheckboxView)
    Q_DECLARE_PRIVATE(DuiCheckboxView)
};

#endif
