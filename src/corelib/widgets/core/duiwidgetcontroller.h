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

#ifndef DUIWIDGETCONTROLLER_H
#define DUIWIDGETCONTROLLER_H

#include <duiwidget.h>
#include <duitheme.h>

class DuiWidgetModel;
class DuiWidgetView;
class DuiWidgetControllerPrivate;
class DuiWidgetStyleContainer;
class DuiAbstractWidgetAnimation;

#define DUI_CONTROLLER(CLASS) \
    public: \
    inline CLASS##Model* model() { return static_cast<CLASS##Model*>(DuiWidgetController::model()); } \
    inline const CLASS##Model* model() const { return static_cast<const CLASS##Model*>(DuiWidgetController::model()); }

/*!
  \class DuiWidgetController
  \brief DuiWidgetController is the base class for a controller in the MVC widget model.

  DuiWidgetController is the base class of all components that implement the Model-View-Controller pattern for widgets.
  In a widget, the controller serves as the public interface to the application developer. The controller internally
  stores the widget's state in the model and delegates painting and event handling to the view.

  Although the controller provides methods to set the view and model components, widgets derived from DuiWidgetController
  always provide an already initalised model while a view is constructed at the time it is needed unless otherwise explicitly
  set.

*/

class DUI_EXPORT DuiWidgetController : public DuiWidget
{
    Q_OBJECT

    Q_PROPERTY(DuiTheme::ViewType viewType READ viewType WRITE setViewType)
    Q_PROPERTY(bool active READ isActive WRITE setActive)

public:
    /*!
     Default widget view type.

     \sa viewType(), setViewType()
     */
    static const DuiTheme::ViewType defaultType;

    /*!
     Creates a new DuiWidgetController with the given \a parent.
     */
    explicit DuiWidgetController(QGraphicsItem *parent = 0);

    /*!
     Creates a new DuiWidgetController with the given \a model and \a parent.

     If \model is 0, a DuiWidgetModel will be created.
     */
    explicit DuiWidgetController(DuiWidgetModel *model, QGraphicsItem *parent = 0);

    /*!
     Destroys the controller.
     */
    virtual ~DuiWidgetController();

    /*!
        \brief Returns an animation which can be used for animating the widget visible state.

        This method can also return NULL, if there is no animation defined.
        This method doesn't give the ownership of the animation to the caller, so
        if you add this animation to a QAnimationGroup, be sure to remove it from there
        before the group deletes the animation.
     */
    DuiAbstractWidgetAnimation *showAnimation();

    /*!
        \brief Returns an animation which can be used from animating the widget to hidden state.

        This method can also return NULL, if there is no animation defined.
        This method doesn't give the ownership of the animation to the caller, so
        if you add this animation to a QAnimationGroup, be suer to remove it from there
        before the group deletes the animation.
     */
    DuiAbstractWidgetAnimation *hideAnimation();

    /*!
     Sets the \a model of the widget.

     This function will increase the reference count on the given model. If a model was already set
     prior to calling this function, that model's reference count will be decreased and the model
     destroyed if it reaches 0.

     \sa model()
     */
    void setModel(DuiWidgetModel *model);

    /*!
     Returns the model of the widget.

     This function will always return a valid model. The model should \a not be directly manipulated
     in the application code, unless the application also explicitly creates the model. Instead, the widget
     should provide the necessary functions in the controller class.

     \sa setModel()
    */
    DuiWidgetModel *model();

    /*!
     Returns a const pointer to the model of the widget.

     This function will always return a valid model.

     \sa setModel()
    */
    const DuiWidgetModel *model() const;

    /*!
     Returns the type of the widget's view.

     \sa setViewType()
     */
    DuiTheme::ViewType viewType() const;

    /*!
     Indicates whether this widget is an active state.

     \sa setActive() DuiWidgetView::setActive()
     */
    bool isActive() const;

    /*!
     Sets the \a view of the widget.

     \sa view()
     */
    virtual void setView(DuiWidgetView *view);

    //! \reimp
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = 0);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void changeEvent(QEvent *event);
    //! \reimp_end

