#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiButton>

#include "imageviewer.h"
#include "chainingexampleinterfaceadaptor.h"

#include "imageviewer.h"

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);

    DuiApplicationWindow w;
    w.show();

    DuiApplicationPage p;
    p.appearNow();

    DuiButton b(p.centralWidget());
    b.setText("NO");

    ImageViewer myImageViewer( &b );

    new ChainingExampleInterfaceAdaptor( &myImageViewer );

    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService( "com.nokia.ChainingExampleApp2" );

    ret = connection.registerObject("/", &myImageViewer);

    ret = app.exec();

    return ret;
}
