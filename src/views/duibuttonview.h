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

#ifndef DUIBUTTONVIEW_H
#define DUIBUTTONVIEW_H

#include <duiwidgetview.h>
#include <duibuttonmodel.h>
#include <duibuttonstyle.h>

class DuiButtonViewPrivate;
class DuiButtonViewTest;
class DuiButton;

class QGraphicsSceneResizeEvent;

/*!
    \class DuiButtonView
    \brief View class for standard push buttons.

    \ingroup views

    \section DuiButtonViewOverview Overview

        <TABLE border="0">
        <TR>
            <TD> \image html push_button.png </TD>
            <TD> \image html push_button_down.png </TD>
        </TR>
        </TABLE>

        DuiButtonView is used to visualize push buttons and toggle
        buttons. The view draws a fixed background and an icon, text or both
        on top of it. If the view is for a checkable button, it uses a different
        background and different text color for the checked state.

        The outlook of button can be changed using the styling attributes defined
        in DuiButtonStyle and DuiWidgetStyle. DuiWidgetStyle::backgroundImage,
        DuiWidgetStyle::backgroundColor and DuiWidgetStyle::backgroundOpacity
        can be used to change appearance of the button itself. If background image
        is not defined then the background is drawn with solid color. The icon
        and text position and outlook can be changed through DuiButtonStyle.
        Following rules apply:

        - Only text is visible.
            - Text can be freely positioned using DuiButtonStyle::verticalTextAlign,
              DuiButtonStyle::horizontalTextAlign, DuiButtonStyle::textMarginLeft,
              DuiButtonStyle::textMarginRight, DuiButtonStyle::textMarginTop and
              DuiButtonStyle::textMarginBottom attributes.
        - Only icon is visible.
            - Icon can be freely positioned using DuiButtonStyle::iconSize and
              DuiButtonStyle::iconAlign attributes.
        - Text and icon are both visible.
            - Space taken by the icon is calculated first and then the rest of
              available space is used for the text. The space taken by the text
              is adjustable with text margin attributes.
            - If icon is aligned to left, then the text goes to right or if icon is
              aligned to top then the text goes automatically below that.

        A button can be in one of the following functional states:
          - Enabled: default state. Button can be tapped to perform an action.
          - Disabled: tapping the button doesn't perform any action.
            - Disabled state is not indicated in any specific way, button is
            shown normally (i.e. no dimming etc.)

    \section DuiButtonViewInteractions Interactions
        The Push Button does not support long press functionality, so it makes
        no difference how long the button is held down. If user presses a button
        and drags while it is in a pannable area, then button follows the finger
        and tactile feedback is cancel. When releasing the finger after panning,
        the button interaction is not activated.

          - <b>Press down</b>: Highlights the button.
          - <b>Tap</b> (button down + button up): Performs an action defined
          by application.
          - <b>Tap down and drag finger out of the button and release finger</b>:
          Cancels the button press.
          - <b>Tap down and drag finger to scroll UI</b>: Cancels the button
          press.

    \section DuiButtonViewOpenIssues Open issues
        - There should be a informative banner telling why the button is disabled,
          unless the reason is very self evident.
        - The Button labels should follow the truncation rules of the Label widget.

    \sa DuiButton DuiButtonView DuiButtonStyle
*/

class DUI_EXPORT DuiButtonView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiButtonModel, DuiButtonStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    DuiButtonView(DuiButton *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~DuiButtonView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:

    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void setupModel();
    virtual void cancelEvent(DuiCancelEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

    /*!
        \brief Draws the button icon (toggled or normal depending on the state
               of the button) into the given \a iconRect.
     */
    virtual void drawIcon(QPainter *painter, const QRectF &iconRect) const;

    //! \internal
    DuiButtonView(DuiButtonViewPrivate &dd, DuiButton *controller);
    //! \internal_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiButtonView)
    Q_DECLARE_PRIVATE(DuiButtonView)

#ifdef UNIT_TEST
    friend class Ut_DuiButtonView;
#endif

};

#endif
