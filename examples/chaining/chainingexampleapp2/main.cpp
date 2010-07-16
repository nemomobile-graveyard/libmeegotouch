#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MButton>

#include "imageviewer.h"
#include "chainingexampleinterfaceadaptor.h"

#include "imageviewer.h"

int main(int argc, char** argv)
{
    MApplication app(argc, argv);

    ImageViewer myImageViewer;

    new ChainingExampleInterfaceAdaptor( &myImageViewer );

    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService( "com.nokia.ChainingExampleApp2" );

    ret = connection.registerObject("/", &myImageViewer);

    ret = app.exec();

    return ret;
}
