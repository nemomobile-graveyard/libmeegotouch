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

#ifndef MWIDGETCONTROLLER_H
#define MWIDGETCONTROLLER_H

#include <mwidget.h>
#include <mtheme.h>

class MWidgetModel;
class MWidgetView;
class MWidgetControllerPrivate;
class MWidgetStyleContainer;
class MAbstractWidgetAnimation;

#define M_CONTROLLER(CLASS) \
    public: \
    inline CLASS##Model* model() { return static_cast<CLASS##Model*>(MWidgetController::model()); } \
    inline const CLASS##Model* model() const { return static_cast<const CLASS##Model*>(MWidgetController::model()); }

/*!
  \class MWidgetController
  \brief MWidgetController is the base class for a controller in the MVC widget model.

  MWidgetController is the base class of all components that implement the Model-View-Controller pattern for widgets.
  In a widget, the controller serves as the public interface to the application developer. The controller internally
  stores the widget's state in the model and delegates painting and event handling to the view.

  Although the controller provides methods to set the view and model components, widgets derived from MWidgetController
  always provide an already initialised model while a view is constructed at the time it is needed unless otherwise explicitly
  set.

*/

class M_CORE_EXPORT MWidgetController : public MWidget
{
    Q_OBJECT

    Q_PROPERTY(MTheme::ViewType viewType READ viewType WRITE setViewType)
    Q_PROPERTY(bool active READ isActive WRITE setActive)
    Q_PROPERTY(QString styleName READ styleName WRITE setStyleName)

public:
    /*!
     Default widget view type.

     \sa viewType(), setViewType()
     */
    static const MTheme::ViewType defaultType;

    /*!
     Creates a new MWidgetController with the given \a parent.
     */
    explicit MWidgetController(QGraphicsItem *parent = 0);

    /*!
     Creates a new MWidgetController with the given \a model and \a parent.

     If \a model is 0, a MWidgetModel will be created.
     */
    explicit MWidgetController(MWidgetModel *model, QGraphicsItem *parent = 0);

    /*!
     Destroys the controller.
     */
    virtual ~MWidgetController();

    /*!
     Sets the \a model of the widget.

     This function will increase the reference count on the given model. If a model was already set
     prior to calling this function, that model's reference count will be decreased and the model
     destroyed if it reaches 0.

     \sa model()
     */
    void setModel(MWidgetModel *model);

    /*!
     Returns the model of the widget.

     This function will always return a valid model. The model should \a not be directly manipulated
     in the application code, unless the application also explicitly creates the model. Instead, the widget
     should provide the necessary functions in the controller class.

     \sa setModel()
    */
    MWidgetModel *model();

    /*!
     Returns a const pointer to the model of the widget.

     This function will always return a valid model.

     \sa setModel()
    */
    const MWidgetModel *model() const;

    M::Position layoutPosition() const;
    void setLayoutPosition(M::Position layoutPosition);

    /*!
     Returns the type of the widget's view.

     \sa setViewType()
     */
    MTheme::ViewType viewType() const;

    /*!
     Indicates whether this widget is an active state.

     \sa setActive() MWidgetView::setActive()
     */
    bool isActive() const;

    /*!
     Sets the \a view of the widget.

     \sa view()
     */
    virtual void setView(MWidgetView *view);

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

    /*!
      Returns the style name of the widget.
     */
    const QString &styleName() const;

public Q_SLOTS:
    /*!
     Set the view type of the widget.

     The \a type determines which MWidgetView the theme system will select at runtime.

     Example:
     \code
     MButton button;
     button.setViewType(MButton::toggleType);
     \endcode
     A button of the "toggle" type may for example look and behave like a checkbox or a slider, depending on the selected theme.

     \sa viewType()
     */
    void setViewType(const MTheme::ViewType &type);

    /*!
     Set the \a active state of the widget

     Active state is communicated to the widget view and can affect how the widget is
     presented. By default widgets are inactive. What active state semantically means
     is up the individual widget.

     \see isActive() MWidgetView::setActive()
     */
    virtual void setActive(bool active);

    /*!
     Set the objectName to \a name

     This function sets the QObject::objectName property of the widget and reloads the
     style of the widget view. The object name is used by the theme system to select a style with
     a matching ID.

     Warning: Since QObject::setObjectName() is non-virtual, code like:
     \code
       MLabel *label = new MLabel("Hello");
       label->setObjectName("hello");
     \endcode
     Would correctly restyle the label.  But:
     \code
       QGraphicsWidget *label = new MLabel("Hello");
       label->setObjectName("hello");
     \endcode
     Would not update the CSS style correctly.

     Note that multiple objects can have the same name, and consequently the same style.

     \deprecated Use setStyleName() to set the style name of the widget.
    */
    virtual void setObjectName(const QString &name);

