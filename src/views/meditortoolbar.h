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
#ifndef MEDITORTOOLBAR_H
#define MEDITORTOOLBAR_H

#include <MStylableWidget>
#include "meditortoolbarstyle.h"

class MEditorToolbarPrivate;

/*! \brief Cut/Copy/Paste etc. popup-toolbar for MTextEdit[View]
 *
 * Actions such as MTextEdit's default cut, copy and paste are added to MEditorToolbar as
 * standard QActions using QGraphicsWidget::addAction().  You can only append actions,
 * inserting to arbitrary positions is not supported.  Actions are represented as
 * clickable buttons.
 *
 * The only supported action properties are \a visible and \a text.
 *
 * The only supported action property changes are changes to the \a visible property.
 *
 * While MEditorToolbar inherits from MStylableWidget, you must not try to control
 * visibility using hide(), show(), setVisible() and isVisibile().  Use \a appear, \a
 * disappear and \a isAppeared instead.  This widget actually becomes a child of an
 * internal overlay widget.
 *
 * There is no specific reason why this class could not be used with widgets other than
 * MTextEdit but no promises about applicability to other use cases are made.
 */
class MEditorToolbar : public MStylableWidget
{
    Q_OBJECT

public:
    //! \brief Configure placement of the toolbar with respect to its position.
    enum ToolbarPlacement {
        BelowPointOfInterest, //!< Below origin, arrow pointing upwards
        AbovePointOfInterest  //!< Above origin, arrow pointing downwards
    };

    //! \brief Construct MEditorToolbar and make it follow \a followWidget
    explicit MEditorToolbar(const MWidget &followWidget);
    virtual ~MEditorToolbar();

    /*! \brief Set position of the toolbar
     *
     *  \param pos position in the local coordinate system of \a followWidget given to the
     *  constructor.  Position should be the point MEditorToolbar actions apply to,
     *  such as cursor or selection.
     *  \param placement Placement of the toolbar with respect to the point of interest \a pos.
     */
    void setPosition(const QPointF &pos, ToolbarPlacement placement);

    /*! \return whether the widget is appeared or not.
     *
     *  Widget is considered appeared always after appear() is called,
     *  even if there is no buttons to show and widget would seem to be
     *  hidden.
     */
    bool isAppeared() const;

signals:
    //! \brief Emitted when the size of the toolbar is changed.
    void sizeChanged();

public slots:
    /*! \brief Show the toolbar.
     *
     *  \param autohide if set, toolbar will be hidden after timeout.
     *  The timeout value is taken from hideTimeout attribute from style.
     *
     *  Nothing is shown unless/until at least one action is in visible state.
     */
    void appear(bool autohide);

    //! \brief Hide the toolbar.
    void disappear();

protected:
    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which,
                            const QSizeF &constraint = QSizeF()) const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *);
    virtual bool event(QEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    //! \reimp_end

protected slots:
    // Make this a slot for easier updating of size hint.
    virtual void updateGeometry();

private:
    Q_DECLARE_PRIVATE(MEditorToolbar)
    MEditorToolbarPrivate * const d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_updateAvailableButtons())
    Q_PRIVATE_SLOT(d_func(), void _q_startAnimatedHide())

    //! Prevent default construction.
    MEditorToolbar();

    M_STYLABLE_WIDGET(MEditorToolbarStyle)
};

#endif // MEDITORTOOLBAR_H
