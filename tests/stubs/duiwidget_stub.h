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

#ifndef DUIWIDGET_STUB
#define DUIWIDGET_STUB

#include "duiwidget.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiWidgetStub : public StubBase {
  public:
  virtual void DuiWidgetConstructor(QGraphicsItem *parent);
  virtual void DuiWidgetDestructor();
  virtual QPainterPath shape() const;
  virtual void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  virtual DuiSceneManager * sceneManager() const;
  virtual bool isOnDisplay() const;
  virtual bool isSelected() const;
  virtual void setVisible(bool visible);
  virtual void setObjectName(const QString &name);
  virtual void setSelected(bool selected);
  virtual void clearActions();
  virtual void enterDisplayEvent();
  virtual void exitDisplayEvent();
  virtual void onDisplayChangeEvent(DuiOnDisplayChangeEvent *event);
  virtual bool event(QEvent *event);
  virtual void cancelEvent(DuiCancelEvent *event);
  virtual void orientationChangeEvent(DuiOrientationChangeEvent *event);
  virtual void actionEvent(QActionEvent *event);
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
  virtual void retranslateUi();
  virtual void gestureEvent(QGestureEvent *event);
  virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
  virtual void panGestureEvent(QGestureEvent *event, QPanGesture* gesture);
  virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);
  virtual void tapGestureEvent(QGestureEvent *event, QTapGesture* gesture);
  virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);
  virtual void DuiWidgetConstructor(DuiWidgetPrivate &dd, QGraphicsItem *parent);
};

// 2. IMPLEMENT STUB
void DuiWidgetStub::DuiWidgetConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void DuiWidgetStub::DuiWidgetDestructor() {

}
QPainterPath DuiWidgetStub::shape() const {
  stubMethodEntered("shape");
  return stubReturnValue<QPainterPath>("shape");
}

void DuiWidgetStub::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QPainter * >(painter));
  params.append( new Parameter<const QStyleOptionGraphicsItem * >(option));
  params.append( new Parameter<QWidget * >(widget));
  stubMethodEntered("paintWindowFrame",params);
}

DuiSceneManager * DuiWidgetStub::sceneManager() const {
  stubMethodEntered("sceneManager");
  return stubReturnValue<DuiSceneManager *>("sceneManager");
}

bool DuiWidgetStub::isOnDisplay() const {
  stubMethodEntered("isOnDisplay");
  return stubReturnValue<bool>("isOnDisplay");
}

bool DuiWidgetStub::isSelected() const {
  stubMethodEntered("isSelected");
  return stubReturnValue<bool>("isSelected");
}

void DuiWidgetStub::setVisible(bool visible) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(visible));
  stubMethodEntered("setVisible",params);
}

void DuiWidgetStub::setObjectName(const QString &name) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(name));
  stubMethodEntered("setObjectName",params);
}

void DuiWidgetStub::setSelected(bool selected) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(selected));
  stubMethodEntered("setSelected",params);
}

void DuiWidgetStub::clearActions() {
  stubMethodEntered("clearActions");
}

void DuiWidgetStub::enterDisplayEvent() {
  stubMethodEntered("enterDisplayEvent");
}

void DuiWidgetStub::exitDisplayEvent() {
  stubMethodEntered("exitDisplayEvent");
}

void DuiWidgetStub::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiOnDisplayChangeEvent * >(event));
  stubMethodEntered("onDisplayChangeEvent",params);
}

bool DuiWidgetStub::event(QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("event",params);
  return stubReturnValue<bool>("event");
}

void DuiWidgetStub::cancelEvent(DuiCancelEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiCancelEvent * >(event));
  stubMethodEntered("cancelEvent",params);
}

void DuiWidgetStub::orientationChangeEvent(DuiOrientationChangeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiOrientationChangeEvent * >(event));
  stubMethodEntered("orientationChangeEvent",params);
}

void DuiWidgetStub::actionEvent(QActionEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QActionEvent *>(event));
  stubMethodEntered("actionEvent",params);
}

void DuiWidgetStub::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneContextMenuEvent * >(event));
  stubMethodEntered("contextMenuEvent",params);
}

void DuiWidgetStub::retranslateUi() {
  stubMethodEntered("retranslateUi");
}