public Q_SLOTS:
    /*!
     Set the view type of the widget.

     The \a type determines which DuiWidgetView the theme system will select at runtime.

     Example:
     \code
     DuiButton button;
     button.setViewType(DuiButton::toggleType);
     \endcode
     A button of the "toggle" type may for example look and behave like a checkbox or a slider, depending on the selected theme.

     \sa viewType()
     */
    void setViewType(const DuiTheme::ViewType &type);

    /*!
     Set the \a active state of the widget

     Active state is communicated to the widget view and can affect how the widget is
     presented. By default widgets are inactive. What active state semantically means
     is up the individual widget.

     \see isActive() DuiWidgetView::setActive()
     */
    virtual void setActive(bool active);

    /*!
     Set the objectName to \a name

     This function sets the QObject::objectName property of the widget and reloads the
     style of the widget view. The object name is used by the theme system to select a style with
     a matching ID.

     An example of setting the object name:
     \code
       mywidget->setObjectname("warning");
     \endcode
     In a CSS file you can then specify a style for this particular widget:
     \code
       #warning { background-color: #FF0000; }
     \endcode

     Note that multiple objects can have the same name, and consequently the same style.
    */
    virtual void setObjectName(const QString &name);

protected Q_SLOTS:
    /*!
       Notification of model data modifications.

       This function is called when some members of the widget model have been modified.
     */
    virtual void updateData(const QList<const char *>& modifications);

    /*!
      Notification of model having changed.

      This function is called when the model of the widget is replaced.

      \sa setModel()
     */
    virtual void setupModel();

    /*!
     Updates the widget's micro focus.

     \sa QWidget::updateMicroFocus.
     */
    void updateMicroFocus();

protected:
    /*!
     DuiWidgetController's implementation of mousePressEvent() forwards the call to the view.

     \sa DuiWidgetView::mousePressEvent()
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /*!
     DuiWidgetController's implementation of mouseReleaseEvent() forwards the call to the view.

     \sa DuiWidgetView::mouseReleaseEvent()
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /*!
     DuiWidgetController's implementation of mouseMoveEvent() forwards the call to the view.

     \sa DuiWidgetView::mouseMoveEvent()
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /*!
     DuiWidgetController's implementation of cancelEvent() forwards the call to the view.

     \sa DuiWidgetView::cancelEvent()
     */
    virtual void cancelEvent(DuiCancelEvent *event);

    /*!
     DuiWidgetController's implementation of orientationChangeEvent() forwards the call to the view.

     \sa DuiWidgetView::orientationChangeEvent()
     */
    virtual void orientationChangeEvent(DuiOrientationChangeEvent *event);

    /*!
     DuiWidgetController's implementation of tapAndHoldGestureEvent() forwards the call to the view.

     \sa DuiWidgetView::tapAndHoldGestureEvent()
     */
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);

    /*!
     DuiWidgetController's implementation of panGestureEvent() forwards the call to the view.

     \sa DuiWidgetView::panGestureEvent()
     */
    virtual void panGestureEvent(QGestureEvent *event, QPanGesture* gesture);

    /*!
     DuiWidgetController's implementation of pinchGestureEvent() forwards the call to the view.

     \sa DuiWidgetView::pinchGestureEvent()
     */
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);

    /*!
     DuiWidgetController's implementation of tapGestureEvent() forwards the call to the view.

     \sa DuiWidgetView::tapGestureEvent()
     */
    virtual void tapGestureEvent(QGestureEvent *event, QTapGesture* gesture);

    /*!
     DuiWidgetController's implementation of swipeGestureEvent() forwards the call to the view.

     \sa DuiWidgetView::swipeGestureEvent()
     */
    virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);

    /*!
     DuiWidgetController's implementation of sceneEventFilter() forwards the call to the view.

     \sa DuiWidgetView::sceneEventFilter()
     */
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    /*!
     DuiWidgetController's implementation of itemChange() forwards the call to the view.

     \sa DuiWidgetView::itemChange()
     */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    /*!
     DuiWidgetController's implementation of inputMethodQuery() forwards the call to the view.

     \sa DuiWidgetView::inputMethodQuery()
     */
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;

    /*!
     Returns the style of the widget.
     */
    DuiWidgetStyleContainer &style();

    /*!
     Returns a constant reference to the style of the widget.
     */
    const DuiWidgetStyleContainer &style() const;

    /*! \internal
     Constructor for internal libdui use only.
     */
    explicit DuiWidgetController(DuiWidgetControllerPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent);
    //! \internal_end

    /*!
     Returns a constant pointer to the view of the widget.

     \sa setView()
     */
    const DuiWidgetView *view() const;

private:
    Q_DECLARE_PRIVATE(DuiWidgetController)
    Q_DISABLE_COPY(DuiWidgetController)

    friend class DuiTheme;
    friend class DuiWidgetView;

#ifdef UNIT_TEST
    friend class Ut_DuiWidgetController;
    friend class Ut_DuiButton;
    friend class Ut_DuiOverlay;
    friend class Ut_DuiPannableViewport;
    friend class Ut_DuiGrid;
#endif

};

#endif
