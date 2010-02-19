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

#ifndef DUISPINNERVIEW_H
#define DUISPINNERVIEW_H

#include "duiwidgetview.h"
#include <duiprogressindicatormodel.h>
#include <duispinnerstyle.h>

class DuiSpinnerViewPrivate;
class DuiProgressIndicator;
class QGraphicsSceneResizeEvent;
class QTimerEvent;

/*!
    \class DuiSpinnerView
    \brief Spinner view for DuiProgressIndicator.

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

    \sa DuiProgressIndicator DuiProgressIndicatorModel DuiSpinnerStyle
*/

class DUI_EXPORT DuiSpinnerView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiProgressIndicatorModel, DuiSpinnerStyle)

public:
    /*!
     * \brief Constructor
     *
     * \param controller  Pointer to the progressindicator's controller
     */
    DuiSpinnerView(DuiProgressIndicator *controller);

    /*!
     * \brief Destructor
     */
    virtual ~DuiSpinnerView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiSpinnerView)
    Q_DECLARE_PRIVATE(DuiSpinnerView)

    Q_PRIVATE_SLOT(d_func(), void visibilityChanged(bool))
    Q_PRIVATE_SLOT(d_func(), void animationTimeout())
};

#endif // DUISPINNERVIEW_H
