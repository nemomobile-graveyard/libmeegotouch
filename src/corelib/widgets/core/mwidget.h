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

#ifndef MWIDGET_H
#define MWIDGET_H

#include "mexport.h"

#include <QGraphicsWidget>
#include "mwindow.h"

class QGestureEvent;
class QTapAndHoldGesture;
class QPanGesture;
class QPinchGesture;
class QTapGesture;
class QSwipeGesture;

class QGraphicsItem;
class MWidgetPrivate;
class MCancelEvent;
class MOnDisplayChangeEvent;
class MOrientationChangeEvent;
class MSceneManager;
class MApplicationPage;

class M_EXPORT MWidget : public QGraphicsWidget
{
    Q_OBJECT

    Q_PROPERTY(QSizePolicy sizePolicy READ sizePolicy WRITE setSizePolicy)
    Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
    Q_PROPERTY(bool onDisplay READ isOnDisplay)

public:
    MWidget(QGraphicsItem *parent = 0);
    virtual ~MWidget();

    /*! \reimp */
    virtual QPainterPath shape() const;
    virtual void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    /*! \reimp_end */

    /*!
     * Returns a pointer to the scene manager that manages the widget.
     * If the widget's scene doesn't have a scene manager, returns a null pointer.
     */
    MSceneManager *sceneManager() const;

    /*!
     * \brief Checks whether the widget might be seen on the display.
     *
     * Returning true means that:
     * - Widget himself is not hidden (isVisible() property is true).
     * - Widget lies within sight of some QGraphicsView that is currently being displayed.
     *
     * Note that it does not guarantee, however, that the widget actually <b>is</b> being shown
     * on the display. There could be, for instance, some opaque QGraphicsItem on
     * top of it (along the z axis) blocking it from getting rendered on the viewport.
     *
     * Returning false means that currently this widget cannot be seen
     * on the display in any way. E.g.: If the widget happens to be out of sight of all the
     * QGraphicsViews that are rendering his scene.
     *
     * Its computational cost is significantly higher than that of a simple attribute
     * getter such as isVisible().
     */
    bool isOnDisplay() const;

    /**
    * Returns <code>true</code> if widget is selected otherwise <code>false</code>.
    * \sa setSelected(bool)
    */
    bool isSelected() const;

    /*! \reimp
     *   We reimplement these to distinguish between the user hiding items
     *   explicitly, and the layout hiding them.
     */
    void setVisible(bool visible);
    void hide() {
        setVisible(false);
    }
    void show() {
        setVisible(true);
    }
    /*! \reimp_end */


public Q_SLOTS:
    /*! \reimp */
    /** This is implement here because although the function already exists from
     * QGraphicsWidget it was not virtual.  This means that calling MWidget's
     * setObjectName would not call MWidgetController's setObjectName which
     * calls updateStyle().  So without this, the widget would not be styled correctly.
     */
    virtual void setObjectName(const QString &name);

    /**
    * This is implemented here because of limitations of item selection which is provided by QGraphicsScene.
    * There is no way to implement custom selection policies, because QGraphicsItem and QGraphicsScene have
    * there own selection logic which is very limited and can't be extended.
    *
    * Notifies widget that it was selected or deselected depending on parameter. It's up to widget to decide
    * how selection should be shown. By default widget is not selected.
    * \a selected specify if widget is selected <code>true</code> or not
    */
    void setSelected(bool selected);
    /*! \reimp_end */

    /*!
        Removes all the actions from the widget's action list.
        Note: actions are not deleted after removal from the list.
    */
    virtual void clearActions();

Q_SIGNALS:

    /*!
     * A signal that is emitted when the widget has entered the visible area of the display or
     * the page containing the widget is not obscured anymore by another window / page.
     * Note!: this is different from Qt's visibilityChanged(), which is emitted due to show() and hide().
     */
    void displayEntered();

    /*!
     * A signal that is emitted when the widget has left the visible area of the display or
     * the page containing the widget has become obscured by another window / page.
     * Note!: this is different from Qt's visibilityChanged(), which is emitted due to show() and hide().
     */
    void displayExited();

protected:

