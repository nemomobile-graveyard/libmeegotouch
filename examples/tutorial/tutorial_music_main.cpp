#include <MApplication>
#include <MApplicationWindow>

// The definition of our data classes
#include "data.h"

#include "mainpage.h"

void fillOutData(QList<Artist *> &artistList);

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MApplicationWindow window;
    MainPage *mainPage;

    // That's the data that will be displayed by our application.
    // For the sake of keeping the example as simple as possible we use
    // a very simplistic data structure.
    QList<Artist *> artistsList;
    fillOutData(artistsList);

    mainPage = new MainPage(artistsList);

    mainPage->appear(&window);
    window.show();

    return app.exec();
}

void fillOutData(QList<Artist *> &artistList)
{
    ...
}
