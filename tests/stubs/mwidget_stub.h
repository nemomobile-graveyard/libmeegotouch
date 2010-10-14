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

#ifndef MWIDGET_STUB
#define MWIDGET_STUB

#include "mwidget.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MWidgetStub : public StubBase {
  public:
  virtual void MWidgetConstructor(QGraphicsItem *parent);
  virtual void MWidgetDestructor();
  virtual QPainterPath shape() const;
  virtual void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  virtual MSceneManager * sceneManager() const;
  virtual bool isOnDisplay() const;
  virtual bool isSelected() const;
  virtual QPointF paintOffset() const;
  virtual void setPaintOffset(const QPointF & offset);
  virtual void setVisible(bool visible);
  virtual void setObjectName(const QString &name);
  virtual void setSelected(bool selected);
  virtual void clearActions();
  virtual void enterDisplayEvent();
  virtual void exitDisplayEvent();
  virtual void onDisplayChangeEvent(MOnDisplayChangeEvent *event);
  virtual bool event(QEvent *event);
  virtual bool sceneEvent(QEvent *event);
  virtual void cancelEvent(MCancelEvent *event);
  virtual void orientationChangeEvent(MOrientationChangeEvent *event);
  virtual void actionEvent(QActionEvent *event);
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
  virtual void retranslateUi();
  virtual void gestureEvent(QGestureEvent *event);
  virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
  virtual void panGestureEvent(QGestureEvent *event, QPanGesture* gesture);
  virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);
  virtual void tapGestureEvent(QGestureEvent *event, QTapGesture* gesture);
  virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);
  virtual void MWidgetConstructor(MWidgetPrivate &dd, QGraphicsItem *parent);
};

// 2. IMPLEMENT STUB
void MWidgetStub::MWidgetConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void MWidgetStub::MWidgetDestructor() {

}
QPainterPath MWidgetStub::shape() const {
  stubMethodEntered("shape");
  return stubReturnValue<QPainterPath>("shape");
}

void MWidgetStub::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QPainter * >(painter));
  params.append( new Parameter<const QStyleOptionGraphicsItem * >(option));
  params.append( new Parameter<QWidget * >(widget));
  stubMethodEntered("paintWindowFrame",params);
}

MSceneManager * MWidgetStub::sceneManager() const {
  stubMethodEntered("sceneManager");
  return stubReturnValue<MSceneManager *>("sceneManager");
}

bool MWidgetStub::isOnDisplay() const {
  stubMethodEntered("isOnDisplay");
  return stubReturnValue<bool>("isOnDisplay");
}

bool MWidgetStub::isSelected() const {
  stubMethodEntered("isSelected");
  return stubReturnValue<bool>("isSelected");
}

QPointF MWidgetStub::paintOffset() const {
  stubMethodEntered("paintOffset");
  return stubReturnValue<QPointF>("paintOffset");
}

void MWidgetStub::setPaintOffset(const QPointF & offset) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QPointF & >(offset));
  stubMethodEntered("setPaintOffset",params);
}

void MWidgetStub::setVisible(bool visible) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(visible));
  stubMethodEntered("setVisible",params);
}

void MWidgetStub::setObjectName(const QString &name) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(name));
  stubMethodEntered("setObjectName",params);
}

void MWidgetStub::setSelected(bool selected) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(selected));
  stubMethodEntered("setSelected",params);
}

void MWidgetStub::clearActions() {
  stubMethodEntered("clearActions");
}

void MWidgetStub::enterDisplayEvent() {
  stubMethodEntered("enterDisplayEvent");
}

void MWidgetStub::exitDisplayEvent() {
  stubMethodEntered("exitDisplayEvent");
}

void MWidgetStub::onDisplayChangeEvent(MOnDisplayChangeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MOnDisplayChangeEvent * >(event));
  stubMethodEntered("onDisplayChangeEvent",params);
}

bool MWidgetStub::event(QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("event",params);
  return stubReturnValue<bool>("event");
}

bool MWidgetStub::sceneEvent(QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("sceneEvent",params);
  return stubReturnValue<bool>("sceneEvent");
}

void MWidgetStub::cancelEvent(MCancelEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MCancelEvent * >(event));
  stubMethodEntered("cancelEvent",params);
}

void MWidgetStub::orientationChangeEvent(MOrientationChangeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MOrientationChangeEvent * >(event));
  stubMethodEntered("orientationChangeEvent",params);
}

void MWidgetStub::actionEvent(QActionEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QActionEvent *>(event));
  stubMethodEntered("actionEvent",params);
}

