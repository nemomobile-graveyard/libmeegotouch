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
class QGraphicsSceneResizeEvent;
class QTimerEvent;

/*!
    \class MSpinnerView
    \brief Spinner view for MProgressIndicator.

    \ingroup views

    \tableofcontents

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

class M_EXPORT MSpinnerView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MProgressIndicatorModel, MSpinnerStyle)

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
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end
    MSpinnerViewPrivate *const d_ptr;

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MSpinnerView)
    Q_DECLARE_PRIVATE(MSpinnerView)

    Q_PRIVATE_SLOT(d_func(), void visibilityChanged())
    Q_PRIVATE_SLOT(d_func(), void animationTimeout())
    Q_PRIVATE_SLOT(d_func(), void _q_resumeAnimation())
    Q_PRIVATE_SLOT(d_func(), void _q_pauseAnimation())
};

#endif // MSPINNERVIEW_H
