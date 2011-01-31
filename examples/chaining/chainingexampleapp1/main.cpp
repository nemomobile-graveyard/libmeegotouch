#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MButton>

#include "imageviewer.h"

int main(int argc, char** argv)
{
    MApplication app(argc, argv);
    MApplicationWindow w;
    w.setObjectName( "Main Window" );
    w.show();

    MApplicationPage p;
    p.appear(&w);

    MButton b(p.centralWidget());
    b.setText("IMAGE");

    ImageViewer myImageViewer( b );

    return app.exec();
}
