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

#ifndef MSLIDERVIEW_H
#define MSLIDERVIEW_H

#include "mwidgetview.h"
#include <mslidermodel.h>
#include <msliderstyle.h>

class MSliderViewPrivate;
class MSlider;

/*!
    \class MSliderView
    \brief Standard view is used to visualize sliders and seekbars.

    \ingroup views

    \section MSliderViewOverview Overview
        Slider view class renders slider and seekbars.

        Outlook of sliders and seekbars can be changed using the styling
        attributes defined in MSliderStyle and MWidgetStyle.

    \section MSliderViewInteractions Interactions
        A slider (seekbar) can be in one of the following functional states:
        - Released: default state.
        - Pressed: when slider handle is tapped.
        Tapping slider handle will catch it and then slider will fall into Pressed state.
        In this state slider handle follows tapping point (is dragged).
        Slider value is changed by draging slider handle to required place.
        If slider groove is tapped, slider thumb will be moved to that place
        and slider will fall into Pressed state.

    \section MSliderViewOpenIssues Open issues
        - Animated slider handle moving (when dragging or tapping slider groove) not supported yet.

    \sa MSliderStyle MWidgetView
*/

class M_VIEWS_EXPORT MSliderView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MSliderModel, MSliderStyle)

    //! \internal
    Q_PROPERTY(int position READ position WRITE setPosition)
    //! \internal_end

public:
    /*!
        \brief Constructs toolbar view.
        \param controller Pointer to the controller.
     */
    MSliderView(MSlider *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MSliderView();

    /* \internal */
    virtual void setGeometry(const QRectF &rect);
    /* \internal_end */

protected:
    /*!
        \brief Updates MSliderView class instance when current style is changed.

        Called when MSliderStyle class instance is changed (usually)
        during initialization.
     */
    virtual void applyStyle();

    /*!
        \brief Mouse press event handler.

        Accepted when clicking onto slider groove. Ignored clicking outside
        of slider groove. Timer event will be generated which set slider
        state to Pressed
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /*!
        \brief Mouse press event handler.

        Releases slider handle. Slider falls back into Released state and
        MSlider::sliderReleased() signal is emitted.
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /*!
        \brief Mouse move event handler.

        If slider is in pressed State drags slider thumb. As slider handle is dragged
        MSlider::valueChanged(int) signal is emitted.
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /*!
        \brief Cancel event handler.

        Sets slider back to Released state.
     */
    virtual void cancelEvent(MCancelEvent *event);

    /*!
        \brief Swipe gesture event handler.

        Accepts swipe events, so they are not sent to the parent widgets while
        the user is dragging the slider..
     */
    virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);

    /*!
        \brief Updates MSliderView class instance when current model is changed.

        Called when MSliderModel (MSeekBarModel) class instance is changed
        (usually) during initialization.
     */
    virtual void setupModel();

    /*!
        \brief Updates MSliderView class instance when some component
                   of underlying model is modified.
        \param modifications List containing the names of modified model components.

        Called when some field of underlying MSliderModel (MSeekBarModel)
        class instance is modified and view shuold reflect that modification.
     */
    virtual void updateData(const QList<const char *>& modifications);

    /*!
      \brief Timer event handler

      Called when timer event occures.
      */
    virtual void timerEvent(QTimerEvent* event);

    /*!
      \brief Timer event handler

      Called when timer event occures.
      */
    virtual void hideEvent(QHideEvent* event);

    MSliderViewPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MSliderView)
    Q_DECLARE_PRIVATE(MSliderView)

    //! \internal
    int position() const;
    void setPosition(int position);
    //! \internal_end

private Q_SLOTS:
    /*!
      \brief Lowers slider handle indicator
      */
    void lowerSliderHandleIndicator();

    /*!
      \brief Called when underlying controller object visibility changed
     */
    void changeSliderHandleIndicatorVisibility();

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_MSliderView;
#endif // UNIT_TEST

};

#endif
