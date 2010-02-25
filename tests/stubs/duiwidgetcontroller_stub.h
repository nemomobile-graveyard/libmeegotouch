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

#ifndef DUIWIDGETCONTROLLER_STUB
#define DUIWIDGETCONTROLLER_STUB

#include "duiwidgetcontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiWidgetControllerStub : public StubBase {
  public:
   const DuiTheme::ViewType defaultType ;
  virtual void DuiWidgetControllerConstructor(QGraphicsItem *parent);
  virtual void DuiWidgetControllerConstructor(DuiWidgetModel *model, QGraphicsItem *parent);
  virtual void DuiWidgetControllerDestructor();
  virtual DuiAbstractWidgetAnimation * showAnimation();
  virtual DuiAbstractWidgetAnimation * hideAnimation();
  virtual void setModel(DuiWidgetModel *model);
  virtual DuiWidgetModel * model();
  virtual const DuiWidgetModel * model() const;
  virtual DuiTheme::ViewType viewType() const;
  virtual bool isActive() const;
  virtual void setView(DuiWidgetView *view);
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  virtual void setGeometry(const QRectF &rect);
  virtual QRectF boundingRect() const;
  virtual QPainterPath shape() const;
  virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
  virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
  virtual void changeEvent(QEvent *event);
  virtual void setViewType(const DuiTheme::ViewType &type);
  virtual void setActive(bool active);
  virtual void setObjectName(const QString &name);
  virtual void updateData(const QList<const char *> &modifications);
  virtual void setupModel();
  virtual void updateMicroFocus();
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void cancelEvent(DuiCancelEvent *event);
  virtual void orientationChangeEvent(DuiOrientationChangeEvent *event);
  virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
  virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
  virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
  virtual DuiWidgetStyleContainer & style();
  virtual const DuiWidgetStyleContainer & style() const;
  virtual void DuiWidgetControllerConstructor(DuiWidgetControllerPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent);
  virtual const DuiWidgetView * view() const;
}; 

// 2. IMPLEMENT STUB
void DuiWidgetControllerStub::DuiWidgetControllerConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void DuiWidgetControllerStub::DuiWidgetControllerConstructor(DuiWidgetModel *model, QGraphicsItem *parent) {
  Q_UNUSED(model);
  Q_UNUSED(parent);

}
void DuiWidgetControllerStub::DuiWidgetControllerDestructor() {

}
DuiAbstractWidgetAnimation * DuiWidgetControllerStub::showAnimation() {
  stubMethodEntered("showAnimation");
  return stubReturnValue<DuiAbstractWidgetAnimation *>("showAnimation");
}

DuiAbstractWidgetAnimation * DuiWidgetControllerStub::hideAnimation() {
  stubMethodEntered("hideAnimation");
  return stubReturnValue<DuiAbstractWidgetAnimation *>("hideAnimation");
}

void DuiWidgetControllerStub::setModel(DuiWidgetModel *model) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiWidgetModel * >(model));
  stubMethodEntered("setModel",params);
}

DuiWidgetModel * DuiWidgetControllerStub::model() {
  stubMethodEntered("model");
  return stubReturnValue<DuiWidgetModel *>("model");
}

const DuiWidgetModel * DuiWidgetControllerStub::model() const {
  stubMethodEntered("model");
  return stubReturnValue<const DuiWidgetModel *>("model");
}

DuiTheme::ViewType DuiWidgetControllerStub::viewType() const {
  stubMethodEntered("viewType");
  return stubReturnValue<DuiTheme::ViewType>("viewType");
}

bool DuiWidgetControllerStub::isActive() const {
  stubMethodEntered("isActive");
  return stubReturnValue<bool>("isActive");
}

void DuiWidgetControllerStub::setView(DuiWidgetView *view) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiWidgetView * >(view));
  stubMethodEntered("setView",params);
}

void DuiWidgetControllerStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QPainter * >(painter));
  params.append( new Parameter<const QStyleOptionGraphicsItem * >(option));
  params.append( new Parameter<QWidget * >(widget));
  stubMethodEntered("paint",params);
}

void DuiWidgetControllerStub::setGeometry(const QRectF &rect) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QRectF & >(rect));
  stubMethodEntered("setGeometry",params);
}

