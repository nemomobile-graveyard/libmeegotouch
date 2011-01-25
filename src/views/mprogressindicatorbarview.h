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

#ifndef MPROGRESSINDICATORBARVIEW_H
#define MPROGRESSINDICATORBARVIEW_H

#include "mwidgetview.h"
#include <mprogressindicatormodel.h>
#include <mprogressindicatorstyle.h>

class MProgressIndicatorBarViewPrivate;
class MProgressIndicatorBarViewTest;
class MProgressIndicator;
class QGraphicsSceneResizeEvent;

/*!
    \class MProgressIndicatorBarView
    \brief Progress bar view for MProgressIndicator.

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

    \sa MProgressIndicator MProgressIndicatorModel MProgressIndicatorStyle
*/

class M_VIEWS_EXPORT MProgressIndicatorBarView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MProgressIndicatorModel, MProgressIndicatorStyle)

public:
    /*!
        \brief Constructor
        \param controller  Pointer to the progressindicator's controller
    */
    MProgressIndicatorBarView(MProgressIndicator *controller);

    /*!
        \brief Destructor
    */
    virtual ~MProgressIndicatorBarView();

private Q_SLOTS:

    /*!
      \brief Called when underlying controller object visibility changed
     */
    void visibilityChangedSlot();

    /*!
      \brief Called when underlying progress bar animation is resumed (when progressbar enters display)
     */
    void resumeAnimation();

    /*!
      \brief Called when underlying progress bar animation is paused (when progressbar exits display)
     */
    void pauseAnimation();

protected:
    //! \reimp
    /*!
     * Draws the contents for this view.
     */
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
        \brief Updates MSpinnerView class instance when current model is changed.
     */
    virtual void setupModel();

    /*!
        \brief Updates MProgressIndicatorBarView class instance when current style is changed.

        Called when MProgressIndicatorStyle class instance is changed (usually)
        during initialization.
     */
    virtual void applyStyle();

    /*!
      \brief Called when underlying controller object is resized
     */
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

    /*!
      \brief Called when direction of layout containing underlying controller changed (LTR / RTL order)
     */
    virtual void changeEvent(QEvent *event);   

    MProgressIndicatorBarViewPrivate *const d_ptr;


protected Q_SLOTS:
    /*!
        \brief Updates MSpinnerView class instance when some component
                   of underlying model is modified.
        \param modifications List containing the names of modified model components.
    */
    virtual void updateData(const QList<const char *>& modifications);

private:
    Q_DISABLE_COPY(MProgressIndicatorBarView)
    Q_DECLARE_PRIVATE(MProgressIndicatorBarView)

#ifdef UNIT_TEST
    friend class Ut_MProgressIndicatorBarView;
#endif

#ifdef M_TEST_CLASS
    M_TEST_CLASS
#endif
};

#endif // MPROGRESSINDICATORBARVIEW_H
