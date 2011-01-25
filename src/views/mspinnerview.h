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

#ifndef MSPINNERVIEW_H
#define MSPINNERVIEW_H

#include "mwidgetview.h"
#include <mprogressindicatormodel.h>
#include <mspinnerstyle.h>

class MSpinnerViewPrivate;
class MProgressIndicator;

/*!
    \class MSpinnerView
    \brief Spinner view for MProgressIndicator.

    \ingroup views

    \section SpinnerOverview Overview
        Spinner rotates clockwise indefinitely until the process is finished or interrupted. Spinner is
        used only for unknown durations. It can can be placed anywhere in UI and also on top of other UI
        components to indicate ongoing processes. It should be used when the available space is limited.
        It can be used to indicate e.g. scanning or refreshing

        <TABLE border="0">
        <TR><TD> \image html circular_progress.png</TD>
            <TD> Progress indicator with spinner view.</TD></TR>
        </TABLE>

    \section SpinnerInteractions Interactions
        Spinner is always non-interactive.

    \section SpinnerOpenIssues Open issues
        \li Progress indicator shape, visualization, animation, size and possible subcomponents such
            as label and error notification? Nothing yet decided by UI/Graphics team, so far only concepting
            has been done.
        \li Layout: where to display the current status (%, time, data bytes) of the progress indication?
        \li Graphics

    \sa MProgressIndicator MProgressIndicatorModel MSpinnerStyle
*/

class M_VIEWS_EXPORT MSpinnerView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MProgressIndicatorModel, MSpinnerStyle)

    //! \internal
    Q_PROPERTY(int currentFrame READ currentFrame WRITE setCurrentFrame)
    //! \internal_end

public:
    /*!
     * \brief Constructor
     *
     * \param controller  Pointer to the progressindicator's controller
     */
    MSpinnerView(MProgressIndicator *controller);

    /*!
     * \brief Destructor
     */
    virtual ~MSpinnerView();

protected:
    /*!
     * Draws the contents for this view.
     */
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
        \brief Updates MSpinnerView class instance when current model is changed.
     */
    virtual void setupModel();

    /*!
        \brief Updates MSpinnerView class instance when current style is changed.

        Called when MSpinnerStyle class instance is changed (usually)
        during initialization.
     */
    virtual void applyStyle();

    MSpinnerViewPrivate *const d_ptr;

protected Q_SLOTS:
    /*!
        \brief Updates MSpinnerView class instance when some component
                   of underlying model is modified.
        \param modifications List containing the names of modified model components.
    */
    virtual void updateData(const QList<const char *>& modifications);

private:
    int currentFrame() const;
    void setCurrentFrame(int frameIndex);

    Q_DISABLE_COPY(MSpinnerView)
    Q_DECLARE_PRIVATE(MSpinnerView)

    Q_PRIVATE_SLOT(d_func(), void _q_pauseAnimation())
    Q_PRIVATE_SLOT(d_func(), void _q_resumeAnimation())
    Q_PRIVATE_SLOT(d_func(), void _q_pauseOrResumeAnimation())
    Q_PRIVATE_SLOT(d_func(), void _q_switcherEntered())
    Q_PRIVATE_SLOT(d_func(), void _q_switcherExited())

#ifdef UNIT_TEST
    friend class Ut_MProgressIndicatorSpinnerView;
#endif
};

#endif // MSPINNERVIEW_H
