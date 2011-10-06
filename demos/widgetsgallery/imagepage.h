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

#ifndef IMAGEPAGE_H
#define IMAGEPAGE_H

#include "templatepage.h"

class MButton;
class MContainer;
class MImageWidget;
class MComboBox;
class MSlider;
class MTextEdit;

class ImagePage : public TemplatePage
{
    Q_OBJECT

public:

    enum ImageProperty {
        ImageZoom = 0,
        ImageTransparency,
        ImageCrop,
        ImageLastItem
    };

    ImagePage();
    virtual ~ImagePage();
    virtual QString timedemoTitle();
    virtual void createContent();
    void setImageZoom(float zoom);
    void setImageOpacity(float opacity);
    void setImageCrop(float width, float height);

public slots:
    void operationChanged(int index);
    void sliderValueChanged(int value);
    void changeImage();

protected:
    virtual void retranslateUi();
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture);

private:
    MContainer *propertiesContainer;
    MComboBox *propertiesComboBox;
    MWidget *visual;
    MImageWidget *image;
    MSlider *slider;
    MTextEdit *changeImageTextEdit;
    QImage *defaultImage;

    int sliderValues[ImageLastItem];
    qreal originalScaleFactor;
};

#endif
