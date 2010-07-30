#include <MApplication>

#include "samplewindow.h"

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    SampleWindow window;

    window.show();

    return app.exec();
}