void DuiWidgetStub::DuiWidgetConstructor(DuiWidgetPrivate &dd, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(parent);

}

void DuiWidgetStub::gestureEvent(QGestureEvent *event)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  stubMethodEntered("gestureEvent",params);
}

void DuiWidgetStub::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapAndHoldGesture * >(gesture));
  stubMethodEntered("tapAndHoldGesture",params);
}

void DuiWidgetStub::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPanGesture * >(gesture));
  stubMethodEntered("panGesture",params);
}

void DuiWidgetStub::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPinchGesture * >(gesture));
  stubMethodEntered("pinchGesture",params);
}

void DuiWidgetStub::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{ 
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapGesture * >(gesture));
  stubMethodEntered("tapGesture",params);
}

void DuiWidgetStub::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QSwipeGesture * >(gesture));
  stubMethodEntered("swipeGesture",params);
}

// 3. CREATE A STUB INSTANCE
DuiWidgetStub gDefaultDuiWidgetStub;
DuiWidgetStub* gDuiWidgetStub = &gDefaultDuiWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidget::DuiWidget(QGraphicsItem *parent) :
        d_ptr(NULL)
{
  gDuiWidgetStub->DuiWidgetConstructor(parent);
}

DuiWidget::~DuiWidget() {
  gDuiWidgetStub->DuiWidgetDestructor();
}

QPainterPath DuiWidget::shape() const {
  return gDuiWidgetStub->shape();
}

void DuiWidget::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  gDuiWidgetStub->paintWindowFrame(painter, option, widget);
}

DuiSceneManager * DuiWidget::sceneManager() const {
  return gDuiWidgetStub->sceneManager();
}

bool DuiWidget::isOnDisplay() const {
  return gDuiWidgetStub->isOnDisplay();
}

bool DuiWidget::isSelected() const {
  return gDuiWidgetStub->isSelected();
}

void DuiWidget::setVisible(bool visible) {
  gDuiWidgetStub->setVisible(visible);
}

void DuiWidget::setObjectName(const QString &name) {
  gDuiWidgetStub->setObjectName(name);
}

void DuiWidget::setSelected(bool selected) {
  gDuiWidgetStub->setSelected(selected);
}

void DuiWidget::clearActions() {
  gDuiWidgetStub->clearActions();
}

void DuiWidget::enterDisplayEvent() {
  gDuiWidgetStub->enterDisplayEvent();
}

void DuiWidget::exitDisplayEvent() {
  gDuiWidgetStub->exitDisplayEvent();
}

void DuiWidget::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event) {
  gDuiWidgetStub->onDisplayChangeEvent(event);
}

bool DuiWidget::event(QEvent *event) {
  return gDuiWidgetStub->event(event);
}

void DuiWidget::cancelEvent(DuiCancelEvent *event) {
  gDuiWidgetStub->cancelEvent(event);
}

void DuiWidget::orientationChangeEvent(DuiOrientationChangeEvent *event) {
  gDuiWidgetStub->orientationChangeEvent(event);
}

void DuiWidget::actionEvent(QActionEvent *event) {
  gDuiWidgetStub->actionEvent(event);
}

void DuiWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  gDuiWidgetStub->contextMenuEvent(event);
}

void DuiWidget::retranslateUi() {
  gDuiWidgetStub->retranslateUi();
}

DuiWidget::DuiWidget(DuiWidgetPrivate &dd, QGraphicsItem *parent) :
        d_ptr(NULL)
{
  gDuiWidgetStub->DuiWidgetConstructor(dd, parent);
}

void DuiWidget::gestureEvent(QGestureEvent *event)
{
  gDuiWidgetStub->gestureEvent(event);
}

void DuiWidget::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  gDuiWidgetStub->tapAndHoldGestureEvent(event,gesture);
}

void DuiWidget::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  gDuiWidgetStub->panGestureEvent(event,gesture);
}

void DuiWidget::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  gDuiWidgetStub->pinchGestureEvent(event,gesture);
}

void DuiWidget::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  gDuiWidgetStub->tapGestureEvent(event,gesture);
}

void DuiWidget::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  gDuiWidgetStub->swipeGestureEvent(event, gesture);
}

#endif
