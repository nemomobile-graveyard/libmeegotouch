#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MComponentCache>
#include <MExport>
#include <MButton>

#include "imageviewer.h"
#include "chainingexampleinterfaceadaptor.h"

#include "imageviewer.h"

M_EXPORT int main(int argc, char** argv)
{
    qCritical( "holger chainingexampleapp2" );
    MApplication& app = *MComponentCache::mApplication(argc, argv);

    ImageViewer myImageViewer;

    new ChainingExampleInterfaceAdaptor( &myImageViewer );

    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService( "com.nokia.chainingexampleapp2" );

    ret = connection.registerObject("/", &myImageViewer);

    ret = app.exec();

    return ret;
}
