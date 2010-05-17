/* This file is part of libmeegotouch
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
#ifndef MVIDEOWIDGETVIEW_P_H
#define MVIDEOWIDGETVIEW_P_H

#include "private/mwidgetview_p.h"

class MVideoWidgetView;
class MVideoWidgetModel;
class MGstVideo;
class QGLShaderProgram;

class MVideoWidgetViewPrivate : public MWidgetViewPrivate
{
public:

    Q_DECLARE_PUBLIC(MVideoWidgetView)

    MVideoWidgetViewPrivate();
    virtual ~MVideoWidgetViewPrivate();

    const MVideoWidgetModel* model() const;
    MVideoWidgetModel* model();

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

    MGstVideo* m_gstVideo;
};

#endif
