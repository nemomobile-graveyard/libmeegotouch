#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiApplicationWindow>
#include <DuiButton>

#include "imageviewer.h"

int main(int argc, char** argv)
{
    DuiApplication app(argc, argv);
    DuiApplicationWindow w;
    w.setObjectName( "Main Window" );
    w.show();

    DuiApplicationPage p;
    p.appearNow();

    DuiButton b(p.centralWidget());
    b.setText("IMAGE");

    ImageViewer myImageViewer( b );

    return app.exec();
}
