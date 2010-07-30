#include <MApplication>
#include <MApplicationWindow>

#include "samplepage.h"

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MApplicationWindow window;
    SamplePage *page = new SamplePage(1);

    page->appear(&window);
    window.show();

    return app.exec();
}
