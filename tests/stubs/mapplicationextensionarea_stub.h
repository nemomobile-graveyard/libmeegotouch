#ifndef MAPPLICATIONEXTENSIONAREA_STUB
#define MAPPLICATIONEXTENSIONAREA_STUB

#include "mapplicationextensionarea.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationExtensionAreaStub : public StubBase {
  public:
  virtual void MApplicationExtensionAreaConstructor(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent);
  virtual QList<MApplicationExtensionInterface *> extensions();
  virtual void MApplicationExtensionAreaDestructor();
  virtual void extensionInstantiated(MApplicationExtensionInterface *extension);
  virtual void extensionRemoved(MApplicationExtensionInterface *extension);
  virtual void MApplicationExtensionAreaConstructor(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions);
};

// 2. IMPLEMENT STUB
void MApplicationExtensionAreaStub::MApplicationExtensionAreaConstructor(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) {
  Q_UNUSED(interface);
  Q_UNUSED(enableInProcessExtensions);
  Q_UNUSED(parent);

}
QList<MApplicationExtensionInterface *> MApplicationExtensionAreaStub::extensions() {
  stubMethodEntered("extensions");
  return stubReturnValue<QList<MApplicationExtensionInterface *> >("extensions");
}

void MApplicationExtensionAreaStub::MApplicationExtensionAreaDestructor() {
}

void MApplicationExtensionAreaStub::extensionInstantiated(MApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MApplicationExtensionInterface * >(extension));
  stubMethodEntered("extensionInstantiated",params);
}

void MApplicationExtensionAreaStub::extensionRemoved(MApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MApplicationExtensionInterface * >(extension));
  stubMethodEntered("extensionRemoved",params);
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
MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaConstructor(interface, enableInProcessExtensions, parent);
}

QList<MApplicationExtensionInterface *> MApplicationExtensionArea::extensions() {
  return gMApplicationExtensionAreaStub->extensions();
}

MApplicationExtensionArea::~MApplicationExtensionArea() {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaDestructor();
}

void MApplicationExtensionArea::extensionInstantiated(MApplicationExtensionInterface *extension) {
  gMApplicationExtensionAreaStub->extensionInstantiated(extension);
}

void MApplicationExtensionArea::extensionRemoved(MApplicationExtensionInterface *extension) {
  gMApplicationExtensionAreaStub->extensionRemoved(extension);
}

MApplicationExtensionArea::MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaConstructor(dd, model, parent, interface, enableInProcessExtensions);
}


#endif
