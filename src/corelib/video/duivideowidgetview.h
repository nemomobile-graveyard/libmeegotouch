/* * This file is part of libdui *
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
#ifndef DUIVIDEOWIDGETVIEW_H
#define DUIVIDEOWIDGETVIEW_H

#include <duiwidgetview.h>
#include <duivideowidgetmodel.h>
//#include <duivideowidgetstyle.h>

class DuiVideoWidgetViewPrivate;
class DuiVideoWidget;

class QGraphicsSceneResizeEvent;

/*!
    \class DuiVideoWidgetView
    \brief View class for a video frame.

    \ingroup views

    \section DuiVideoWidgetViewOverview Overview
        Standard view for displaying video frames without any other graphical
        components.        

    \section DuiVideoWidgetViewInteractions Interactions
          
    \section DuiVideoWidgetViewOpenIssues Open issues
        
    \sa DuiVideoWidget DuiVideoWidgetStyle
*/
class DUI_EXPORT DuiVideoWidgetView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiVideoWidgetModel, DuiWidgetStyle)

public:

    /*!
        \brief Constructs the view.
        \param Pointer to the controller.
     */
    DuiVideoWidgetView(DuiVideoWidget* controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~DuiVideoWidgetView();

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
    virtual void cancelEvent(DuiCancelEvent* event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const;
    //! \reimp_end

    //! \cond
    DuiVideoWidgetView(DuiVideoWidgetViewPrivate& dd, DuiVideoWidget* controller);
    //! \endcond
    
protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char*>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiVideoWidgetView)
    Q_DECLARE_PRIVATE(DuiVideoWidgetView)

    Q_PRIVATE_SLOT(d_func(), void videoReady())
    Q_PRIVATE_SLOT(d_func(), void frameReady())
    Q_PRIVATE_SLOT(d_func(), void stateChanged())
};

#endif
