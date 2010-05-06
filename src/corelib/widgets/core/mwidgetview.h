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

#ifndef MWIDGETVIEW_H
#define MWIDGETVIEW_H

#include <QObject>
#include <QSizeF>
#include <QRectF>
#include <QPainterPath>
#include <QGraphicsItem>

#include <mviewcreator.h>
#include <mwidgetstyle.h>

class QPainter;
class QStyleOptionGraphicsItem;
class QGraphicsSceneMouseEvent;
class MWidgetController;
class MWidgetViewPrivate;
class MWidgetModel;
class MWidgetStyle;
class MCancelEvent;
class MOrientationChangeEvent;
class MAbstractWidgetAnimation;

class QGestureEvent;
class QTapAndHoldGesture;
class QPanGesture;
class QPinchGesture;
class QTapGesture;
class QSwipeGesture;

class QGraphicsSceneResizeEvent;


#define M_VIEW(MODEL,STYLE) \
    public: \
    inline static const char* staticStyleType() { return #STYLE; } \
    inline virtual const char* styleType() const { return #STYLE; } \
    protected: \
    virtual MWidgetStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline MODEL* model() { return static_cast<MODEL*>(MWidgetView::model()); } \
    inline const MODEL* model() const { return static_cast<const MODEL*>(MWidgetView::model()); } \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MWidgetView::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MWidgetView::style()); }

/*!
 * \class MWidgetView
 * \brief MWidgetView provides an abstract base class for MVC views
 *
 * MWidgetView provides an abstract base class from which widgets
 * using MVC pattern can inherit their views
 */

class M_EXPORT MWidgetView : public QObject
{
    Q_OBJECT

    friend class MWidgetController;
    friend class MWidgetControllerPrivate;
    friend class MAbstractWidgetAnimation;
    friend class MTheme;
#ifdef UNIT_TEST
    friend class Ut_MWidgetView;
    friend class Ut_MWidgetController;
    friend class Ut_MPannableViewport;
#endif

protected:
    virtual MWidgetStyleContainer *createStyleContainer() const {
        return new MWidgetStyleContainer();
    }

public:
    inline static const char *staticStyleType() {
        return "MWidgetStyle";
    }
    inline virtual const char *styleType() const {
        return "MWidgetStyle";
    }

    /*!
       \brief Constructor for the view.
     */
    MWidgetView();

    MWidgetView(MWidgetController *controller);

    /*!
       \brief Destructor for the view.
     */
    virtual ~MWidgetView();

    void show();
    void hide();

    void destroy();

    /*!
      Sets this widget view into active / inactive mode. Active mode of a widget view
      follows the active mode of the widget it is applied to. Derived classes can override
      this method to add custom behaviour when active mode is enabled or disabled.
      \see MWidgetController::setActive() MWidgetController::isActive()
      \param active Indicates whether active mode is set on or off.
     */
    virtual void setActive(bool active);

    /*!
       Sets a new model for this view.
       This method notifies derived classes by calling setupModel()
     */
    void setModel(MWidgetModel *model);

    //! Do not reimplement this method in derived classes \see{drawBackground,drawContents,drawForeground}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    // the semantics of the following methods match the similar methods on QGraphicsItem,
    // QGraphicsWidget and friends
    virtual void setGeometry(const QRectF &rect);
    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void changeEvent(QEvent *event);
    qreal effectiveOpacity() const;
    qreal opacity() const;
    qreal scale() const;
    void setOpacity(qreal opacity);
    void setScale(qreal scale);

    /*!
       \brief return the used QFont.
     */
    virtual QFont font() const;

Q_SIGNALS:

protected:

