#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MLabel>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MApplicationWindow window;
    MApplicationPage page;

    page.setTitle("My First Page");
    page.setCentralWidget(new MLabel("Hello World!"));
    page.appear(&window);

    window.show();

    return app.exec();
}
