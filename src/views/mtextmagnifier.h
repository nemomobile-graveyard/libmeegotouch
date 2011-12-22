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
#ifndef MTEXTMAGNIFIER_H
#define MTEXTMAGNIFIER_H

#include "mstylablewidget.h"
#include "mtextmagnifierstyle.h"
#include "mtopleveloverlay.h"

#include <QPropertyAnimation>

//! \internal

/*! \brief Magnifier widget to magnify text.
 *
 *  The class magnifies contents of a certain source item.
 *  Position of the magnification is set by calling \a setMagnifiedPosition.
 *  Size of the magnification is determined entirely by style defined amount
 *  of magnification, and the size of the magnifier widget. Size of the widget
 *  on the other hand is defined by its frame image, read from style.
 *
 *  To be always on top of other widgets in scene this magnifier uses
 *  MOverlay internally as a parent scene window.
 */
class MTextMagnifier : public MStylableWidget
{
    Q_OBJECT
public:
    enum DeletionPolicy {
        KeepWhenDone,    //!< Magnifier is kept alive after being disappeared.
        DestroyWhenDone  //!< Magnifier is destroyed after being disappeared.
    };

    /*! \brief Constructs magnifier that magnifies \a sourceWidget.
     *  \param sourceWidget The contents of this widget is magnified.
     *  \param keepVisibleSize Size of centered rectangle in magnifier
     *         which is guaranteed to be kept within screen limits. This is
     *         given in unmagnified, \a sourceWidget, units.
     */
    explicit MTextMagnifier(const MWidget &sourceWidget,
                            const QSizeF &keepVisibleSize);
    virtual ~MTextMagnifier();

    //! Show the magnifier. You should call \a setMagnifiedPosition before this.
    void appear();

    //! Hide magnifier.
    //! \param deletionPolicy Whether to destroy magnifier after animation or not.
    void disappear(DeletionPolicy deletionPolicy);

    //! Set magnified position in source item coordinates.
    //! Magnifier is drawn at this position unless offset is defined in style.
    void setMagnifiedPosition(const QPointF &sourceWidgetPos);

    //! Tells whether magnifier is visible at the moment.
    bool isAppeared() const;

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter,
                              const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QRectF boundingRect() const;
    //! \reimp_end

private:
    MTextMagnifier();

    void prepareOffscreenSurface(const QSize &size);
    QSizeF frameSize() const;
    void updateMagnifierOffset();
    void setupAnimationParameters();

private slots:
    void handleScaleUpAnimationFinished();

private:
    const MWidget &sourceWidget;
    const QSizeF keepVisibleSize;
    QPointF offsetFromCenter;
    QScopedPointer<QPixmap> offscreenSurface;

    DeletionPolicy deletionPolicy;

    QPropertyAnimation scaleUpAnimation;

    MTopLevelOverlay overlay;

    M_STYLABLE_WIDGET(MTextMagnifierStyle)

    friend class Ut_MTextMagnifier;
};

//! \internal_end

#endif
