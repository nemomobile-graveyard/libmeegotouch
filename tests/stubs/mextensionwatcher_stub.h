#ifndef MEXTENSIONWATCHER_STUB
#define MEXTENSIONWATCHER_STUB

#include "mextensionwatcher.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MExtensionWatcherStub : public StubBase {
  public:
  virtual void MExtensionWatcherConstructor();
  virtual void MExtensionWatcherDestructor();
  virtual void addExtension(QSharedPointer<const MDesktopEntry> extension);
  virtual void removeExtension(const QString &fileName);
  virtual void notifyDataChanged(const QString &path) const;
   QFileSystemWatcher watcher ;
   QHash<QString, QSharedPointer<const MDesktopEntry> > extensionsToWatch ;
}; 

// 2. IMPLEMENT STUB
void MExtensionWatcherStub::MExtensionWatcherConstructor() {

}
void MExtensionWatcherStub::MExtensionWatcherDestructor() {

}
void MExtensionWatcherStub::addExtension(QSharedPointer<const MDesktopEntry> extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<const MDesktopEntry> >(extension));
  stubMethodEntered("addExtension",params);
}

void MExtensionWatcherStub::removeExtension(const QString &fileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(fileName));
  stubMethodEntered("removeExtension",params);
}

void MExtensionWatcherStub::notifyDataChanged(const QString &path) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(path));
  stubMethodEntered("notifyDataChanged",params);
}



// 3. CREATE A STUB INSTANCE
MExtensionWatcherStub gDefaultMExtensionWatcherStub;
MExtensionWatcherStub* gMExtensionWatcherStub = &gDefaultMExtensionWatcherStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MExtensionWatcher::MExtensionWatcher() {
  gMExtensionWatcherStub->MExtensionWatcherConstructor();
}

MExtensionWatcher::~MExtensionWatcher() {
  gMExtensionWatcherStub->MExtensionWatcherDestructor();
}

void MExtensionWatcher::addExtension(QSharedPointer<const MDesktopEntry> extension) {
  gMExtensionWatcherStub->addExtension(extension);
}

void MExtensionWatcher::removeExtension(const QString &fileName) {
  gMExtensionWatcherStub->removeExtension(fileName);
}

void MExtensionWatcher::notifyDataChanged(const QString &path) const {
  gMExtensionWatcherStub->notifyDataChanged(path);
}


#endif
