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

#ifndef DUISLIDERVIEW_H
#define DUISLIDERVIEW_H

#include "duiwidgetview.h"
#include <duislidermodel.h>
#include <duisliderstyle.h>

class DuiSliderViewPrivate;
class DuiSlider;

/*!
    \class DuiLabelView
    \brief Standard view is used to visualize sliders and seekbars.

    \ingroup views

    \section DuiSliderViewOverview Overview
        Slider view class renders slider and seekbars.

        Outlook of sliders and seekbars can be changed using the styling
        attributes defined in DuiSliderStyle and DuiWidgetStyle.

    \section DuiSliderViewInteractions Interactions
        A slider (seekbar) can be in one of the following functional states:
        - Released: default state.
        - Pressed: when slider handle is tapped.
        Tapping slider handle will catch it and then slider will fall into Pressed state.
        In this state slider handle follows tapping point (is dragged).
        Slider value is changed by draging slider handle to required place.
        If slider groove is tapped, slider thumb will be moved to that place
        and slider will fall into Pressed state.

    \section DuiSliderViewOpenIssues Open issues
        - Animated slider handle moving (when dragging or tapping slider groove) not supported yet.

    \sa DuiSliderStyle DuiWidgetView
*/

class DUI_EXPORT DuiSliderView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiSliderModel, DuiSliderStyle)

public:
    /*!
        \brief Constructs toolbar view.
        \param Pointer to the controller.
     */
    DuiSliderView(DuiSlider *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~DuiSliderView();

protected:
    /*!
        \brief Updates DuiSliderView class instance when current style is changed.

        Called when DuiSliderStyle class instance is changed (usually)
        during initialization.
     */
    virtual void applyStyle();

    /*!
        \brief Mouse press event handler.

        Accepted when clicking onto slider groove. Ignored clicking outside
        of slider groove. Slider falls into Pressed state and DuiSlider::sliderPressed()
        signal is emitted.
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /*!
        \brief Mouse press event handler.

        Releases slider handle. Slider falls back into Released state and
        DuiSlider::sliderReleased() signal is emitted.
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /*!
        \brief Mouse move event handler.

        If slider is in pressed State drags slider thumb. As slider handle is dragged
        DuiSlider::valueChanged(int) signal is emitted.
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /*!
        \brief Cancel event handler.

        Sets slider back to Released state.
     */
    virtual void cancelEvent(DuiCancelEvent *event);

    /*!
        \brief Updates DuiSliderView class instance when current model is changed.

        Called when DuiSliderModel (DuiSeekBarModel) class instance is changed
        (usually) during initialization.
     */
    virtual void setupModel();

    /*!
        \brief Updates DuiSliderView class instance when some component
                   of underlying model is modified.
        \param List containing the names of modified model components.

        Called when some field of underlying DuiSliderModel (DuiSeekBarModel)
        class instance is modified and view shuold reflect that modification.
     */
    virtual void updateData(const QList<const char *>& modifications);

private:
    Q_DISABLE_COPY(DuiSliderView)
    Q_DECLARE_PRIVATE(DuiSliderView)

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_DuiSliderView;
#endif // UNIT_TEST

};

#endif
