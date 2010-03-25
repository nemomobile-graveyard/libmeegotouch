#ifndef DUIAPPLICATIONEXTENSIONAREA_STUB
#define DUIAPPLICATIONEXTENSIONAREA_STUB

#include "duiapplicationextensionarea.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiApplicationExtensionAreaStub : public StubBase {
  public:
  virtual void DuiApplicationExtensionAreaConstructor(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent);
  virtual QList<DuiApplicationExtensionInterface *> extensions();
  virtual void DuiApplicationExtensionAreaDestructor();
  virtual void extensionInstantiated(DuiApplicationExtensionInterface *extension);
  virtual void extensionRemoved(DuiApplicationExtensionInterface *extension);
  virtual void DuiApplicationExtensionAreaConstructor(DuiApplicationExtensionAreaPrivate *dd, DuiApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions);
};

// 2. IMPLEMENT STUB
void DuiApplicationExtensionAreaStub::DuiApplicationExtensionAreaConstructor(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) {
  Q_UNUSED(interface);
  Q_UNUSED(enableInProcessExtensions);
  Q_UNUSED(parent);

}
QList<DuiApplicationExtensionInterface *> DuiApplicationExtensionAreaStub::extensions() {
  stubMethodEntered("extensions");
  return stubReturnValue<QList<DuiApplicationExtensionInterface *> >("extensions");
}

void DuiApplicationExtensionAreaStub::DuiApplicationExtensionAreaDestructor() {
}

void DuiApplicationExtensionAreaStub::extensionInstantiated(DuiApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiApplicationExtensionInterface * >(extension));
  stubMethodEntered("extensionInstantiated",params);
}

void DuiApplicationExtensionAreaStub::extensionRemoved(DuiApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiApplicationExtensionInterface * >(extension));
  stubMethodEntered("extensionRemoved",params);
}

void DuiApplicationExtensionAreaStub::DuiApplicationExtensionAreaConstructor(DuiApplicationExtensionAreaPrivate *dd, DuiApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);
  Q_UNUSED(interface);
  Q_UNUSED(enableInProcessExtensions);

}


// 3. CREATE A STUB INSTANCE
DuiApplicationExtensionAreaStub gDefaultDuiApplicationExtensionAreaStub;
DuiApplicationExtensionAreaStub* gDuiApplicationExtensionAreaStub = &gDefaultDuiApplicationExtensionAreaStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiApplicationExtensionArea::DuiApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) {
  gDuiApplicationExtensionAreaStub->DuiApplicationExtensionAreaConstructor(interface, enableInProcessExtensions, parent);
}

QList<DuiApplicationExtensionInterface *> DuiApplicationExtensionArea::extensions() {
  return gDuiApplicationExtensionAreaStub->extensions();
}

DuiApplicationExtensionArea::~DuiApplicationExtensionArea() {
  gDuiApplicationExtensionAreaStub->DuiApplicationExtensionAreaDestructor();
}

void DuiApplicationExtensionArea::extensionInstantiated(DuiApplicationExtensionInterface *extension) {
  gDuiApplicationExtensionAreaStub->extensionInstantiated(extension);
}

void DuiApplicationExtensionArea::extensionRemoved(DuiApplicationExtensionInterface *extension) {
  gDuiApplicationExtensionAreaStub->extensionRemoved(extension);
}

DuiApplicationExtensionArea::DuiApplicationExtensionArea(DuiApplicationExtensionAreaPrivate *dd, DuiApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) {
  gDuiApplicationExtensionAreaStub->DuiApplicationExtensionAreaConstructor(dd, model, parent, interface, enableInProcessExtensions);
}


#endif
