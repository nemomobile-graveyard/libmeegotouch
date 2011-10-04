/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef MEXTENSIONAREA_STUB
#define MEXTENSIONAREA_STUB

#include "mextensionarea.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MExtensionAreaStub : public StubBase {
  public:
  virtual void MExtensionAreaConstructor(QGraphicsItem *parent);
  virtual void MExtensionAreaDestructor();
  virtual void MExtensionAreaConstructor(MExtensionAreaPrivate *dd, MWidgetModel *model, QGraphicsItem *parent);
  virtual void addWidget(QGraphicsWidget *widget, MDataStore &store);
  virtual void removeWidget(QGraphicsWidget *widget);
}; 

// 2. IMPLEMENT STUB
void MExtensionAreaStub::MExtensionAreaConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void MExtensionAreaStub::MExtensionAreaDestructor() {

}
void MExtensionAreaStub::MExtensionAreaConstructor(MExtensionAreaPrivate *dd, MWidgetModel *model, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);

}
void MExtensionAreaStub::addWidget(QGraphicsWidget *widget, MDataStore &store) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsWidget * >(widget));
  params.append( new Parameter<MDataStore & >(store));
  stubMethodEntered("addWidget",params);
}

void MExtensionAreaStub::removeWidget(QGraphicsWidget *widget) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsWidget * >(widget));
  stubMethodEntered("removeWidget",params);
}



// 3. CREATE A STUB INSTANCE
MExtensionAreaStub gDefaultMExtensionAreaStub;
MExtensionAreaStub* gMExtensionAreaStub = &gDefaultMExtensionAreaStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MExtensionArea::MExtensionArea(QGraphicsItem *parent) : d_ptr(NULL){
  gMExtensionAreaStub->MExtensionAreaConstructor(parent);
}

MExtensionArea::~MExtensionArea() {
  gMExtensionAreaStub->MExtensionAreaDestructor();
}

MExtensionArea::MExtensionArea(MExtensionAreaPrivate *dd, MWidgetModel *model, QGraphicsItem *parent) : d_ptr(dd) {
  gMExtensionAreaStub->MExtensionAreaConstructor(dd, model, parent);
}

void MExtensionArea::addWidget(QGraphicsWidget *widget, MDataStore &store) {
  gMExtensionAreaStub->addWidget(widget, store);
}

void MExtensionArea::removeWidget(QGraphicsWidget *widget) {
  gMExtensionAreaStub->removeWidget(widget);
}


#endif
