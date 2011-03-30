#include "swaphook.h"

#include <cstdio>
#include <ctime>

#include <QObject>
#include <QEvent>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>

#include <MApplication>
#include <MWindow>

#ifdef EGL
#include <dlfcn.h>
#endif

class SwapHookPrivate : public QObject
{
    Q_OBJECT
public:
    SwapHookPrivate();
    ~SwapHookPrivate();

    bool eventFilter(QObject *object, QEvent *e);

    void frameFinished();

    qint64 time();

#ifdef EGL
    EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface);
    typedef EGLAPI EGLBoolean EGLAPIENTRY (*eglSwapBuffers_ptr)(EGLDisplay dpy, EGLSurface surface);
    eglSwapBuffers_ptr func;
#else
    void *func;
#endif

    void *lib;
    bool lurk;
    qint64 lurkBegin;
    bool firstTimestamp;
    QLinkedList<qint64> stamps;

    QElapsedTimer elapsedTimer;
};


#ifdef EGL
extern "C" M_EXPORT EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    return SwapHook::instance()->d->eglSwapBuffers(dpy, surface);
}
#endif

qint64 SwapHookPrivate::time()
{
    return elapsedTimer.elapsed();
}

SwapHookPrivate::SwapHookPrivate() :
        QObject(),
        func(0),
        lib(0),
        lurk(false),
        lurkBegin(0),
        firstTimestamp(true),
        stamps()
{
#ifdef EGL
    QString preload = qgetenv("LD_PRELOAD");

    QStringList libs = preload.split(':', QString::SkipEmptyParts);
    libs << QString("libEGL.so");

    foreach (const QString& candidate, libs) {

        lib = dlopen(candidate.toAscii().data(), RTLD_NOW);

        if(!lib) {
            qWarning("%s:%d: can't dlopen %s (%s)\n", __FILE__, __LINE__, qPrintable(candidate), dlerror());
            continue;
        }

        func = (eglSwapBuffers_ptr)dlsym(lib, "eglSwapBuffers");
        if(!func) {
            qWarning("%s:%d: can't find eglSwapBuffers from %s (%s)\n", __FILE__, __LINE__, qPrintable(candidate), dlerror());
            continue;
        }
        return;
    }
    qFatal("%s:%d: failed to redirect eglSwapBuffers()", __FILE__, __LINE__);
#endif

    elapsedTimer.start();
}

SwapHookPrivate::~SwapHookPrivate()
{
#ifdef EGL
    if(lib) {
        dlclose(lib);
    }
#endif
}

bool SwapHookPrivate::eventFilter(QObject *object, QEvent *e) {
    if (e->type() == QEvent::Paint) {
        frameFinished();
    }

    return QObject::eventFilter(object, e);
}

#ifdef EGL
EGLBoolean SwapHookPrivate::eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    if (!func)
        qFatal("SwapHook object not properly initialized");

    frameFinished();

    return func(dpy, surface);
}
#endif


void SwapHookPrivate::frameFinished() {
    if (lurk) {
        qint64 ts = time();
        if (firstTimestamp) {
            lurkBegin = ts;
            firstTimestamp = false;
        }
        stamps.push_back(ts);
        if (!MApplication::showFps()) {
            // Update again on return to the main loop, in order to refresh
            // the scene as frequently as possible for benchmark purposes.
            QTimer::singleShot(0, MApplication::activeWindow()->viewport(), SLOT(update()));
        }
    }
}

SwapHook::SwapHook()
    : QObject(),
      d(new SwapHookPrivate)
{
}

SwapHook::~SwapHook()
{
}

SwapHook* SwapHook::instance() {
    static SwapHook *gSwapHook = 0;
    if (!gSwapHook) {
        gSwapHook = new SwapHook();
    }
    return gSwapHook;
}

void SwapHook::startLurking()
{
    if(d->lurk)
        return;

    if (!d->func) {
        // fallback to even filtering as swap buffer monitoring is not available
        MApplication::activeWindow()->viewport()->installEventFilter(d.data());
    }

    d->stamps.clear();
    d->lurk = true;
    d->firstTimestamp = true;
    MApplication::activeWindow()->viewport()->update();
}

void SwapHook::stopLurking()
{
    if(!(d->lurk))
        return;

    if (!d->func) {
        MApplication::activeWindow()->viewport()->removeEventFilter(d.data());
    }

    d->lurk = false;
}

QLinkedList<qint64> SwapHook::timestamps() {
    return d->stamps;
}

#include "swaphook.moc"
