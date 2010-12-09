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

class M_CORE_EXPORT MWidget : public QGraphicsWidget
{
    Q_OBJECT

    Q_PROPERTY(QSizePolicy sizePolicy READ sizePolicy WRITE setSizePolicy)
    Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
    Q_PROPERTY(bool onDisplay READ isOnDisplay)
    Q_PROPERTY(QPointF paintOffset READ paintOffset WRITE setPaintOffset)

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

    /*!
     Returns the offset by which the widget will be transformed when painted.

     This convenience function is equivalent to querying the horizontal and
     vertical translation factors of the widgets's transform().

     \sa setPaintOffset()
     */
    QPointF paintOffset() const;

    /*!
     Sets the offset by which the widget will be transformed when painted.

     This convenience function is equivalent to calling setTransform on the widget,
     supplying a QTransform that shifts the widget by \a offset.

     The paint offset allows for a widget to appear as if in a position
     different than that of the widget's pos() property. This allows for example
     to animate the widget's apparent location using a QPropertyAnimation
     without affecting layouting or the layout affecting the animation.

     \sa paintOffset()
     */
    void setPaintOffset(const QPointF & offset);

    /*!
     * This enum contains possible values of a mouse event cancel policy.
     *
     * \li MouseEventNoCancel - the cancel event will not be sent if this widget receives a gesture.
     * \li MouseEventCancelOnGestureStarted - the cancel event will be sent when
     *     a gesture is being started and accepted by this widget.
     * \li MouseEventCancelOnGestureFinished - the cancel event will be sent when
     *     a gesture is being finished and was accepted by this widget.
     *
     * \sa grabGestureWithCancelPolicy.
     */
    enum MouseEventCancelPolicy {
        MouseEventNoCancel,
        MouseEventCancelOnGestureStarted,
        MouseEventCancelOnGestureFinished
    };

    /*!
     Subscribes the widget to gestures events of a specific \a type. Allows definition
     of cancel event handling policy for that gesture.

     The meegotouch library can send cancel event to the current mouse grabber and
     stop processing mouse events. This allows the UI to react to situation when
     the user starts a gesture on top of interactive elements like buttons. In
     a typical example, the user starts panning gesture over a button, which will
     initially reach the pressed state. Then the user moves his finger and the button
     will then be moved back to un-pressed state, without triggering actions connected
     to it. Internally, the button will receive a cancel event and it will not receive
     the release event.

     This method is an overloaded version of grabGesture defined in QGraphicsWidget
     which introduces \a cancelPolicy argument. It can be used to define the reaction
     to started or finished gesture when it reaches the object of the MWidget.

     \sa MouseEventCancelPolicy
     */
    void grabGestureWithCancelPolicy(Qt::GestureType type, Qt::GestureFlags flags = Qt::GestureFlags(), MouseEventCancelPolicy cancelPolicy = MouseEventNoCancel);

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
    /** Hide the non-virtual QObject::setObjectName()  with this virtual function to catch calls to
     * change the object name.  This is required to allow us to update the style from CSS. 
     *
     * Warning: Since QObject::setObjectName() is non-virtual, code like:
     * \code
     *   MLabel *label = new MLabel("Hello"); label->setObjectName("hello");
     * \endcode
     * Would correctly restyle the label.  But:
     * \code
     *   QGraphicsWidget *label = new MLabel("Hello"); label->setObjectName("hello");
     * \endcode
     * Would not update the CSS style correctly.
     *
     * \deprecated Use MWidgetController:setStyleName instead.
     */
    virtual void setObjectName(const QString &name);

    /**
    * Hide the non-virtual QGraphicsItem::setSelected() with this virtual function to catch calls
    * to select the object.  This is required to allow for custom selection policies.
    *
    * It is up to widget to decide how selection should be shown. By default a widget is not selected.
    */
    void setSelected(bool selected);
    /*! \reimp_end */

    /*!
        Removes all the actions from the widget's action list.
        Note: actions are not deleted after removal from the list.
        To clear and delete the actions from the widget, following procedure is proposed:
        <code>
            QList<QAction*> actions = widget->actions();
            widget->clearActions();
            qDeleteAll(actions);
        </code>

    */
    virtual void clearActions();

Q_SIGNALS:

    /*!
     * A signal that is emitted when:
     * - the widget has entered the visible area of the display
     * - the page containing the widget is not obscured anymore by another window / page
     * - the widget is explicitly shown with show() or setVisible(true) and is on the display area.
     * Note!: the stacking order of widgets is not taken into account
     */
    void displayEntered();

    /*!
     * A signal that is emitted when:
     * - the widget has left the visible area of the display
     * - the page containing the widget has become obscured by another window / page
     * - the widget is explicitly hidden with hide() or setVisible(false).
     * Note!: the stacking order of widgets is not taken into account
     */
    void displayExited();

protected:

    /*!
     * A handler that is called just before displayEntered() is emitted.
     */
    virtual void enterDisplayEvent();

    /*!
     * A handler that is called just before displayExited() is emitted.
     */
    virtual void exitDisplayEvent();

    /*!
     * This event handler allows a widget to notify subscribers (and its children) about
     * changes in its presence on the display. enterDisplayEvent() and exitDisplayEvent()
     * convenience handlers are called by the default implementation. MOnDisplayChangeEvent
     * may be sent e.g. if the widget gets panned out of display, the widget is explicitly
     * hidden/shown or the window gets obscured by another window.
     */
    virtual void onDisplayChangeEvent(MOnDisplayChangeEvent *event);

    /*! \reimp */
    bool event(QEvent *event);
    bool sceneEvent(QEvent *event);
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