    /*!
      Set the style name to \a name.

      This function sets the style name property of the widget to \a name and reloads the
      style of the widget view. The style name is used to select a style with a matching
      name from the theme.

      An example of setting the object name:
      \code
        mywidget->setStyleName("warning");
      \endcode
      In a CSS file you can then specify a style for this particular widget:
      \code
        #warning { background-color: #FF0000; }
      \endcode

      \sa styleName()
    */
    void setStyleName(const QString &name);

protected Q_SLOTS:
    /*!
       Notification of model data modifications.

       This function is called when some members of the widget model have been modified.
     */
    virtual void updateData(const QList<const char *>& modifications);

    /*!
      Notification of model having changed.

      This function is called when the model of the widget is initially set or when later replaced.
      You can reimplement this method to synchronize your widget's internal state with that of
      the new model.

      An example would be an widget that creates a child label widget; on setupModel() it
      should synchronize the label text with the model data. Subsequent updates to the model data
      and the resulting changes to the label are delivered through updateData().

      \note The widget uses the base implementation of this function to set up notification
      of changes in the model. Because of this, it is very important that subclasses
      call the base implementation.

      \sa setModel()
      \sa updateData()
     */
    virtual void setupModel();

    /*!
     Updates the widget's micro focus.

     \sa QWidget::updateMicroFocus.
     */
    void updateMicroFocus();

protected:
    /*!
     MWidgetController's implementation of mousePressEvent() forwards the call to the view.

     \sa MWidgetView::mousePressEvent()
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /*!
     MWidgetController's implementation of mouseReleaseEvent() forwards the call to the view.

     \sa MWidgetView::mouseReleaseEvent()
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /*!
     MWidgetController's implementation of mouseMoveEvent() forwards the call to the view.

     \sa MWidgetView::mouseMoveEvent()
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /*!
     MWidgetController's implementation of cancelEvent() forwards the call to the view.

     \sa MWidgetView::cancelEvent()
     */
    virtual void cancelEvent(MCancelEvent *event);

    /*!
     MWidgetController's implementation of orientationChangeEvent() forwards the call to the view.

     \sa MWidgetView::orientationChangeEvent()
     */
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);

    /*!
     MWidgetController's implementation of tapAndHoldGestureEvent() forwards the call to the view.

     \sa MWidgetView::tapAndHoldGestureEvent()
     */
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);

    /*!
     MWidgetController's implementation of panGestureEvent() forwards the call to the view.

     \sa MWidgetView::panGestureEvent()
     */
    virtual void panGestureEvent(QGestureEvent *event, QPanGesture* gesture);

    /*!
     MWidgetController's implementation of pinchGestureEvent() forwards the call to the view.

     \sa MWidgetView::pinchGestureEvent()
     */
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);

    /*!
     MWidgetController's implementation of tapGestureEvent() forwards the call to the view.

     \sa MWidgetView::tapGestureEvent()
     */
    virtual void tapGestureEvent(QGestureEvent *event, QTapGesture* gesture);

    /*!
     MWidgetController's implementation of swipeGestureEvent() forwards the call to the view.

     \sa MWidgetView::swipeGestureEvent()
     */
    virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);

    /*!
     MWidgetController's implementation of sceneEventFilter() forwards the call to the view.

     \sa MWidgetView::sceneEventFilter()
     */
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

    /*!
     MWidgetController's implementation of itemChange() forwards the call to the view.

     \sa MWidgetView::itemChange()
     */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    /*!
     MWidgetController's implementation of inputMethodQuery() forwards the call to the view.

     \sa MWidgetView::inputMethodQuery()
     */
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;

    /*!
     Returns the style of the widget.
     */
    MWidgetStyleContainer &style();

    /*!
     Returns a constant reference to the style of the widget.
     */
    const MWidgetStyleContainer &style() const;

    /*! \internal
     Constructor for internal libmeegotouch use only.
     */
    explicit MWidgetController(MWidgetControllerPrivate *dd, MWidgetModel *model, QGraphicsItem *parent);
    //! \internal_end

    /*!
     Returns a constant pointer to the view of the widget.

     \sa setView()
     */
    const MWidgetView *view() const;

private:
    Q_DECLARE_PRIVATE(MWidgetController)
    Q_DISABLE_COPY(MWidgetController)

    friend class MTheme;
    friend class MWidgetView;
    friend class MWidgetViewPrivate;

#ifdef UNIT_TEST
    friend class Ut_MWidgetController;
    friend class Ut_AllWidgets;
    friend class Ut_MButton;
    friend class Ut_MOverlay;
    friend class Ut_MPannableViewport;
    friend class Ut_MGrid;
#endif

};

#endif
