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

#ifndef MWIDGETCONTROLLER_STUB
#define MWIDGETCONTROLLER_STUB

#include "mwidgetcontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
class MWidgetControllerStub : public StubBase {
  public:
  virtual void MWidgetControllerConstructor(QGraphicsItem *parent);
  virtual void MWidgetControllerConstructor(MWidgetModel *model, QGraphicsItem *parent);
  virtual void MWidgetControllerDestructor();
  virtual void setModel(MWidgetModel *model);
  virtual MWidgetModel * model();
  virtual const MWidgetModel * model() const;
  virtual MTheme::ViewType viewType() const;
  virtual bool isActive() const;
  virtual void setView(MWidgetView *view);
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  virtual void setGeometry(const QRectF &rect);
  virtual QRectF boundingRect() const;
  virtual QPainterPath shape() const;
  virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
  virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
  virtual void changeEvent(QEvent *event);
  virtual void setViewType(const MTheme::ViewType &type);
  virtual void setActive(bool active);
  virtual void setObjectName(const QString &name);
  virtual void updateData(const QList<const char *> &modifications);
  virtual void setupModel();
  virtual void updateMicroFocus();
  virtual void setStyleName(const QString &name);
  virtual const QString& styleName()const;
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void cancelEvent(MCancelEvent *event);
  virtual void orientationChangeEvent(MOrientationChangeEvent *event);
  virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
  virtual void panGestureEvent(QGestureEvent *event, QPanGesture* gesture);
  virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);
  virtual void tapGestureEvent(QGestureEvent *event, QTapGesture* gesture);
  virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);
  virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
  virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
  virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
  virtual MWidgetStyleContainer & style();
  virtual const MWidgetStyleContainer & style() const;
  virtual void MWidgetControllerConstructor(MWidgetControllerPrivate *dd, MWidgetModel *model, QGraphicsItem *parent);
  virtual const MWidgetView * view() const;
}; 

// 2. IMPLEMENT STUB
void MWidgetControllerStub::MWidgetControllerConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void MWidgetControllerStub::MWidgetControllerConstructor(MWidgetModel *model, QGraphicsItem *parent) {
  Q_UNUSED(model);
  Q_UNUSED(parent);

}
void MWidgetControllerStub::MWidgetControllerDestructor() {

}

void MWidgetControllerStub::setModel(MWidgetModel *model) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MWidgetModel * >(model));
  stubMethodEntered("setModel",params);
}

MWidgetModel * MWidgetControllerStub::model() {
  stubMethodEntered("model");
  return stubReturnValue<MWidgetModel *>("model");
}

const MWidgetModel * MWidgetControllerStub::model() const {
  stubMethodEntered("model");
  return stubReturnValue<const MWidgetModel *>("model");
}

MTheme::ViewType MWidgetControllerStub::viewType() const {
  stubMethodEntered("viewType");
  return stubReturnValue<MTheme::ViewType>("viewType");
}

bool MWidgetControllerStub::isActive() const {
  stubMethodEntered("isActive");
  return stubReturnValue<bool>("isActive");
}

void MWidgetControllerStub::setView(MWidgetView *view) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MWidgetView * >(view));
  stubMethodEntered("setView",params);
}

void MWidgetControllerStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QPainter * >(painter));
  params.append( new Parameter<const QStyleOptionGraphicsItem * >(option));
  params.append( new Parameter<QWidget * >(widget));
  stubMethodEntered("paint",params);
}

void MWidgetControllerStub::setGeometry(const QRectF &rect) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QRectF & >(rect));
  stubMethodEntered("setGeometry",params);
}

QRectF MWidgetControllerStub::boundingRect() const {
  stubMethodEntered("boundingRect");
  return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath MWidgetControllerStub::shape() const {
  stubMethodEntered("shape");
  return stubReturnValue<QPainterPath>("shape");
}

QSizeF MWidgetControllerStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::SizeHint >(which));
  params.append( new Parameter<const QSizeF & >(constraint));
  stubMethodEntered("sizeHint",params);
  return stubReturnValue<QSizeF>("sizeHint");
}

void MWidgetControllerStub::resizeEvent(QGraphicsSceneResizeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneResizeEvent * >(event));
  stubMethodEntered("resizeEvent",params);
}

