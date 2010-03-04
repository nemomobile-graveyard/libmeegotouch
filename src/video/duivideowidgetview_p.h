/* This file is part of libdui
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */
#ifndef DUIVIDEOWIDGETVIEW_P_H
#define DUIVIDEOWIDGETVIEW_P_H

#include "private/duiwidgetview_p.h"

class DuiVideoWidgetView;
class DuiVideoWidgetModel;
class DuiGstVideo;
class QGLShaderProgram;

class DuiVideoWidgetViewPrivate : public DuiWidgetViewPrivate
{
public:

    Q_DECLARE_PUBLIC(DuiVideoWidgetView)

    DuiVideoWidgetViewPrivate();
    virtual ~DuiVideoWidgetViewPrivate();

    const DuiVideoWidgetModel* model() const;
    DuiVideoWidgetModel* model();

    void update();
    void updateGeometry();

    void videoReady();
    void frameReady();
    void stateChanged();

    void updateVideoGeometry();

    void blitGLFrame();
    void blitRgbTexture(const uchar* data, int  width, int height);
    void blitMultiYuvTexture(const uchar* data, int  width, int height);
    void blitSingleYuvTexture(const uchar* data, int  width, int height);

    void blitSwFrame();
    void prepareBlit(int w, int h);
    void blit(const uchar* data);
    void blit(const uchar* data, int w, int h);

    bool m_useSingleYuvTexture;
    unsigned int m_textures[3];

    QImage *image;
    uchar *bits;
    const int *scaleOffsets;

    bool m_needFillBg;

    QString m_videoFilename;

    QRectF m_scaledVideoRect;

    bool m_fullscreen;

    DuiGstVideo* m_gstVideo;
    QGLShaderProgram* yuv1;
    QGLShaderProgram* yuv3;
};

#endif
