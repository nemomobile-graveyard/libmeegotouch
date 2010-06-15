#ifndef MAPPLICATIONEXTENSIONAREA_STUB
#define MAPPLICATIONEXTENSIONAREA_STUB

#include "mapplicationextensionarea.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationExtensionAreaStub : public StubBase {
  public:
  virtual void MApplicationExtensionAreaConstructor(const QString &interface, QGraphicsItem *parent);
  virtual void MApplicationExtensionAreaDestructor();
  virtual void setInProcessFilter(const QRegExp &inProcessFilter);
  virtual void setOutOfProcessFilter(const QRegExp &outOfProcessFilter);
  virtual void setOrder(const QStringList &order);
  virtual bool init();
  virtual QList<MApplicationExtensionInterface *> extensions();
  virtual void MApplicationExtensionAreaConstructor(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent);
  virtual void MApplicationExtensionAreaConstructor(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface);
  virtual void MApplicationExtensionAreaConstructor(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions);
}; 

// 2. IMPLEMENT STUB
void MApplicationExtensionAreaStub::MApplicationExtensionAreaConstructor(const QString &interface, QGraphicsItem *parent) {
  Q_UNUSED(interface);
  Q_UNUSED(parent);

}
void MApplicationExtensionAreaStub::MApplicationExtensionAreaDestructor() {

}
void MApplicationExtensionAreaStub::setInProcessFilter(const QRegExp &inProcessFilter) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QRegExp & >(inProcessFilter));
  stubMethodEntered("setInProcessFilter",params);
}

void MApplicationExtensionAreaStub::setOutOfProcessFilter(const QRegExp &outOfProcessFilter) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QRegExp & >(outOfProcessFilter));
  stubMethodEntered("setOutOfProcessFilter",params);
}

void MApplicationExtensionAreaStub::setOrder(const QStringList &order) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QStringList & >(order));
  stubMethodEntered("setOrder",params);
}

bool MApplicationExtensionAreaStub::init() {
  stubMethodEntered("init");
  return stubReturnValue<bool>("init");
}

QList<MApplicationExtensionInterface *> MApplicationExtensionAreaStub::extensions() {
  stubMethodEntered("extensions");
  return stubReturnValue<QList<MApplicationExtensionInterface *> >("extensions");
}

void MApplicationExtensionAreaStub::MApplicationExtensionAreaConstructor(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) {
  Q_UNUSED(interface);
  Q_UNUSED(enableInProcessExtensions);
  Q_UNUSED(parent);

}
void MApplicationExtensionAreaStub::MApplicationExtensionAreaConstructor(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);
  Q_UNUSED(interface);

}
void MApplicationExtensionAreaStub::MApplicationExtensionAreaConstructor(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);
  Q_UNUSED(interface);
  Q_UNUSED(enableInProcessExtensions);

}


// 3. CREATE A STUB INSTANCE
MApplicationExtensionAreaStub gDefaultMApplicationExtensionAreaStub;
MApplicationExtensionAreaStub* gMApplicationExtensionAreaStub = &gDefaultMApplicationExtensionAreaStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, QGraphicsItem *parent) {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaConstructor(interface, parent);
}

MApplicationExtensionArea::~MApplicationExtensionArea() {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaDestructor();
}

void MApplicationExtensionArea::setInProcessFilter(const QRegExp &inProcessFilter) {
  gMApplicationExtensionAreaStub->setInProcessFilter(inProcessFilter);
}

void MApplicationExtensionArea::setOutOfProcessFilter(const QRegExp &outOfProcessFilter) {
  gMApplicationExtensionAreaStub->setOutOfProcessFilter(outOfProcessFilter);
}

void MApplicationExtensionArea::setOrder(const QStringList &order) {
  gMApplicationExtensionAreaStub->setOrder(order);
}

bool MApplicationExtensionArea::init() {
  return gMApplicationExtensionAreaStub->init();
}

QList<MApplicationExtensionInterface *> MApplicationExtensionArea::extensions() {
  return gMApplicationExtensionAreaStub->extensions();
}

MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaConstructor(interface, enableInProcessExtensions, parent);
}

MApplicationExtensionArea::MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface) {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaConstructor(dd, model, parent, interface);
}

MApplicationExtensionArea::MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaConstructor(dd, model, parent, interface, enableInProcessExtensions);
}


#endif
