/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mgraphicsdimeffect.h"
#include <QPainter>
#include <QDebug>
#include <MGLES2Renderer>
#include <QGLFramebufferObject>

#ifndef M_SHADER_SOURCE_DIR
#define M_SHADER_SOURCE_DIR "/usr/share/meegotouch/shaders"
#endif

class MGraphicsDimEffectPrivate
{
    Q_DECLARE_PUBLIC(MGraphicsDimEffect)
    MGraphicsDimEffect *q_ptr;

public:
    MGraphicsDimEffectPrivate(MGraphicsDimEffect *q)
        : q_ptr(q),
          frameBuffer(0),
          texture(0)
    {
        if (!shader) {
            MGLES2Renderer* renderer = MGLES2Renderer::instance();
            if (renderer) {
                shader = renderer->getShaderProgram(M_SHADER_SOURCE_DIR "/dimeffect.frag" );
                shader->setParent(qApp);
            }
        }
    }

    ~MGraphicsDimEffectPrivate()
    {
        delete frameBuffer;
        frameBuffer = 0;
    }

    void draw(QPainter* painter)
    {
        if (MGLES2Renderer::instance() && painter->device()->devType() != QInternal::Image)
            glesDraw(painter);
        else
            plainDraw(painter);
    }

    void glesDraw(QPainter* painter)
    {
        Q_Q(MGraphicsDimEffect);

        qreal dimRatio = q->ratio();

        if (!frameBuffer || !texture) {
            if (!frameBuffer)
                frameBuffer = new QGLFramebufferObject(q->boundingRect().size().toSize(),
                                                       QGLFramebufferObject::CombinedDepthStencil);
            if (!frameBuffer->bind()) {
                qWarning() << "MGraphicsDimEffectPrivate::draw()" << "Can't bind frameBuffer.";
                return;
            }

            glClearColor(0,0,0,0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            QPainter p(frameBuffer);
            q->drawSource(&p);
            texture = frameBuffer->texture();
            frameBuffer->release();
        }

        MGLES2Renderer* renderer = MGLES2Renderer::instance();
        renderer->begin(painter);
        renderer->bindTexture(texture, frameBuffer->size(), 0, "texture0");
        renderer->activateProgram(shader);
        shader->bind();
        shader->setUniformValue("dimRatio", (GLfloat) dimRatio);
        renderer->draw(0,0);
        shader->release();
        renderer->end();
    }

    void plainDraw(QPainter* painter)
    {
        Q_Q(MGraphicsDimEffect);

        qreal dimRatio = q->ratio();

        if (qFuzzyIsNull(dimRatio))
            return;

        QPoint offset;
        Qt::CoordinateSystem system = q->sourceIsPixmap() ? Qt::LogicalCoordinates : Qt::DeviceCoordinates;
        QPixmap pixmap = q->sourcePixmap(system, &offset, QGraphicsEffect::NoPad);
        if (pixmap.isNull())
            return;

        painter->save();

        if (system == Qt::DeviceCoordinates)
            painter->setWorldTransform(QTransform());

        if (pixmap.width() * pixmap.height() < 128 * 128) {
            // this is workaround for escape button panel
            // its rect() is bigger than visible part and also overlaps with navigation bar
            // so instead of drawing a transparent rect on top of it, we modify pixmap itself
            QImage image = pixmap.toImage();
            for (int y = 0; y < image.height(); y++) {
                QRgb* ptr = (QRgb*) image.scanLine(y);
                QRgb* end = ptr + image.width();
                while (ptr != end) {
                    if (qAlpha(*ptr))
                        *ptr = qRgba(dimRatio * qRed(*ptr),
                                     dimRatio * qGreen(*ptr),
                                     dimRatio * qBlue(*ptr),
                                     qAlpha(*ptr));
                    ptr++;
                }
            }
            painter->drawImage(offset, image);
        } else {
            painter->drawPixmap(offset, pixmap);
            painter->fillRect(pixmap.rect().translated(offset), QColor(0, 0, 0, 255 * (1.0 - dimRatio)));
        }

        painter->restore();
    }

    void sourceChanged(QGraphicsEffect::ChangeFlags flags)
    {
        if (flags.testFlag(QGraphicsEffect::SourceBoundingRectChanged)) {
            delete frameBuffer;
            frameBuffer = 0;
            texture = 0;
        } else if (flags.testFlag(QGraphicsEffect::SourceInvalidated)) {
            texture = 0;
        }
    }

    static QGLShaderProgram* shader;
    QGLFramebufferObject* frameBuffer;
    quint32 texture;
};

QGLShaderProgram* MGraphicsDimEffectPrivate::shader = 0;


MGraphicsDimEffect::MGraphicsDimEffect(QObject * p)
    : MGraphicsEffect(p),
      d_ptr(new MGraphicsDimEffectPrivate(this))
{
}

MGraphicsDimEffect::~MGraphicsDimEffect()
{
    delete d_ptr;
}

void MGraphicsDimEffect::draw(QPainter* painter)
{
    Q_D(MGraphicsDimEffect);
    d->draw(painter);
}

void MGraphicsDimEffect::sourceChanged(ChangeFlags flags)
{
    QGraphicsEffect::sourceChanged(flags);
    Q_D(MGraphicsDimEffect);

    if (!isEnabled())
        return;

    d->sourceChanged(flags);
}

qreal MGraphicsDimEffect::baseRatio() const
{
    return style()->dimRatio();
}