void MWidgetControllerStub::changeEvent(QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("changeEvent",params);
}

void MWidgetControllerStub::setViewType(const MTheme::ViewType &type) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MTheme::ViewType & >(type));
  stubMethodEntered("setViewType",params);
}

void MWidgetControllerStub::setActive(bool active) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(active));
  stubMethodEntered("setActive",params);
}

void MWidgetControllerStub::setObjectName(const QString &name) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(name));
  stubMethodEntered("setObjectName",params);
}

void MWidgetControllerStub::updateData(const QList<const char *> &modifications) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<const char *> & >(modifications));
  stubMethodEntered("updateData",params);
}

void MWidgetControllerStub::setupModel() {
  stubMethodEntered("setupModel");
}

void MWidgetControllerStub::updateMicroFocus() {
  stubMethodEntered("updateMicroFocus");
}

void MWidgetControllerStub::setStyleName(const QString &name){
    QList<ParameterBase*> params;
    params.append(new Parameter<QString>(name));
    stubMethodEntered("setStyleName",params);
}

const QString & MWidgetControllerStub::styleName() const{
    stubMethodEntered("styleName");
    return *stubReturnValue<QString *>("styleName");
}

void MWidgetControllerStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void MWidgetControllerStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseReleaseEvent",params);
}

void MWidgetControllerStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseMoveEvent",params);
}

void MWidgetControllerStub::cancelEvent(MCancelEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MCancelEvent * >(event));
  stubMethodEntered("cancelEvent",params);
}

void MWidgetControllerStub::orientationChangeEvent(MOrientationChangeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MOrientationChangeEvent * >(event));
  stubMethodEntered("orientationChangeEvent",params);
}

void MWidgetControllerStub::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapAndHoldGesture * >(gesture));
  stubMethodEntered("tapAndHoldGesture",params);
}

void MWidgetControllerStub::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPanGesture * >(gesture));
  stubMethodEntered("panGesture",params);
}

void MWidgetControllerStub::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPinchGesture * >(gesture));
  stubMethodEntered("pinchGesture",params);
}

void MWidgetControllerStub::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapGesture * >(gesture));
  stubMethodEntered("tapGesture",params);
}

void MWidgetControllerStub::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QSwipeGesture * >(gesture));
  stubMethodEntered("swipeGesture",params);
}

bool MWidgetControllerStub::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsItem * >(watched));
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("sceneEventFilter",params);
  return stubReturnValue<bool>("sceneEventFilter");
}

QVariant MWidgetControllerStub::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsItem::GraphicsItemChange >(change));
  params.append( new Parameter<const QVariant & >(value));
  stubMethodEntered("itemChange",params);
  return stubReturnValue<QVariant>("itemChange");
}

QVariant MWidgetControllerStub::inputMethodQuery(Qt::InputMethodQuery query) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::InputMethodQuery >(query));
  stubMethodEntered("inputMethodQuery",params);
  return stubReturnValue<QVariant>("inputMethodQuery");
}

MWidgetStyleContainer & MWidgetControllerStub::style() {
  stubMethodEntered("style");
  return *stubReturnValue<MWidgetStyleContainer *>("style");
}

const MWidgetStyleContainer & MWidgetControllerStub::style() const {
  stubMethodEntered("style");
  return *stubReturnValue<const MWidgetStyleContainer *>("style");
}

void MWidgetControllerStub::MWidgetControllerConstructor(MWidgetControllerPrivate *dd, MWidgetModel *model, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);

}
const MWidgetView * MWidgetControllerStub::view() const {
  stubMethodEntered("view");
  return stubReturnValue<const MWidgetView *>("view");
}



// 3. CREATE A STUB INSTANCE
MWidgetControllerStub gDefaultMWidgetControllerStub;
MWidgetControllerStub* gMWidgetControllerStub = &gDefaultMWidgetControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWidgetController::MWidgetController(QGraphicsItem *parent) {
  gMWidgetControllerStub->MWidgetControllerConstructor(parent);
}

MWidgetController::MWidgetController(MWidgetModel *model, QGraphicsItem *parent) {
  gMWidgetControllerStub->MWidgetControllerConstructor(model, parent);
}

MWidgetController::~MWidgetController() {
  gMWidgetControllerStub->MWidgetControllerDestructor();
}

void MWidgetController::setModel(MWidgetModel *model) {
  gMWidgetControllerStub->setModel(model);
}

