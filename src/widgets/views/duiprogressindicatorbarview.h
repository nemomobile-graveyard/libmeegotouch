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

#ifndef DUIPROGRESSINDICATORBARVIEW_H
#define DUIPROGRESSINDICATORBARVIEW_H

#include "duiwidgetview.h"
#include <duiprogressindicatormodel.h>
#include <duiprogressindicatorstyle.h>

class DuiProgressIndicatorBarViewPrivate;
class DuiProgressIndicatorBarViewTest;
class DuiProgressIndicator;
class QGraphicsSceneResizeEvent;

/*!
    \class DuiProgressIndicatorBarView
    \brief Progress bar view for DuiProgressIndicator.

    \ingroup views

    \tableofcontents

    \section ProgressIndicatorBarViewOverview Overview
        Progress bars are used when duration of the process is (or will be) known. Progress bar with known
        duration will give haptic feedback when operation is finished. It can be placed anywhere in UI and also
        on top of other UI components to indicate ongoing processes. For the cases when duration is not
        immediately known, the progress bar can be temporarily in unknown duration state until the duration
        becomes known. Use to indicate e.g. uploading, downloading

        <TABLE border="0">
        <TR><TD> \image html progressbar1.png</TD>
            <TD> A progress bar in basic state, filled until 100% reached.</TD></TR>
        <TR><TD> \image html progressbar2.png</TD>
            <TD> A progress bar in unknown duration state, animated indefinitely until duration becomes known.</TD></TR>
        </TABLE>

    \section ProgressIndicatorBarViewInteractions Interactions
        Progress bar is always non-interactive.

    \section ProgressIndicatorBarViewOpenIssues Open issues
        \li Progress indicator shape, visualization, animation, size and possible subcomponents such
            as label and error notification? Nothing yet decided by UI/Graphics team, so far only concepting
            has been done.
        \li Layout: where to display the current status (%, time, data bytes) of the progress indication?
        \li Graphics

    \sa DuiProgressIndicator DuiProgressIndicatorModel DuiProgressIndicatorStyle
*/

class DUI_EXPORT DuiProgressIndicatorBarView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiProgressIndicatorModel, DuiProgressIndicatorStyle)

public:
    /*!
        \brief Constructor
        \param controller  Pointer to the progressindicator's controller
    */
    DuiProgressIndicatorBarView(DuiProgressIndicator *controller);

    /*!
        \brief Destructor
    */
    virtual ~DuiProgressIndicatorBarView();

private Q_SLOTS:
    void animationTimeout();
    void visibilityChangedSlot();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiProgressIndicatorBarView)
    Q_DECLARE_PRIVATE(DuiProgressIndicatorBarView)

#ifdef DUI_TEST_CLASS
    DUI_TEST_CLASS
#endif
};

#endif // DUIPROGRESSINDICATORBARVIEW_H
