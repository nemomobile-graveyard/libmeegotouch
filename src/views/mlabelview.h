/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MLABELVIEW_H
#define MLABELVIEW_H

#include <mwidgetview.h>
#include <mlabelmodel.h>
#include <mlabelstyle.h>

class MLabel;
class MLabelViewPrivate;
class QGraphicsSceneResizeEvent;
class MCancelEvent;

/*!
    \class MLabelView
    \brief Standard view class for MLabel.

    \ingroup views

    \section MLabelViewOverview Overview
        Label view class that supports rendering of simple unformatted text and rich
        html formatted text.

        <TABLE border="0">
        <TR>
            <TD> \image html label_simple.png</TD>
            <TD> Default simple label. </TD>
        </TR>
        <TR>
            <TD> \image html label_simple_custom.png</TD>
            <TD> Simple label with custom font and color.</TD>
        </TR>
        <TR>
            <TD> \image html label_simple_elided.png</TD>
            <TD> Simple label with eliding enabled.</TD>
        </TR>
        <TR>
            <TD> \image html label_rich.png</TD>
            <TD> Rich label with html styling.</TD>
        </TR>
        </TABLE>

    \section MLabelViewInteractions Interactions
        Links in rich text can be opened by clicking them. Simple unformatted label
        is totally non-interactive.

    \section MLabelViewOpenIssues Open issues
        - Rich label should support text selection.
        - Graphics: Appearance of the link, text selection needs to be defined. Does the link have icon + styled text, or only styled text?
        - Guidelines for truncating phone number, localized strings, rich label beginning truncation maybe needed later.

    \sa MLabelStyle MWidgetView
*/

class M_VIEWS_EXPORT MLabelView : public MWidgetView
{
    //FIXME
    //Temporary remove this when proper longPressEvents are coming to view.
    friend class MLabel;

    Q_OBJECT
    M_VIEW(MLabelModel, MLabelStyle)

    /*!
        \property MLabelView::renderedText
        \brief Text that is rendered on the display.

        See MLabelView::renderedText for details.
    */
    Q_PROPERTY(QString renderedText READ renderedText)

public:

    /*!
        \brief Constructs label view.
        \param Pointer to the controller.
     */
    MLabelView(MLabel *controller);

    /*!
        \brief Destructs label view.
     */
    virtual ~MLabelView();

    /*!
        Returns the text that is rendered on the display. The rendered
        text might differ from MLabel::text e.g. because the rendered
        text got elided or got line breaks.
        \return rendered text
     */
    QString renderedText() const;

    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

protected:

    /*!
        \brief Mouse press event handler.

        Accepted when displaying rich text and when clicking an anchor. Ignored
        if displaying simple text or clicking outside anchor.
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /*!
        \brief Mouse release event handler.

        If released over an anchor MLabelModel::linkActivated() signal is emitted.
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    //FIXME
    //Temporary remove this when proper longPressEvents are coming to view.
    void longPressEvent(QGraphicsSceneContextMenuEvent *event);

    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setupModel();
    virtual void applyStyle();
    virtual QFont font() const;
    virtual void cancelEvent(MCancelEvent *event);
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);
    //! \reimp_end

protected Q_SLOTS:

    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MLabelView)
    Q_DECLARE_PRIVATE(MLabelView)

    //! \internal
    QPointF mapToRoot(const QPointF &point);
    //! \internal_end

    Q_PRIVATE_SLOT(d_func(), void _q_highlighterUpdateTimerExceeded())

    //! \see MLabelViewSimple::tileInformation()
    bool tileInformation(int index, QPixmap &pixmap, QPoint& pos) const;

#ifdef UNIT_TEST
    friend class Pt_MLabel;
    friend class Ut_MLabel;
#endif
};

#endif
