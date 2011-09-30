/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef SWAPHOOK_H
#define SWAPHOOK_H

#ifdef __arm__
#define EGL
#include <EGL/egl.h>
#   ifdef M_OS_MAEMO5
#       define EGLAPI
#       define EGLAPIENTRY
#   endif //M_OS_MAEMO5
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
  QLinkedList<qint64> timestamps();

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
