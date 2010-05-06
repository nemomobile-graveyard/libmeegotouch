#include <MApplication>
#include <MApplicationWindow>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MApplicationWindow window;

    window.show();

    return app.exec();
}