    /*
      \brief Reimplementation that takes the CSS specified sizes into account.

      The default widget view size hint implementation adds the option to specify
      the hints in the style definition of the widget.

      The priority order while evaluting the hints are, from highest to lowest:
      developer set hints (using setMinimumSize, setMaximumSize and setPreferredSize),
      CSS specified hints and finally widget layout reported hints.

      Widget layout reported hints refer to the layout added to the widget with setLayout(),
      not any layout the widget may itself be inside. In order to create a widget that
      expands dynamically based on the contents inside the set layout, you should
      leave at least one dimension of the size hint unspecified in the CSS.

      Reimplemented from QGraphicsLayoutItem.
    */
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    void update(const QRectF &rect = QRectF());
    void update(qreal x, qreal y, qreal width, qreal height);
    void updateGeometry();
    QRectF geometry() const;
    QSizeF size() const;
    QPointF pos() const;
    QRectF rect() const;
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;

    /*!
      This event handler is designed to allow widgets to cancel the
      previously received mousePress event. This situation occurs, when
      the user starts to pan the view and the mousePress event that was
      sent when the user touched the screen needs to be cancelled.
     */
    virtual void cancelEvent(MCancelEvent *event);

    /*!
     * This event handler is designed to allow widgets to react to the
     * orientation change.
     */
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);

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

    /*!
     * Returns margins in pixels. These margins can be defined in css.
     */
    QRect margins() const;

    /*!
     * Returns left margin in pixels. This margin can be defined in css.
     */
    int marginLeft() const;

    /*!
     * Returns top margin in pixels. This margin can be defined in css.
     */
    int marginTop() const;

    /*!
     * Returns right margin in pixels. This margin can be defined in css.
     */
    int marginRight() const;

    /*!
     * Returns bottom margin in pixels. This margin can be defined in css.
     */
    int marginBottom() const;

    /*!
     * Returns reactive margins in pixels. These margins can be defined in css.
     */
    QRect reactiveMargins() const;

    /*!
     * Returns left reactive margin in pixels. This margin can be defined in css.
     */
    int reactiveMarginLeft() const;

    /*!
     * Returns top reactive margin in pixels. This margin can be defined in css.
     */
    int reactiveMarginTop() const;

    /*!
     * Returns right reactive margin in pixels. This margin can be defined in css.
     */
    int reactiveMarginRight() const;

    /*!
     * Returns bottom reactive margin in pixels. This margin can be defined in css.
     */
    int reactiveMarginBottom() const;

    /*!
     * This is called from MWidgetController to notify that some part
     * of the item's state has changed or will change.  It is a read-only
     * version of the QGraphicsWidget::itemChanged
     *
     * The default implementation is to set the styleMode to "disabled"
     * when an item becomes disabled, and to set the styleMode to "selected" when
     * the item becomes selected.  Reimplement this function to change this
     * default behaviour or to react to other changes.
     */
    virtual void notifyItemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

    /*!
     * Draws the background for this view.
     */
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * Draws the contents for this view.
     */
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * Draws the foreground for this view.
     */
    virtual void drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

protected Q_SLOTS:
    /*!
       Notification of model modifications. This method gets called when
       some members of the model has been modified.
     */
    virtual void updateData(const QList<const char *>& modifications);

    /*! Updates the widget's micro focus (i.e. notifies input context).
     * Similar to QWidget::updateMicroFocus.
     */
    void updateMicroFocus();

protected:
    /*!
       Notification for derived classes. This method gets called when a new model is applied for this view.
       This happens when MWidgetView::setModel() is called.
     */
    virtual void setupModel();

    /*!
       Notification for derived classes. This method gets called when
       a new style is applied for this view.  This happens e.g. when
       the object is constructed, when a new object name is given
       to the widget, or when the layout direction changes.
     */
    virtual void applyStyle();

    /*!
       Returns model of the widget. This method should be used only by the M_VIEW macro.
     */
    MWidgetModel *model();
    const MWidgetModel *model() const;

    /*!
       Returns style of the widget. This method should be used only by the M_VIEW macro.
     */
    MWidgetStyleContainer &style();
    const MWidgetStyleContainer &style() const;

protected:
    MWidgetViewPrivate *const d_ptr;

    MWidgetView(MWidgetViewPrivate *dd);
    MWidgetView(MWidgetViewPrivate &dd, MWidgetController *controller);
private:
    Q_DECLARE_PRIVATE(MWidgetView)
    Q_DISABLE_COPY(MWidgetView)
};

#endif