QRectF DuiWidgetControllerStub::boundingRect() const {
  stubMethodEntered("boundingRect");
  return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath DuiWidgetControllerStub::shape() const {
  stubMethodEntered("shape");
  return stubReturnValue<QPainterPath>("shape");
}

QSizeF DuiWidgetControllerStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::SizeHint >(which));
  params.append( new Parameter<const QSizeF & >(constraint));
  stubMethodEntered("sizeHint",params);
  return stubReturnValue<QSizeF>("sizeHint");
}

void DuiWidgetControllerStub::resizeEvent(QGraphicsSceneResizeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneResizeEvent * >(event));
  stubMethodEntered("resizeEvent",params);
}

void DuiWidgetControllerStub::changeEvent(QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("changeEvent",params);
}

void DuiWidgetControllerStub::setViewType(const DuiTheme::ViewType &type) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiTheme::ViewType & >(type));
  stubMethodEntered("setViewType",params);
}

void DuiWidgetControllerStub::setActive(bool active) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(active));
  stubMethodEntered("setActive",params);
}

void DuiWidgetControllerStub::setObjectName(const QString &name) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(name));
  stubMethodEntered("setObjectName",params);
}

void DuiWidgetControllerStub::updateData(const QList<const char *> &modifications) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<const char *> & >(modifications));
  stubMethodEntered("updateData",params);
}

void DuiWidgetControllerStub::setupModel() {
  stubMethodEntered("setupModel");
}

void DuiWidgetControllerStub::updateMicroFocus() {
  stubMethodEntered("updateMicroFocus");
}

void DuiWidgetControllerStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void DuiWidgetControllerStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseReleaseEvent",params);
}

void DuiWidgetControllerStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseMoveEvent",params);
}

void DuiWidgetControllerStub::cancelEvent(DuiCancelEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiCancelEvent * >(event));
  stubMethodEntered("cancelEvent",params);
}

void DuiWidgetControllerStub::orientationChangeEvent(DuiOrientationChangeEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiOrientationChangeEvent * >(event));
  stubMethodEntered("orientationChangeEvent",params);
}

bool DuiWidgetControllerStub::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsItem * >(watched));
  params.append( new Parameter<QEvent * >(event));
  stubMethodEntered("sceneEventFilter",params);
  return stubReturnValue<bool>("sceneEventFilter");
}

QVariant DuiWidgetControllerStub::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsItem::GraphicsItemChange >(change));
  params.append( new Parameter<const QVariant & >(value));
  stubMethodEntered("itemChange",params);
  return stubReturnValue<QVariant>("itemChange");
}

QVariant DuiWidgetControllerStub::inputMethodQuery(Qt::InputMethodQuery query) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::InputMethodQuery >(query));
  stubMethodEntered("inputMethodQuery",params);
  return stubReturnValue<QVariant>("inputMethodQuery");
}

DuiWidgetStyleContainer & DuiWidgetControllerStub::style() {
  stubMethodEntered("style");
  return *stubReturnValue<DuiWidgetStyleContainer *>("style");
}

const DuiWidgetStyleContainer & DuiWidgetControllerStub::style() const {
  stubMethodEntered("style");
  return *stubReturnValue<const DuiWidgetStyleContainer *>("style");
}

void DuiWidgetControllerStub::DuiWidgetControllerConstructor(DuiWidgetControllerPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);

}
const DuiWidgetView * DuiWidgetControllerStub::view() const {
  stubMethodEntered("view");
  return stubReturnValue<const DuiWidgetView *>("view");
}



// 3. CREATE A STUB INSTANCE
DuiWidgetControllerStub gDefaultDuiWidgetControllerStub;
DuiWidgetControllerStub* gDuiWidgetControllerStub = &gDefaultDuiWidgetControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidgetController::DuiWidgetController(QGraphicsItem *parent) {
  gDuiWidgetControllerStub->DuiWidgetControllerConstructor(parent);
}

DuiWidgetController::DuiWidgetController(DuiWidgetModel *model, QGraphicsItem *parent) {
  gDuiWidgetControllerStub->DuiWidgetControllerConstructor(model, parent);
}

DuiWidgetController::~DuiWidgetController() {
  gDuiWidgetControllerStub->DuiWidgetControllerDestructor();
}