void MWidgetStub::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneContextMenuEvent * >(event));
  stubMethodEntered("contextMenuEvent",params);
}

void MWidgetStub::retranslateUi() {
  stubMethodEntered("retranslateUi");
}

void MWidgetStub::MWidgetConstructor(MWidgetPrivate &dd, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(parent);

}

void MWidgetStub::gestureEvent(QGestureEvent *event)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  stubMethodEntered("gestureEvent",params);
}

void MWidgetStub::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapAndHoldGesture * >(gesture));
  stubMethodEntered("tapAndHoldGestureEvent",params);
}

void MWidgetStub::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPanGesture * >(gesture));
  stubMethodEntered("panGestureEvent",params);
}

void MWidgetStub::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPinchGesture * >(gesture));
  stubMethodEntered("pinchGestureEvent",params);
}

void MWidgetStub::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{ 
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapGesture * >(gesture));
  stubMethodEntered("tapGestureEvent",params);
}

void MWidgetStub::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QSwipeGesture * >(gesture));
  stubMethodEntered("swipeGestureEvent",params);
}

// 3. CREATE A STUB INSTANCE
MWidgetStub gDefaultMWidgetStub;
MWidgetStub* gMWidgetStub = &gDefaultMWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWidget::MWidget(QGraphicsItem *parent) :
        d_ptr(NULL)
{
  gMWidgetStub->MWidgetConstructor(parent);
}

MWidget::~MWidget() {
  gMWidgetStub->MWidgetDestructor();
}

QPainterPath MWidget::shape() const {
  return gMWidgetStub->shape();
}

void MWidget::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  gMWidgetStub->paintWindowFrame(painter, option, widget);
}

MSceneManager * MWidget::sceneManager() const {
  return gMWidgetStub->sceneManager();
}

bool MWidget::isOnDisplay() const {
  return gMWidgetStub->isOnDisplay();
}

bool MWidget::isSelected() const {
  return gMWidgetStub->isSelected();
}

QPointF MWidget::paintOffset() const {
    return gMWidgetStub->paintOffset();
}

void MWidget::setPaintOffset(const QPointF & offset) {
    gMWidgetStub->setPaintOffset(offset);
}

void MWidget::setVisible(bool visible) {
  gMWidgetStub->setVisible(visible);
}

void MWidget::setObjectName(const QString &name) {
  gMWidgetStub->setObjectName(name);
}

void MWidget::setSelected(bool selected) {
  gMWidgetStub->setSelected(selected);
}

void MWidget::clearActions() {
  gMWidgetStub->clearActions();
}

void MWidget::enterDisplayEvent() {
  gMWidgetStub->enterDisplayEvent();
}

void MWidget::exitDisplayEvent() {
  gMWidgetStub->exitDisplayEvent();
}

void MWidget::onDisplayChangeEvent(MOnDisplayChangeEvent *event) {
  gMWidgetStub->onDisplayChangeEvent(event);
}

bool MWidget::event(QEvent *event) {
  return gMWidgetStub->event(event);
}

bool MWidget::sceneEvent(QEvent *event) {
  return gMWidgetStub->sceneEvent(event);
}

void MWidget::cancelEvent(MCancelEvent *event) {
  gMWidgetStub->cancelEvent(event);
}

void MWidget::orientationChangeEvent(MOrientationChangeEvent *event) {
  gMWidgetStub->orientationChangeEvent(event);
}

void MWidget::actionEvent(QActionEvent *event) {
  gMWidgetStub->actionEvent(event);
}

void MWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  gMWidgetStub->contextMenuEvent(event);
}

void MWidget::retranslateUi() {
  gMWidgetStub->retranslateUi();
}

MWidget::MWidget(MWidgetPrivate &dd, QGraphicsItem *parent) :
        d_ptr(NULL)
{
  gMWidgetStub->MWidgetConstructor(dd, parent);
}

void MWidget::gestureEvent(QGestureEvent *event)
{
  gMWidgetStub->gestureEvent(event);
}

void MWidget::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  gMWidgetStub->tapAndHoldGestureEvent(event,gesture);
}

void MWidget::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  gMWidgetStub->panGestureEvent(event,gesture);
}

void MWidget::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  gMWidgetStub->pinchGestureEvent(event,gesture);
}

void MWidget::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  gMWidgetStub->tapGestureEvent(event,gesture);
}

void MWidget::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  gMWidgetStub->swipeGestureEvent(event, gesture);
}

#endif
