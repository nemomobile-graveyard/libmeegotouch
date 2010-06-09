#ifndef MAPPLICATIONEXTENSIONAREA_STUB
#define MAPPLICATIONEXTENSIONAREA_STUB

#include "mapplicationextensionarea.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationExtensionAreaStub : public StubBase {
  public:
  virtual void MApplicationExtensionAreaConstructor(const QString &interface, QGraphicsItem *parent);
  virtual QList<MApplicationExtensionInterface *> extensions();
  virtual void MApplicationExtensionAreaDestructor();
  virtual void MApplicationExtensionAreaConstructor(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions);
};

// 2. IMPLEMENT STUB
void MApplicationExtensionAreaStub::MApplicationExtensionAreaConstructor(const QString &interface, QGraphicsItem *parent) {
  Q_UNUSED(interface);
  Q_UNUSED(parent);

}
QList<MApplicationExtensionInterface *> MApplicationExtensionAreaStub::extensions() {
  stubMethodEntered("extensions");
  return stubReturnValue<QList<MApplicationExtensionInterface *> >("extensions");
}

void MApplicationExtensionAreaStub::MApplicationExtensionAreaDestructor() {
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

QList<MApplicationExtensionInterface *> MApplicationExtensionArea::extensions() {
  return gMApplicationExtensionAreaStub->extensions();
}

MApplicationExtensionArea::~MApplicationExtensionArea() {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaDestructor();
}

MApplicationExtensionArea::MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) {
  gMApplicationExtensionAreaStub->MApplicationExtensionAreaConstructor(dd, model, parent, interface, enableInProcessExtensions);
}


#endif