    /*!
     * A handler that is called when the widget has entered the visible area of the display or
     * the page containing the widget is not obscured anymore by another window / page.
     */
    virtual void enterDisplayEvent();

    /*!
     * A handler that is called when the widget has left the visible area of the display or
     * the page containing the widget has become obscured by another window / page.
     */
    virtual void exitDisplayEvent();

    /*!
     * This event handler allows a widget to notify subscribers (and its children) about
     * changes in its presence on the display. enterDisplayEvent() and exitDisplayEvent()
     * convenience handlers are called by the default implementation. MOnDisplayChangeEvent
     * may be sent e.g. if the widget gets panned out of display or the window gets obscured
     * by another window.
     */
    virtual void onDisplayChangeEvent(MOnDisplayChangeEvent *event);

    /*! \reimp */
    bool event(QEvent *event);
    /*! \reimp_end */

    /*!
      This event handler is designed to allow widgets to cancel the effect of
      previously received event. One example of that behavior is a situation
      when the user starts to pan the view and the mousePress event that was
      sent when the user touched the screen needs to be cancelled.
     */
    virtual void cancelEvent(MCancelEvent *event);

    /*!
     * This event handler is designed to allow widgets to react to the
     * orientation change. Reimplement it to make your widget sensitive to
     * orientation changes. Default implementation does nothing.
     */
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);

    virtual void actionEvent(QActionEvent *);

    /*!
        This event handler can be reimplemented in a subclass to process context
        menu events. The \a event parameter contains details about the event to
        be handled.
    */
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    /*! \reimp_end */

    /*!
        This virtual function is called when this widget received a
        QEvent::LanguageChange event.

        Reimplement this function in your own widgets to retranslate
        user-visible strings.
     */
    virtual void retranslateUi();

    /*!
     * This event handler is designed to allow widgets to react to the
     * gesture events. Reimplement it to make your widget sensitive to
     * custom made gestures.
     *
     * Default implementation handles gestures used by M library. Please
     * remember to call MWidget::gestureEvent() if this method is overriden.
     */
    virtual void gestureEvent(QGestureEvent *event);

    /*!
     * This event handler is designed to allow widgets to react to the
     * tap&hold gesture event. Default implementation does nothing.
     *
     * Gesture events are ignored by default. If you want to specifically accept
     * a gesture state, you need to call QGestureEvent::accept(QGesture*) or similar.
     */
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);

    /*!
     * This event handler is designed to allow widgets to react to the
     * pan gesture event. Default implementation does nothing.
     *
     * Gesture events are ignored by default. If you want to specifically accept
     * a gesture state, you need to call QGestureEvent::accept(QGesture*) or similar.
     */
    virtual void panGestureEvent(QGestureEvent *event, QPanGesture* gesture);

    /*!
     * This event handler is designed to allow widgets to react to the
     * pinch gesture event. Default implementation does nothing.
     *
     * Gesture events are ignored by default. If you want to specifically accept
     * a gesture state, you need to call QGestureEvent::accept(QGesture*) or similar.
     */
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);

    /*!
     * This event handler is designed to allow widgets to react to the
     * tap gesture event. Default implementation does nothing.
     *
     * Gesture events are ignored by default. If you want to specifically accept
     * a gesture state, you need to call QGestureEvent::accept(QGesture*) or similar.
     */
    virtual void tapGestureEvent(QGestureEvent *event, QTapGesture* gesture);

    /*!
     * This event handler is designed to allow widgets to react to the
     * swipe gesture event. Default implementation does nothing.
     *
     * Gesture events are ignored by default. If you want to specifically accept
     * a gesture state, you need to call QGestureEvent::accept(QGesture*) or similar.
     */
    virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);

protected:
    MWidgetPrivate *const d_ptr;
    MWidget(MWidgetPrivate &dd, QGraphicsItem *parent);

private:
    Q_DECLARE_PRIVATE(MWidget)
    Q_DISABLE_COPY(MWidget)

    friend class MApplicationWindow;
    friend class MScene;
    friend class MPannableViewport;
    friend class MApplicationPage;
    friend class MLayoutPrivate;

#ifdef UNIT_TEST
    friend class Ut_MWidget;
#endif
};

#endif
