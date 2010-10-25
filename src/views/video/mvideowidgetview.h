/* * This file is part of libmeegotouch *
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */
#ifndef MVIDEOWIDGETVIEW_H
#define MVIDEOWIDGETVIEW_H

#include <mwidgetview.h>
#include <mvideowidgetmodel.h>
#include <mvideowidgetstyle.h>

class MVideoWidgetViewPrivate;
class MVideoWidget;

class QGraphicsSceneResizeEvent;

/*!
    \class MVideoWidgetView
    \brief View class for a video frame.

    \ingroup views

    \section MVideoWidgetViewOverview Overview
        Standard view for displaying video frames without any other graphical
        components.

    \section MVideoWidgetViewInteractions Interactions

    \section MVideoWidgetViewOpenIssues Open issues

    \sa MVideoWidget MVideoWidgetStyle
*/
class M_VIEWS_EXPORT MVideoWidgetView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MVideoWidgetModel, MVideoWidgetStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    MVideoWidgetView(MVideoWidget* controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MVideoWidgetView();

    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent* event);
    //! \reimp_end

protected:

    //! \reimp
    virtual void drawContents(QPainter* painter, const QStyleOptionGraphicsItem* option) const;
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void setupModel();
    virtual void cancelEvent(MCancelEvent* event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const;
    //! \reimp_end

    //! \cond
    MVideoWidgetView(MVideoWidgetViewPrivate& dd, MVideoWidget* controller);
    //! \endcond

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char*>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MVideoWidgetView)
    Q_DECLARE_PRIVATE(MVideoWidgetView)

    Q_PRIVATE_SLOT(d_func(), void videoReady())
    Q_PRIVATE_SLOT(d_func(), void frameReady())
    Q_PRIVATE_SLOT(d_func(), void stateChanged())
};

#endif