DuiAbstractWidgetAnimation * DuiWidgetController::showAnimation() {
  return gDuiWidgetControllerStub->showAnimation();
}

DuiAbstractWidgetAnimation * DuiWidgetController::hideAnimation() {
  return gDuiWidgetControllerStub->hideAnimation();
}

void DuiWidgetController::setModel(DuiWidgetModel *model) {
  gDuiWidgetControllerStub->setModel(model);
}

DuiWidgetModel * DuiWidgetController::model() {
  return gDuiWidgetControllerStub->model();
}

const DuiWidgetModel * DuiWidgetController::model() const {
  return gDuiWidgetControllerStub->model();
}

DuiTheme::ViewType DuiWidgetController::viewType() const {
  return gDuiWidgetControllerStub->viewType();
}

bool DuiWidgetController::isActive() const {
  return gDuiWidgetControllerStub->isActive();
}

void DuiWidgetController::setView(DuiWidgetView *view) {
  gDuiWidgetControllerStub->setView(view);
}

void DuiWidgetController::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  gDuiWidgetControllerStub->paint(painter, option, widget);
}

void DuiWidgetController::setGeometry(const QRectF &rect) {
  gDuiWidgetControllerStub->setGeometry(rect);
}

QRectF DuiWidgetController::boundingRect() const {
  return gDuiWidgetControllerStub->boundingRect();
}

QPainterPath DuiWidgetController::shape() const {
  return gDuiWidgetControllerStub->shape();
}

QSizeF DuiWidgetController::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const {
  return gDuiWidgetControllerStub->sizeHint(which, constraint);
}

void DuiWidgetController::resizeEvent(QGraphicsSceneResizeEvent *event) {
  gDuiWidgetControllerStub->resizeEvent(event);
}

void DuiWidgetController::changeEvent(QEvent *event) {
  gDuiWidgetControllerStub->changeEvent(event);
}

void DuiWidgetController::setViewType(const DuiTheme::ViewType &type) {
  gDuiWidgetControllerStub->setViewType(type);
}

void DuiWidgetController::setActive(bool active) {
  gDuiWidgetControllerStub->setActive(active);
}

void DuiWidgetController::setObjectName(const QString &name) {
  gDuiWidgetControllerStub->setObjectName(name);
}

void DuiWidgetController::updateData(const QList<const char *> &modifications) {
  gDuiWidgetControllerStub->updateData(modifications);
}

void DuiWidgetController::setupModel() {
  gDuiWidgetControllerStub->setupModel();
}

void DuiWidgetController::updateMicroFocus() {
  gDuiWidgetControllerStub->updateMicroFocus();
}

void DuiWidgetController::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gDuiWidgetControllerStub->mousePressEvent(event);
}

void DuiWidgetController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  gDuiWidgetControllerStub->mouseReleaseEvent(event);
}

void DuiWidgetController::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  gDuiWidgetControllerStub->mouseMoveEvent(event);
}

void DuiWidgetController::cancelEvent(DuiCancelEvent *event) {
  gDuiWidgetControllerStub->cancelEvent(event);
}

void DuiWidgetController::orientationChangeEvent(DuiOrientationChangeEvent *event) {
  gDuiWidgetControllerStub->orientationChangeEvent(event);
}

bool DuiWidgetController::sceneEventFilter(QGraphicsItem *watched, QEvent *event) {
  return gDuiWidgetControllerStub->sceneEventFilter(watched, event);
}

QVariant DuiWidgetController::itemChange(GraphicsItemChange change, const QVariant &value) {
  return gDuiWidgetControllerStub->itemChange(change, value);
}

QVariant DuiWidgetController::inputMethodQuery(Qt::InputMethodQuery query) const {
  return gDuiWidgetControllerStub->inputMethodQuery(query);
}

DuiWidgetStyleContainer & DuiWidgetController::style() {
  return gDuiWidgetControllerStub->style();
}

const DuiWidgetStyleContainer & DuiWidgetController::style() const {
  return gDuiWidgetControllerStub->style();
}

DuiWidgetController::DuiWidgetController(DuiWidgetControllerPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent) {
  gDuiWidgetControllerStub->DuiWidgetControllerConstructor(dd, model, parent);
}

const DuiWidgetView * DuiWidgetController::view() const {
  return gDuiWidgetControllerStub->view();
}


#endif
