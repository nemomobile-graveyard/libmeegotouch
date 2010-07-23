#ifndef __SWAPHOOK_H__
#define __SWAPHOOK_H__

// Used to store time in milliseconds.
typedef unsigned long long timestamp;

#ifdef __arm__
#define EGL
#include <EGL/egl.h>
#   ifdef HAVE_N900
#       define EGLAPI
#       define EGLAPIENTRY
#   endif //HAVE_N900
#endif //__arm__

#include <QObject>
#include <QLinkedList>

class SwapHookPrivate;

/**
  * Swap hooks saves the timestamps when eglSwapBuffers is beein called
  * or as fallback monitors paint events.
  *
  */
class SwapHook : public QObject
{
    Q_OBJECT
#ifdef EGL
friend EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers(EGLDisplay dpy, EGLSurface surface);
#endif //EGL

private:
  SwapHook();
  ~SwapHook();

public:

  /**
    * Return the saved timestamps.
    */
  QLinkedList<timestamp> timestamps();

  /**
    * Returns the global SwapHook instance.
    */
  static SwapHook* instance();

public slots:
  /**
    * Starts to monitor buffer switches.
    */
  void startLurking();

  /**
    * Stops to monitor buffer switches.
    */
  void stopLurking();

private:
  QScopedPointer<SwapHookPrivate> d;

};


#endif // __SWAPHOOK_H__
