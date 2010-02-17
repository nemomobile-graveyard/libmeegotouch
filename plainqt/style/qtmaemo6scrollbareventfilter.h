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

#ifndef QTMAEMO6SCROLLBAREVENTFILTER_H
#define QTMAEMO6SCROLLBAREVENTFILTER_H

#include <QObject>
#include <QMap>
#include <QLabel>

class QWidget;
class QScrollBar;
class QAbstractScrollArea;
class QSequentialAnimationGroup;

#define SCROLLAREATHUMBVIEW "scrollAreaThumbView"
#define WIDGET_OPACITY "widgetOpacity"


/*!
 * this class shows a thumbnail of any other widget placed within an
 * scrollarea. It marks the currently visible part in the scrollarea's
 * viewport on the thumbnail
 */
class AbstractScrollAreaThumbView : public QLabel
{
    Q_OBJECT
public:
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity);
    Q_PROPERTY(QRect visibleRect READ visibleRect WRITE setVisibleRect);

    AbstractScrollAreaThumbView(QWidget *parent = NULL);

    /*!
     * returns the opacity for the widget
     */
    double opacity() const {
        return m_opacity;
    };
    /*!
     * sets the opacity of the widget
     * this is used for fade in and fade out effects
     */
    void setOpacity(double o) {
        m_opacity = o;
    };

    /*!
     * returns the rect that describes the visible part of the widget
     */
    QRect visibleRect() const {
        return m_visibleRect;
    };

    /*!
     * sets the current visible rect of the widget
     */
    void setVisibleRect(QRect rect) {
        m_visibleRect = rect;
    };
protected:
    /*! \reimp */
    virtual void paintEvent(QPaintEvent *event);
    /*! \reimp_end */
protected:
    QRect m_visibleRect;
    double m_opacity;
};


/*!
 * this class handles visibility of scrollbars.
 * it also cares for the fade effects
 */
class QtMaemo6ScrollBarEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit QtMaemo6ScrollBarEventFilter(QObject *parent);
    ~QtMaemo6ScrollBarEventFilter();

    /*!
     * enables the effects on the given scrollbars
     */
    void enableOn(QObject *o);

    bool scrollBarsAlwaysVisible() const {
        return m_scrollBarsAlwaysVisible;
    };
    void setScrollBarsAlwaysVisible(bool b) {
        m_scrollBarsAlwaysVisible = b;
    };
protected:

    /*!
     * This filter filters some events that are needed for correct scrollbar handling
     * case QEvent:Move:
     *      This event is filtered from the widget contained in scrollareas viewport.
     *      It is used for showing and hiding the scrollbars when scrolling is active.
     * case QEvent::Resize:
     *      This event is filtered from the scrollArea. It's used to manually adjust the
     *      scrollbars.
     */
    bool eventFilter(QObject *obj, QEvent *event);

    /*!
     * this updates the visibility of the scrollbar, depending of global settings
     * and the scrollrange of the scrollbar
     */
    void setScrollBarVisibility(QScrollBar *scrollBar);

    /*!
     * Generates the fade in and fade out animation, for QWidget w
     * and starts it
     * \param w the widget which sould be animated
     * \param property the property used to animate the widgets opacity
    */
    void fadeInOutAnimation(QWidget *w, const char *property);
protected Q_SLOTS:
    /*!
     * removes scrollbars from the animations list if the animation is completed
     */
    void cleanUpTimerMap();

    /*!
     * places the scrolling preview thumb
     */
    void setScrollAreaThumbGeometry(QLabel *label);

    /*!
     * this slot is called, if a scrollbars value changed
     */
    void scrollBarValueChanged();

    /*!
     * Generates a thumbnail pixmap of the whole scrollarea contents
     * and sets the pixmap to the thumbnail label of the scrollarea
     * and starts it
     * \param scrollArea the scrollArea the thumbnail should be generated for
     * \param forceUpdate forces to redraw the cached pixmap
    */
    void generateScrollAreaThumb(QAbstractScrollArea *scrollArea, bool forceUpdate = false);

protected:
    enum AnimationState {
        FadeIn = 0,
        Show = 1,
        FadeOut = 2
    };

    QMap<const QWidget *, QSequentialAnimationGroup *> m_pendingAnimations;
    bool m_scrollBarsAlwaysVisible;
    const int m_scrollAreaThumbnailMaxSize;
    const int m_scrollAreaThumbnailOffset;
    const int m_scrollAreaThumbnailBorder;
    const double m_showScrollAreaThumbnailFactor;
};


#endif //QTMAEMO6SCROLLBAREVENTFILTER_H