MWidgetModel * MWidgetController::model() {
  return gMWidgetControllerStub->model();
}

const MWidgetModel * MWidgetController::model() const {
  return gMWidgetControllerStub->model();
}

MTheme::ViewType MWidgetController::viewType() const {
  return gMWidgetControllerStub->viewType();
}

bool MWidgetController::isActive() const {
  return gMWidgetControllerStub->isActive();
}

void MWidgetController::setView(MWidgetView *view) {
  gMWidgetControllerStub->setView(view);
}

void MWidgetController::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  gMWidgetControllerStub->paint(painter, option, widget);
}

void MWidgetController::setGeometry(const QRectF &rect) {
  gMWidgetControllerStub->setGeometry(rect);
}

QRectF MWidgetController::boundingRect() const {
  return gMWidgetControllerStub->boundingRect();
}

QPainterPath MWidgetController::shape() const {
  return gMWidgetControllerStub->shape();
}

QSizeF MWidgetController::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
  return gMWidgetControllerStub->sizeHint(which, constraint);
}

void MWidgetController::resizeEvent(QGraphicsSceneResizeEvent *event) {
  gMWidgetControllerStub->resizeEvent(event);
}

void MWidgetController::changeEvent(QEvent *event) {
  gMWidgetControllerStub->changeEvent(event);
}

void MWidgetController::setViewType(const MTheme::ViewType &type) {
  gMWidgetControllerStub->setViewType(type);
}

void MWidgetController::setActive(bool active) {
  gMWidgetControllerStub->setActive(active);
}

void MWidgetController::setStyleName(const QString &name) {
  gMWidgetControllerStub->setStyleName(name);
}
void MWidgetController::setObjectName(const QString &name) {
  gMWidgetControllerStub->setObjectName(name);
}

void MWidgetController::updateData(const QList<const char *> &modifications) {
  gMWidgetControllerStub->updateData(modifications);
}

void MWidgetController::setupModel() {
  gMWidgetControllerStub->setupModel();
}

void MWidgetController::updateMicroFocus() {
  gMWidgetControllerStub->updateMicroFocus();
}

void MWidgetController::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gMWidgetControllerStub->mousePressEvent(event);
}

void MWidgetController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  gMWidgetControllerStub->mouseReleaseEvent(event);
}

void MWidgetController::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  gMWidgetControllerStub->mouseMoveEvent(event);
}

void MWidgetController::cancelEvent(MCancelEvent *event) {
  gMWidgetControllerStub->cancelEvent(event);
}

void MWidgetController::orientationChangeEvent(MOrientationChangeEvent *event) {
  gMWidgetControllerStub->orientationChangeEvent(event);
}

void MWidgetController::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  gMWidgetControllerStub->tapAndHoldGestureEvent(event,gesture);
}

void MWidgetController::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  gMWidgetControllerStub->panGestureEvent(event,gesture);
}

void MWidgetController::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  gMWidgetControllerStub->pinchGestureEvent(event,gesture);
}

void MWidgetController::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  gMWidgetControllerStub->tapGestureEvent(event,gesture);
}

void MWidgetController::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  gMWidgetControllerStub->swipeGestureEvent(event, gesture);
}

bool MWidgetController::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
  return gMWidgetControllerStub->sceneEventFilter(watched, event);
}

QVariant MWidgetController::itemChange(GraphicsItemChange change, const QVariant &value) {
  return gMWidgetControllerStub->itemChange(change, value);
}

QVariant MWidgetController::inputMethodQuery(Qt::InputMethodQuery query) const {
  return gMWidgetControllerStub->inputMethodQuery(query);
}

MWidgetStyleContainer & MWidgetController::style() {
  return gMWidgetControllerStub->style();
}

const QString& MWidgetController::styleName() const{
  return gMWidgetControllerStub->styleName();
}

const MWidgetStyleContainer & MWidgetController::style() const {
  return gMWidgetControllerStub->style();
}

MWidgetController::MWidgetController(MWidgetControllerPrivate *dd, MWidgetModel *model, QGraphicsItem *parent) {
  gMWidgetControllerStub->MWidgetControllerConstructor(dd, model, parent);
}

const MWidgetView * MWidgetController::view() const {
  return gMWidgetControllerStub->view();
}
#endif
